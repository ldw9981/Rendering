#include "AseFile.h"

AseFile::AseFile()
{
	tDiffuse = NULL;
	tNormal = NULL;
	tHeight = NULL;
}

AseFile::~AseFile()
{
	DeleteAll();
}

int AseFile::Init(char *file_name, int MipLevels, int rotateX90, float scale, int loadNormalMap, int loadHeightMap)
{
	char strMessage[255]={0};

	DeleteAll();
	file = fopen( file_name, "rb");
	if(!file)
	{
		sprintf(strMessage, "Unable to find the file: %s!", file_name);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return 0;
	}
	char *pdest = strrchr( file_name, '/' );
	if( pdest==NULL)directory[0]=NULL;
	else
	{
		strncpy( directory, file_name, pdest-file_name+1);
		directory[pdest-file_name+1]=NULL;
	}
	int ret = ReadAseFile();
	fclose( file );
	if(!ret)return 0;

	if(!LoadTexturesDiffuse(MipLevels))return 0;
	
	if(loadNormalMap)
		if(!LoadTexturesNormal(MipLevels))return 0;
	
	if(loadHeightMap)
		if(!LoadTexturesHeight(MipLevels))return 0;

	if(rotateX90)RotateX90();
	if(scale!=1.0f)Rescale( scale);
	MakeBoundingBoxs();

	for(int i=0; i<objects.size(); i++)
	//	if(objects[i].pFaceNormals==NULL || objects[i].pNormals==NULL)
			ComputeNormals(objects[i]);

	if(loadNormalMap)
		ComputeTangentBinormal();
	
	return 1;
}

void AseFile::CheckVisibility(Frustum &f)
{
	visible = f.BoxInFrustum(min,max);
	if(!visible)return;
	for(int i=0; i<objects.size(); i++)
		objects[i].visible = f.BoxInFrustum(objects[i].min,objects[i].max);
}

void AseFile::AddCollisionFaces(vector<z_face> *faces, const vec center, float radius)
{
	z_face f;
	if(!SphereInBox( center, radius, min, max))return;
	for(int i=0; i<objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];
		if(!SphereInBox( center, radius, obj->min, obj->max))continue;
		for( int j=0; j<obj->numOfFaces; j++)
		{
			zASE_Face *ind = &obj->pFaces[j];
			if( SpherePolygonCollision( center, radius, obj->pVerts[ind->index[0]], obj->pVerts[ind->index[1]], obj->pVerts[ind->index[2]], obj->pFaceNormals[j] ) )
			{
				f.a = obj->pVerts[ind->index[0]];
				f.b = obj->pVerts[ind->index[1]];
				f.c = obj->pVerts[ind->index[2]];
				f.normal = obj->pFaceNormals[j];
				faces->push_back(f);
			}
		}
	}
}

vec AseFile::CheckSimpleCollision(const vec &vp, float radius)
{
	vec newvp = vp;
	if(!SphereInBox( vp, radius, min, max))
		return newvp;
	for(int i=0; i<objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];
		if(!SphereInBox( newvp, radius, obj->min, obj->max))continue;
		for( int j=0; j<obj->numOfFaces; j++)
		{
			zASE_Face *ind = &obj->pFaces[j];
			if( SpherePolygonCollisionRadius05( newvp, radius, obj->pVerts[ind->index[0]], obj->pVerts[ind->index[1]], obj->pVerts[ind->index[2]], obj->pFaceNormals[j] ) )
			{
				float distanceCenterToPlane = DOT3( obj->pFaceNormals[j], newvp) + PlaneDistance( obj->pFaceNormals[j], obj->pVerts[ind->index[0]] );			// d = Ax + By + Cz + D

				if(distanceCenterToPlane>0)
					newvp += (radius-distanceCenterToPlane)*obj->pFaceNormals[j];
				else
					newvp -= (radius+distanceCenterToPlane)*obj->pFaceNormals[j];
			}
		}
	}
	return newvp;
}

void AseFile::Rescale(float scale)
{
	for(int i=0; i<objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];
		obj->min *= scale;
		obj->max *= scale;
		if( obj->pVerts!=NULL)
		{
			for(int j=0; j<obj->numOfVerts; j++)obj->pVerts[j]*=scale;
		}
	}
	for( i=0; i<static_lights.size(); i++)
	{
		static_lights[i].lpos *= scale;
		static_lights[i].light_range *= scale;
	}
	min *= scale;
	max *= scale;
}

void AseFile::RotateX90()
{
	for(int i=0; i<objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];
		if( obj->pVerts!=NULL)
		{
			for(int j=0; j<obj->numOfVerts; j++)
			{
				float z = -obj->pVerts[j].y;
				obj->pVerts[j].y = obj->pVerts[j].z;
				obj->pVerts[j].z = z;
			}
		}
		if( obj->pNormals!=NULL)
		{
			for(int j=0; j<obj->numOfVerts; j++)
			{
				float z = -obj->pNormals[j].y;
				obj->pNormals[j].y = obj->pNormals[j].z;
				obj->pNormals[j].z = z;
			}
		}
		if( obj->pFaceNormals!=NULL)
		{
			for(int j=0; j<obj->numOfFaces; j++)
			{
				float z = -obj->pFaceNormals[j].y;
				obj->pFaceNormals[j].y = obj->pFaceNormals[j].z;
				obj->pFaceNormals[j].z = z;
			}
		}
	}
	for(i=0; i<static_lights.size(); i++)
	{
		float z = -static_lights[i].lpos.y;
		static_lights[i].lpos.y = static_lights[i].lpos.z;
		static_lights[i].lpos.z = z;
	}
}

void AseFile::ComputeNormals(zASE_Object &obj)
{
	if(obj.pFaceNormals==NULL && obj.numOfFaces>0)obj.pFaceNormals = new vec[obj.numOfFaces];
	if(obj.pFaceNormals==NULL)return;
	if(obj.pNormals==NULL && obj.numOfVerts>0)obj.pNormals = new vec[obj.numOfVerts];
	if(obj.pNormals==NULL)return;
	
	for(int j=0; j<obj.numOfFaces; j++)
	{
		vec a,b;
		a = obj.pVerts[obj.pFaces[j].index[1]] - obj.pVerts[obj.pFaces[j].index[0]];
		b = obj.pVerts[obj.pFaces[j].index[2]] - obj.pVerts[obj.pFaces[j].index[0]];
		obj.pFaceNormals[j] = CROSS( a, b);
		obj.pFaceNormals[j].Normalize();
	}

	int count;

	for( j=0; j<obj.numOfVerts; j++)
	{
		count=0;
		obj.pNormals[j].clear();
		for(int k=0; k<obj.numOfFaces; k++)
		{
			if( obj.pFaces[k].index[0]==j || obj.pFaces[k].index[1]==j ||	obj.pFaces[k].index[2]==j )
			{
				obj.pNormals[j] += obj.pFaceNormals[k];
				count++;
			}
		}
		obj.pNormals[j] *= 1.f/(float)count;
		obj.pNormals[j].Normalize();
	}
}

void AseFile::ComputeTangentBinormal()
{
	for(int i=0; i<this->objects.size(); i++)
	{
		if(!objects[i].numOfTexVerts || !objects[i].numOfFaces)continue;

		vec* faceTangent;
		zASE_Object *obj;
		obj = &objects[i];

		faceTangent = new vec[obj->numOfFaces];		if(faceTangent==NULL)return;
		obj->pTangent = new vec[obj->numOfVerts];	if(obj->pTangent==NULL)return;
		obj->pBinormal = new vec[obj->numOfVerts];	if(obj->pBinormal==NULL)return;

		for(int j=0; j<obj->numOfFaces; j++)
		{
			tangent( obj->pVerts[obj->pFaces[j].index[0]],
					 obj->pVerts[obj->pFaces[j].index[1]],
					 obj->pVerts[obj->pFaces[j].index[2]],
					 obj->pTexVerts[obj->pFaceCoord[j].index[0]],
					 obj->pTexVerts[obj->pFaceCoord[j].index[1]],
					 obj->pTexVerts[obj->pFaceCoord[j].index[2]],
					 faceTangent[j] );
		}

		for(j=0; j<obj->numOfVerts; j++)			// Go through all of the vertices
		{
			obj->pTangent[j].clear();
			for(int k=0; k<obj->numOfFaces; k++)	// Go through all of the triangles
			{										// Check if the vertex is shared by another face
				if( obj->pFaces[k].index[0]==j || 
					obj->pFaces[k].index[1]==j ||
					obj->pFaces[k].index[2]==j )
				{
					obj->pTangent[j] += faceTangent[k];
				}
			}
			obj->pTangent[j].Normalize();
			obj->pBinormal[j] = Normalize( CROSS( obj->pNormals[j], obj->pTangent[j] ));
		}
		if(faceTangent!=NULL)delete [] faceTangent;
	}
}

void AseFile::MakeBoundingBoxs()
{
	min.set( 1e+10, 1e+10, 1e+10);
	max.set(-1e+10,-1e+10,-1e+10);
	for(int i=0; i<objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];
		obj->min.set( 1e+10, 1e+10, 1e+10);
		obj->max.set(-1e+10,-1e+10,-1e+10);
		if( obj->pVerts!=NULL)
		{
			for(int j=0; j<obj->numOfVerts; j++)
			{
				ModifyBox( obj->min, obj->max, obj->pVerts[j]);
				obj->visible=1;
			}
		}
	}
	for( i=0; i<objects.size(); i++)
	{
		ModifyBox( min, max, objects[i].min);
		ModifyBox( min, max, objects[i].max);
	}
	visible=1;
}

void AseFile::DeleteObjects()
{
	for(int i=0; i<objects.size(); i++)
	{
		if(objects[i].pFaceCoord!=NULL)delete [] objects[i].pFaceCoord;
		if(objects[i].pFaceNormals!=NULL)delete [] objects[i].pFaceNormals;
		if(objects[i].pFaces!=NULL)delete [] objects[i].pFaces;
		if(objects[i].pTexVerts!=NULL)delete [] objects[i].pTexVerts;
		if(objects[i].pNormals!=NULL)delete [] objects[i].pNormals;
		if(objects[i].pTangent!=NULL)delete [] objects[i].pTangent;
		if(objects[i].pBinormal!=NULL)delete [] objects[i].pBinormal;
		if(objects[i].pVerts!=NULL)delete [] objects[i].pVerts;
		if(objects[i].pFaceCoord_!=NULL)delete [] objects[i].pFaceCoord_;
		if(objects[i].pTexVerts_!=NULL)delete [] objects[i].pTexVerts_;
	}
	objects.clear();
}

void AseFile::DeleteAll()
{
	int i;
	DeleteObjects();
	for( i=0; i<materials.size(); i++)
	{
		if(materials[i].pSubMaterials!=NULL)
		{
			delete [] materials[i].pSubMaterials;
		}
	}
	materials.clear();
	for( i=0; i<textureNames.size(); i++)
		delete [] textureNames[i];
	textureNames.clear();
	if(tDiffuse)delete [] tDiffuse; tDiffuse = NULL;
	if(tNormal)delete [] tNormal;  tNormal = NULL;
	if(tHeight)delete [] tHeight;  tHeight = NULL;
	static_lights.clear();
}

int AseFile::LoadTexturesDiffuse(int MipLevels)
{
	if(textureNames.size()<=0)return 1;
	if(tDiffuse!=NULL)delete [] tDiffuse;
	
	tDiffuse = new texture[textureNames.size()];

	for(int i=0; i<textureNames.size(); i++)
	{
		if(!tDiffuse[i].load(textureNames[i], MipLevels))
			return 0;
	}
	return 1;
}

int AseFile::LoadTexturesNormal(int MipLevels, int plane, float strong)
{
	unsigned int format = D3DFMT_R8G8B8;

	if(textureNames.size()<=0)return 1;
	if(tNormal!=NULL)delete [] tNormal;
	
	tNormal = new texture[textureNames.size()];

	for(int i=0; i<textureNames.size(); i++)
	{
		Image im;
		char name[100],text[400]={0};
		int ok=0;
		char* pdest = strrchr( textureNames[i], '.' );
		if( pdest==NULL) return 0;
		else
		{
			strncpy( text, textureNames[i], pdest-textureNames[i]);
			text[pdest-textureNames[i]]=NULL;
		}
		pdest = strrchr( text, '/' );		// find last '/'
		if(pdest!=NULL)
			strncpy( name, pdest+1, strlen(pdest+1)+1);		// copy name with NULL
		else
			strcpy( name, text);
		
		strcpy( text, directory);
		strcat( text, name);
		strcat( text, "dot3.bmp");
		if( tNormal[i].loadNoMessage( text, MipLevels, format))ok=1;
		if(!ok)
		{
			strcpy( text, directory); strcat( text, name); strcat( text, "dot3.jpg");
			if( tNormal[i].loadNoMessage( text, MipLevels, format))ok=1;
		}
		if(!ok)
		{
			strcpy( text, directory); strcat( text, name); strcat( text, "dot3.tga");
			if( tNormal[i].loadNoMessage( text, MipLevels, format))ok=1;
		}
		if(!ok)
		{
			strcpy( text, directory); strcat( text, name); strcat( text, "bump.bmp");
			if( im.LoadNoMessage(text) )ok=1;
			if(!ok)
			{
				strcpy( text, directory); strcat( text, name); strcat( text, "bump.jpg");
				if( im.LoadNoMessage(text))ok=1;
			}
			if(!ok)
			{
				strcpy( text, directory); strcat( text, name); strcat( text, "bump.tga");
				if( im.LoadNoMessage(text))ok=1;
			}
			if(!ok)
			{
				if( im.LoadNoMessage(textureNames[i]))ok=1;
			}
			if(ok)
			{
				im.Filter(0,plane,strong);
				if( !tNormal[i].load( im, MipLevels, format))return 0;
			}
			else
			{
				strcpy( text, english ? "Couldn't create normal map to ":"Nemozem vytvorit normal mapu k ");
				strcat( text, textureNames[i]);

				MessageBox(hWnd, text, "ERROR", MB_OK);
				return 0;
			}
		}
	}
	return 1;
}

int AseFile::LoadTexturesHeight(int MipLevels)
{
	unsigned int format = D3DFMT_R8G8B8;

	if(textureNames.size()<=0)return 1;
	if(tHeight!=NULL)delete [] tHeight;
	
	tHeight = new texture[textureNames.size()];

	for(int i=0; i<textureNames.size(); i++)
	{
		Image im;
		char name[100],text[400]={0};
		int ok=0;
		char* pdest = strrchr( textureNames[i], '.' );
		if( pdest==NULL) return 0;
		else
		{
			strncpy( text, textureNames[i], pdest-textureNames[i]);
			text[pdest-textureNames[i]]=NULL;
		}
		pdest = strrchr( text, '/' );		// find last '/'
		if(pdest!=NULL)
			strncpy( name, pdest+1, strlen(pdest+1)+1);		// copy name with NULL
		else
			strcpy( name, text);
		
		strcpy( text, directory); strcat( text, name); strcat( text, "bump.bmp");
		if( im.LoadNoMessage(text) )ok=1;
		if(!ok)
		{
			strcpy( text, directory); strcat( text, name); strcat( text, "bump.jpg");
			if( im.LoadNoMessage(text))ok=1;
		}
		if(!ok)
		{
			strcpy( text, directory); strcat( text, name); strcat( text, "bump.tga");
			if( im.LoadNoMessage(text))ok=1;
		}
		if(!ok)
		{
			if( im.LoadNoMessage(textureNames[i]))ok=1;
		}
		if(ok)
		{
			im.setPlanes(3);
			if( !tHeight[i].load( im, MipLevels, format))return 0;
		}
		else
		{
			strcpy( text, english ? "Couldn't load height map to ":"Nemozem nacitat vyskovu mapu k ");
			strcat( text, textureNames[i]);

			MessageBox(hWnd, text, "ERROR", MB_OK);
			return 0;
		}
	}
	return 1;
}

static void MoveSubString( char *a, int pos)				// presunie znaky z pozicii <pos,strlen(a)+1> na <0,strlen(a)+1-pos>
{															// pos ukazuje na pociatocny znak presuvaneho sub retazca
	if(pos<=0)return;
	for( int j=0; a[pos]!=NULL; j++, pos++) a[j]=a[pos];	// presunieme znaky od pozicie len do konca na zaciatok retazca a
	a[j]=NULL;												// zapiseme na konice NULL
}

static int EqualString( char *a, const char *b )
{
	for(int k=0; a[k]=='\t'||a[k]==' '; k++);
	MoveSubString( a, k );
	for(int i=0; a[i]!=NULL && b[i]!=NULL; i++)	if(a[i]!=b[i])return 0;
	if(i==strlen( b))
	{
		MoveSubString( a, strlen(b) );
		return 1;
	}
	return 0;
}

static int FindBracketOpen(char *text)		// fint '{'
{
	for( int i=0; text[i]!=NULL; i++) if(text[i]=='{')return 1;
	return 0;
}

static int FindBracketClose(char *text)		// fint '}'
{
	for( int i=0; text[i]!=NULL; i++) if(text[i]=='}')return 1;
	return 0;
}

int AseFile::ReadAseFile()
{
	char temp[255];
	if(!file)return 0;

	while( fgets(temp, 255, file) )
	{
		if( EqualString( temp, "*MATERIAL_LIST") )		{	if(!ReadMaterial_List())return 0;}
		else if( EqualString( temp, "*GEOMOBJECT") )	{	if(!ReadGeomObject())return 0;}
		else if( EqualString( temp, "*GROUP") )			{	if(!ReadGroup())return 0;}
		else if( EqualString( temp, "*LIGHTOBJECT") )	{	if(!ReadLightObject())return 0;}
	//	else if( EqualString( temp, "*CAMERAOBJECT") )	{	if(!ReadCameraObject())return 0;}
		else ReadUnknown( temp);
	}
	return 1;
}

int AseFile::ReadGroup()
{
	char temp[255];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*GEOMOBJECT ") )		{ if(!ReadGeomObject())return 0;	continue; }
		else if( EqualString( temp, "*LIGHTOBJECT") )	{	if(!ReadLightObject())return 0;}
	//	else if( EqualString( temp, "*HELPEROBJECT ") )	{ if(!ReadGroupHelperObject())return 0;	continue; }
		else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMaterial_List()
{
	char temp[255];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MATERIAL ") )	{	if( !ReadMaterial(atoi(temp)) )return 0; }
	//	else if( EqualString( temp, "*MATERIAL_COUNT ") ) numOfMat=atoi(temp);
		else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

static void CopyString( const char *in, char *out)
{
	for( int i=0; in[i]!='"'&&in[i]!=NULL; i++);
	i++;
	if(in[i]==NULL)out[0]=NULL;
	for( int j=0; in[i]!='"'&&in[i]!=NULL&&j<127; i++, j++)
	{
		out[j]=in[i];
	}
	out[j]=NULL;
}

static inline void CopyVector4( const char *in, vec4 &out){	sscanf( in, "%f %f %f", &out.x, &out.y, &out.z);out.w = 1.0f;}
static inline void CopyVector( const char *in, vec &out){	sscanf( in, "%f %f %f", &out.x, &out.y, &out.z);}
static inline void CopyFloat( const char *in, float &out){	out = atof( in);}

int AseFile::ReadMaterial(int num)
{
	char temp[255];
	zASE_Material mat;
	memset( &mat, 0, sizeof( zASE_Material));
	mat.textureId=-1;
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MATERIAL_NAME ") )			{ CopyString( temp, mat.name );			continue; }
	//	else if( EqualString( temp, "*MATERIAL_CLASS ") )		{ CopyString( temp, mat.classname );	continue; }
		else if( EqualString( temp, "*MATERIAL_AMBIENT ") )		{ CopyVector4( temp, mat.ambient );		continue; }
		else if( EqualString( temp, "*MATERIAL_DIFFUSE ") )		{ CopyVector4( temp, mat.diffuse );		continue; }
		else if( EqualString( temp, "*MATERIAL_SPECULAR ") )		{ CopyVector4( temp, mat.specular );		continue; }
		else if( EqualString( temp, "*MATERIAL_SHINE ") )		{ CopyFloat ( temp, mat.shine );		continue; }
		else if( EqualString( temp, "*MATERIAL_SHINESTRENGTH ") ){ CopyFloat ( temp, mat.shineStrength );continue; }
		else if( EqualString( temp, "*MATERIAL_TRANSPARENCY ") )	{ CopyFloat ( temp, mat.transparency );	continue; }
		else if( EqualString( temp, "*MATERIAL_WIRESIZE ") )		{ CopyFloat ( temp, mat.wiresize );		continue; }
	//	else if( EqualString( temp, "*MATERIAL_SHADING ") )		{ strcpy( mat.shading, temp);			continue; }
	//	else if( EqualString( temp, "*MATERIAL_XP_FALLOFF ") )	{ CopyFloat ( temp, mat.xp_falloff );	continue; }
		else if( EqualString( temp, "*MATERIAL_SELFILLUM ") )	{ CopyFloat ( temp, mat.selfIllum );	continue; }
	//	else if( EqualString( temp, "*MATERIAL_FALLOFF ") )		{ strcpy( mat.falloff, temp);			continue; }
	//	else if( EqualString( temp, "*MATERIAL_XP_TYPE ") )		{ strcpy( mat.xp_type, temp);			continue; }
		else if( EqualString( temp, "*MAP_DIFFUSE ") )		{ if(!ReadMaterialMap( mat ))return 0;		continue; }
		else if( EqualString( temp, "*NUMSUBMTLS ") )			
		{
			mat.numOfSubMaterials = atoi(temp);
			if(mat.numOfSubMaterials<=0)continue;
			mat.pSubMaterials = new zASE_Material[mat.numOfSubMaterials];
			memset( &mat.pSubMaterials[0], 0, mat.numOfSubMaterials*sizeof( zASE_Material));
			for(int i=0; i<mat.numOfSubMaterials; i++)
				mat.pSubMaterials[i].textureId=-1;
			continue; 
		}
		else if( EqualString( temp, "*SUBMATERIAL ") )		
		{ 
			int num = atoi( temp);
			if( num<0 || num>=mat.numOfSubMaterials){	ReadUnknown( temp);	continue; }
			if(!ReadMaterialSubMaterial( mat.pSubMaterials[num], num ))return 0;
			continue; 
		}
		else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	materials.push_back(mat);
	return 1;
}

int AseFile::ReadMaterialMap(zASE_Material &mat, int subMaterial)
{
	char temp[255],name[255]={0};
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		//	*BITMAP "D:\programy\VC++\MyProjects\027_ASE_loader\data\model\earth.bmp"
		if( EqualString( temp, "*BITMAP ") )	{ CopyString( temp, name );			continue; }
		else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	if( name[0]==NULL)return 0;
	FILE* test;
	test = fopen(name,"rb");
	char texture_name[255];
	if(test==NULL)
	{
		strcpy( texture_name, directory);

		char *pdest = strrchr( name, '\\' );
		if( pdest!=NULL && pdest[1]!=NULL)
			strcat(texture_name, pdest+1);
		else
			strcat(texture_name, name);
	}
	else
	{
		fclose(test);
		for(int i=0; i<strlen(name); i++)
			if(name[i]=='\\')name[i]='/';
		strcpy( texture_name, name);
	}
	
	for(int i=0; i<textureNames.size(); i++)
	{
		if(!strcmp(texture_name, textureNames[i]))
		{
			mat.textureId = i;
			i = textureNames.size()+1;
		}
	}
	if(i==textureNames.size())
	{
		char* newName = new char[strlen(texture_name)+1];
		strcpy(newName,texture_name);
		textureNames.push_back(newName);
		mat.textureId = textureNames.size()-1;
	}
	return 1;
}

int AseFile::ReadMaterialSubMaterial(zASE_Material &mat, int subMaterial)
{
	char temp[255];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MATERIAL_NAME ") )			{ CopyString( temp, mat.name );			continue; }
	//	else if( EqualString( temp, "*MATERIAL_CLASS ") )		{ CopyString( temp, mat.classname );	continue; }
		else if( EqualString( temp, "*MATERIAL_AMBIENT ") )		{ CopyVector4( temp, mat.ambient );		continue; }
		else if( EqualString( temp, "*MATERIAL_DIFFUSE ") )		{ CopyVector4( temp, mat.diffuse );		continue; }
		else if( EqualString( temp, "*MATERIAL_SPECULAR ") )		{ CopyVector4( temp, mat.specular );		continue; }
		else if( EqualString( temp, "*MATERIAL_SHINE ") )		{ CopyFloat ( temp, mat.shine );		continue; }
		else if( EqualString( temp, "*MATERIAL_SHINESTRENGTH ") ){ CopyFloat ( temp, mat.shineStrength );continue; }
		else if( EqualString( temp, "*MATERIAL_TRANSPARENCY ") )	{ CopyFloat ( temp, mat.transparency );	continue; }
		else if( EqualString( temp, "*MATERIAL_WIRESIZE ") )		{ CopyFloat ( temp, mat.wiresize );		continue; }
	//	else if( EqualString( temp, "*MATERIAL_SHADING ") )		{ strcpy( mat.shading, temp);			continue; }
	//	else if( EqualString( temp, "*MATERIAL_XP_FALLOFF ") )	{ CopyFloat ( temp, mat.xp_falloff );	continue; }
		else if( EqualString( temp, "*MATERIAL_SELFILLUM ") )	{ CopyFloat ( temp, mat.selfIllum );	continue; }
	//	else if( EqualString( temp, "*MATERIAL_FALLOFF ") )		{ strcpy( mat.falloff, temp);			continue; }
	//	else if( EqualString( temp, "*MATERIAL_XP_TYPE ") )		{ strcpy( mat.xp_type, temp);			continue; }
		else if( EqualString( temp, "*MAP_DIFFUSE ") )		{ if(!ReadMaterialMap( mat, subMaterial))return 0;	continue; }
		else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadGeomObject()
{
	char temp[255];
	zASE_Object	obj;
	memset( &obj, 0, sizeof( zASE_Object));
	obj.rotmatrix[0].set(1,0,0);
	obj.rotmatrix[1].set(0,1,0);
	obj.rotmatrix[2].set(0,0,1);
	do
	{
		if( !fgets(temp, 255, file) )return 0;
	
		if( EqualString( temp, "*NODE_NAME ") )			{ CopyString( temp, obj.name );	continue; }
		else if( EqualString( temp, "*NODE_TM ") )		{ if(!ReadNode_TM( obj ))return 0;continue; }
		else if( EqualString( temp, "*MESH ") )				
		{ if( !ReadMesh(obj) )return 0; else continue; }
	//	else if( EqualString( temp, "*PROP_MOTIONBLUR ") )			{ continue; }		// 0,1
	//	else if( EqualString( temp, "*PROP_CASTSHADOW ") )			{ continue; }		// 0,1
	//	else if( EqualString( temp, "*PROP_RECVSHADOW ") )			{ continue; }		// 0,1
	//	else if( EqualString( temp, "*WIREFRAME_COLOR ") )			{ continue; }		// 3 x float
		else if( EqualString( temp, "*MATERIAL_REF ") )			{ obj.materialID = atoi(temp); continue; }
		else ReadUnknown( temp);

	}while(!FindBracketClose( temp));		// while temp not contain '}'
	objects.push_back( obj);
	return 1;
}

int AseFile::ReadNode_TM(zASE_Object &obj)
{
	char temp[255];
	vec row[3];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
	
		if( EqualString( temp, "*TM_ROW0 ") )		{ CopyVector( temp, row[0]); continue; }
		else if( EqualString( temp, "*TM_ROW1 ") )	{ CopyVector( temp, row[1]); continue; }
		else if( EqualString( temp, "*TM_ROW2 ") )	{ CopyVector( temp, row[2]); continue; }
	//	else if( EqualString( temp, "*TM_POS ") )	{ CopyVector( temp, obj.pos); continue; }
		else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	obj.rotmatrix[0].set( row[0].x, row[1].x, row[2].x);
	obj.rotmatrix[1].set( row[0].y, row[1].y, row[2].y);
	obj.rotmatrix[2].set( row[0].z, row[1].z, row[2].z);
	obj.rotmatrix[0].Normalize();
	obj.rotmatrix[1].Normalize();
	obj.rotmatrix[2].Normalize();
	return 1;
}

/*	*MESH {
		*TIMEVALUE 0
		*MESH_NUMVERTEX 8
		*MESH_NUMFACES 12
		*MESH_VERTEX_LIST {
			*MESH_VERTEX    0	-9.00000	-3.00000	3.00000
		}
		*MESH_FACE_LIST {
			*MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
		}
		*MESH_NUMTVERTEX 0
		*MESH_NUMCVERTEX 0
		*MESH_NORMALS {
			*MESH_FACENORMAL 0	0.00000	0.00000	-1.00000
				*MESH_VERTEXNORMAL 0	0.00000	0.00000	-1.00000
				*MESH_VERTEXNORMAL 2	0.00000	0.00000	-1.00000
				*MESH_VERTEXNORMAL 3	0.00000	0.00000	-1.00000
		}
	}
*/
int AseFile::ReadMesh(zASE_Object &obj)
{
	char temp[255];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
	
	//	if( EqualString( temp, "*TIMEVALUE ") )			{ CopyFloat ( temp, time );	continue; }
		if( EqualString( temp, "*MESH_NUMVERTEX ") )		{ obj.numOfVerts = atoi(temp); continue; }
		else if( EqualString( temp, "*MESH_NUMFACES ") )	{ obj.numOfFaces = atoi(temp); continue; }
		else if( EqualString( temp, "*MESH_VERTEX_LIST ") )	{ if( !ReadMesh_Vertex_List(obj) )return 0; else continue; }
		else if( EqualString( temp, "*MESH_FACE_LIST ") )	{ if( !ReadMesh_Face_List(obj) )return 0; else continue; }
		else if( EqualString( temp, "*MESH_NUMTVERTEX ") )	{ obj.numOfTexVerts = atoi(temp); continue; }
		else if( EqualString( temp, "*MESH_TVERTLIST ") )	{ if( !ReadMesh_TVertList(obj) )return 0; else continue; }
	//	else if( EqualString( temp, "*MESH_NUMTVFACES ") )	{ obj.numOfFaces = atoi(temp); continue; }
		else if( EqualString( temp, "*MESH_TFACELIST ") )	{ if( !ReadMesh_TFaceList(obj) )return 0; else continue; }
	//	else if( EqualString( temp, "*MESH_NUMCVERTEX ") )	{ continue; }
		else if( EqualString( temp, "*MESH_NORMALS ") )		{ if( !ReadMesh_Normals(obj) )return 0; else continue; }
		else if( EqualString( temp, "*MESH_MAPPINGCHANNEL 3 ") ){ if( !ReadMesh_MappingChannel(obj) )return 0; else continue; }
		else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_Vertex_List(zASE_Object &obj)
{
	char temp[255];
	int index=0,i;
	if( obj.numOfVerts!=0)obj.pVerts = new vec[obj.numOfVerts];
	else return ReadUnknown( temp);
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MESH_VERTEX ") )
		{
		//	*MESH_VERTEX    0	-9.00000	-3.00000	3.00000
		//	sscanf( temp, "%d", &index)
			if(index>=obj.numOfVerts)
			{
				if( FindBracketClose( temp) )return 1;
				else continue;
			}
			sscanf( temp, "%d\t%f\t%f\t%f", &i, &obj.pVerts[index].x, &obj.pVerts[index].y, &obj.pVerts[index].z );
			index++;
		}
	//	else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_Face_List(zASE_Object &obj)
{
	char temp[255];
	int index=0,i;
	if( obj.numOfFaces!=0)obj.pFaces = new zASE_Face[obj.numOfFaces];
	else return ReadUnknown( temp);
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MESH_FACE ") )
		{
		//	*MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
		//	sscanf( temp, "%d", &index)
			if(index>=obj.numOfFaces)
			{
				if( FindBracketClose( temp) )return 1;
				else continue;
			}
			sscanf( temp, "%d: A: %hu B: %hu C: %hu", &i, &obj.pFaces[index].index[0], &obj.pFaces[index].index[1], &obj.pFaces[index].index[2] );
			index++;
		}
	//	else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_TVertList(zASE_Object &obj)
{
	char temp[255];
	int index=0,i;
	if( obj.numOfTexVerts!=0)obj.pTexVerts = new vec2[obj.numOfTexVerts];
	else return ReadUnknown( temp);
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MESH_TVERT ") )
		{
		//	*MESH_TVERT 0	0.00000	0.00000	0.00000 
		//	sscanf( temp, "%d", &index)
			if(index>=obj.numOfTexVerts)
			{
				if( FindBracketClose( temp) )return 1;
				else continue;
			}
			sscanf( temp, "%d %f %f", &i, &obj.pTexVerts[index].x, &obj.pTexVerts[index].y );
			index++;
		}
	//	else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_TFaceList(zASE_Object &obj)
{
	char temp[255];
	int index=0,i;
	if( obj.numOfFaces!=0)obj.pFaceCoord = new zASE_Face[obj.numOfFaces];
	else return ReadUnknown( temp);
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MESH_TFACE ") )
		{
		//	*MESH_TFACE 0	9	11	10
		//	sscanf( temp, "%d", &index)
			if(index>=obj.numOfFaces)
			{
				if( FindBracketClose( temp) )return 1;
				else continue;
			}
			sscanf( temp, "%d %hu %hu %hu", &i, &obj.pFaceCoord[index].index[0], &obj.pFaceCoord[index].index[1], &obj.pFaceCoord[index].index[2] );
			index++;
		}
	//	else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_Normals(zASE_Object &obj)
{
	char temp[255];
	int index=0,i;
	if( obj.numOfVerts!=0)obj.pNormals = new vec[obj.numOfVerts];
	else return ReadUnknown( temp);
	if( obj.numOfFaces!=0)obj.pFaceNormals = new vec[obj.numOfFaces];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
	//	*MESH_FACENORMAL 0	0.00000	0.00000	-1.00000
	//		*MESH_VERTEXNORMAL 0	0.00000	0.00000	-1.00000
	//		*MESH_VERTEXNORMAL 2	0.00000	0.00000	-1.00000
	//		*MESH_VERTEXNORMAL 3	0.00000	0.00000	-1.00000

		if( EqualString( temp, "*MESH_FACENORMAL ") )
		{
			sscanf( temp, "%d", &index);
			if(index>=obj.numOfFaces || index<0)continue;
		//	sscanf( temp, "%d %f %f %f", &i, &obj.pFaceNormals[index].x, &obj.pFaceNormals[index].y, &obj.pFaceNormals[index].z );
			vec in;	sscanf( temp, "%d %f %f %f", &i, &in.x, &in.y, &in.z );
			obj.pFaceNormals[index].x = DOT3( obj.rotmatrix[0], in );
			obj.pFaceNormals[index].y = DOT3( obj.rotmatrix[1], in );
			obj.pFaceNormals[index].z = DOT3( obj.rotmatrix[2], in );
			obj.pFaceNormals[index].Normalize();

		}
		else if (EqualString( temp, "*MESH_VERTEXNORMAL "))
		{
			sscanf( temp, "%d", &index);
			if(index>=obj.numOfVerts || index<0)continue;
		//	sscanf( temp, "%d %f %f %f", &i, &obj.pNormals[index].x, &obj.pNormals[index].y, &obj.pNormals[index].z );
			vec in;	sscanf( temp, "%d %f %f %f", &i, &in.x, &in.y, &in.z );
			obj.pNormals[index].x = DOT3( obj.rotmatrix[0], in );
			obj.pNormals[index].y = DOT3( obj.rotmatrix[1], in );
			obj.pNormals[index].z = DOT3( obj.rotmatrix[2], in );
		//	obj.pNormals[index].Normalize();
		}
	//	ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_MappingChannel(zASE_Object &obj)
{
	char temp[255];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
	
		else if( EqualString( temp, "*MESH_NUMTVERTEX ") )	{ obj.numOfTexVerts_ = atoi(temp); continue; }
		else if( EqualString( temp, "*MESH_TVERTLIST ") )	{ if( !ReadMesh_TVertList_(obj) )return 0; else continue; }
	//	else if( EqualString( temp, "*MESH_NUMTVFACES ") )	{ obj.numOfFaces = atoi(temp); continue; }
		else if( EqualString( temp, "*MESH_TFACELIST ") )	{ if( !ReadMesh_TFaceList_(obj) )return 0; else continue; }
		else 
			ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_TVertList_(zASE_Object &obj)
{
	char temp[255];
	int index=0,i;
	if( obj.numOfTexVerts_!=0)obj.pTexVerts_ = new vec2[obj.numOfTexVerts_];
	else return ReadUnknown( temp);
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MESH_TVERT ") )
		{
		//	*MESH_TVERT 0	0.00000	0.00000	0.00000 
		//	sscanf( temp, "%d", &index)
			if(index>=obj.numOfTexVerts_)
			{
				if( FindBracketClose( temp) )return 1;
				else continue;
			}
			sscanf( temp, "%d %f %f", &i, &obj.pTexVerts_[index].x, &obj.pTexVerts_[index].y );
			index++;
		}
	//	else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadMesh_TFaceList_(zASE_Object &obj)
{
	char temp[255];
	int index=0,i;
	if( obj.numOfFaces!=0)obj.pFaceCoord_ = new zASE_Face[obj.numOfFaces];
	else return ReadUnknown( temp);
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*MESH_TFACE ") )
		{
		//	*MESH_TFACE 0	9	11	10
		//	sscanf( temp, "%d", &index)
			if(index>=obj.numOfFaces)
			{
				if( FindBracketClose( temp) )return 1;
				else continue;
			}
			sscanf( temp, "%d %hu %hu %hu", &i, &obj.pFaceCoord_[index].index[0], &obj.pFaceCoord_[index].index[1], &obj.pFaceCoord_[index].index[2] );
			index++;
		}
	//	else ReadUnknown( temp);
	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadLightObject()
{
	z_Light light;
	light.lpos.set(0,0,0);
	light.color.set(1,1,1);
	light.light_range = 100.f;
	light.modification = 1;
	char temp[255];
	int ok=0;

	do
	{
		if( !fgets(temp, 255, file) )return 0;

		if( EqualString( temp, "*LIGHT_TYPE ") )		 { if(!strncmp(temp,"Omni",4))ok=1; continue;}
		else if( EqualString( temp, "*NODE_TM ") )		 { if(!ReadLightObjectNodeTM(light))return 0;}
		else if( EqualString( temp, "*LIGHT_SETTINGS ") ){ if(!ReadLightObjectLightSettings(light))return 0;}
		else ReadUnknown( temp);

	}while(!FindBracketClose( temp));		// while temp not contain '}'
	if(ok)static_lights.push_back(light);
	return 1;
}

int AseFile::ReadLightObjectNodeTM(z_Light &light)
{
	char temp[255];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*TM_POS ") )	{ CopyVector( temp, light.lpos); continue; }
		else ReadUnknown( temp);

	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadLightObjectLightSettings(z_Light &light)
{
	char temp[255];
	do
	{
		if( !fgets(temp, 255, file) )return 0;
		if( EqualString( temp, "*LIGHT_COLOR ") )	{ CopyVector( temp, light.color); continue; }
		if( EqualString( temp, "*LIGHT_ATTNEND ") )	{ light.light_range = atof( temp); continue; }
		else ReadUnknown( temp);

	}while(!FindBracketClose( temp));		// while temp not contain '}'
	return 1;
}

int AseFile::ReadUnknown(char *text)
{
	if( FindBracketOpen(text) )
	{
		char temp[255];
		do
		{
			if( !fgets(temp, 255, file) )return 0;
			if(!ReadUnknown(temp))return 0;
		
		}while(!FindBracketClose( temp));		// while temp not contain '}'
	}
	return 1;
}

