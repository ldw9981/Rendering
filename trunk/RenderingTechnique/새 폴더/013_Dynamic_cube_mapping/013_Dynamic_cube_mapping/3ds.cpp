// 3ds.cpp: implementation of the C3ds class.
//
//////////////////////////////////////////////////////////////////////

#include "3ds.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3ds::C3ds()
{
	error=1;
	textures = NULL;
}

// filter : 0 - nearest | 1 - linear | 2 - bilinear, linear_mipmap_nearest | 3 - trilinear, linear_mipmap_linear
C3ds::C3ds(char *file_name, int filter, int faceNormal, int shadowObject)
{
	z3ds_Chunk ch;
	char strMessage[255]={0};
	textures = NULL;
	isShadowObject = shadowObject;
	isFaceNormal = faceNormal;

	file = fopen( file_name, "rb");
	
	if(!file)
	{
		sprintf(strMessage, "Unable to find the file: %s!", file_name);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		error = 1;
		return;
	}

	ReadChunk( &ch);
	// Make sure this is a 3DS file
	if (ch.ID != MAIN3DS)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", file_name);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		error = 1;
		return;
	}
	ReadNextChunk( &ch);
	fclose( file);

	if(isShadowObject)
	{
		for(int i=0; i<objects.size(); i++)
		{
			if(objects[i].numOfFaces)
			{
				objects[i].pFacesShadow = new z3ds_FaceShadow[objects[i].numOfFaces];
				if(objects[i].pFacesShadow==NULL){error=1; return;}
			}
		}
		SetConnectivity();
	}

	ComputeNormals();

	if(materials.size()>0)textures = new texture[materials.size()];
	// Go through all the materials
	for(int i = 0; i < materials.size(); i++)
	{
		// Check to see if there is a file name to load in this material
		if(strlen(materials[i].strFile) > 0)
		{
			// Use the name of the texture file to load the bitmap, with a texture ID (i).
			char text[300];
			
			char* pdest = strrchr( file_name, '/' );		// find '/'
			if( pdest==NULL)strncpy(text, materials[i].strFile, 300);
			else
			{
				strncpy( text, file_name, pdest-file_name+1);
				text[pdest-file_name+1]=NULL;
				strcat(text, materials[i].strFile);
			}
			if( !textures[i].load( text, 1, filter))error = 1;
		}
		// Set the texture ID for this material
		materials[i].texureId = i;
	}
//	RotateX90();
//	Rescale(3.0f);
}

C3ds::~C3ds()
{
	for(int i=0; i<dxobjs.size(); i++)
	{
		if( dxobjs[i].pvb != NULL )dxobjs[i].pvb->Release();
	}
	dxobjs.clear();

	delete [] textures;
	DeleteObjects();
	materials.clear();
}


void C3ds::ReadChunk(z3ds_Chunk *pChunk)
{
	// This reads the chunk ID which is 2 bytes.
	// The chunk ID is like OBJECT or MATERIAL.  It tells what data is
	// able to be read in within the chunks section.  
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, file);

	// Then, we read the length of the chunk which is 4 bytes.
	// This is how we know how much to read in, or read past.
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, file);
}

void C3ds::ReadNextChunk(z3ds_Chunk *old)
{
	z3ds_Chunk ch;
		
	while( old->bytesRead < old->length )
	{
		ReadChunk( &ch);

		switch( ch.ID)
		{
		case VERSION:							// This holds the version of the file
			{
				unsigned short version = 0;					// This will hold the file version
				ch.bytesRead += fread(&version, 1, ch.length - ch.bytesRead, file);
				// If the file version is over 3, give a warning that there could be a problem
				if (version > 0x03)
					MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			}
			break;
		case EDIT3DS:				// 0x3D3D
			ReadEdit( &ch);
			break;
		case KEYF3DS:				// 0xB000
		//	Read_KeyF( &ch);
			fseek( file, ch.length-ch.bytesRead, SEEK_CUR);
			ch.bytesRead = ch.length;
			break;
		default:
			fseek( file, ch.length-ch.bytesRead, SEEK_CUR);
			ch.bytesRead = ch.length;
		}
		old->bytesRead += ch.bytesRead;
	}
}

void C3ds::ReadEdit(z3ds_Chunk *old)
{
	z3ds_Chunk ch;
		
	while( old->bytesRead < old->length )
	{
		ReadChunk( &ch);

		switch( ch.ID)
		{
		case EDIT_MATERIAL:				// 0xAFFF
			ReadEditMaterial( &ch);
			break;
		case EDIT_OBJECT:				// 0x4000
			ReadEditObject( &ch);
			break;
		default:
			fseek( file, ch.length-ch.bytesRead, SEEK_CUR);
			ch.bytesRead = ch.length;
		}
		old->bytesRead += ch.bytesRead;
	}
}

void C3ds::ReadEditMaterial(z3ds_Chunk *old)
{
	unsigned char buf[100];
	unsigned char color[3];
	z3ds_Chunk ch, temp;;
	z3ds_MaterialInfo mat;
	memset( &mat, 0, sizeof(z3ds_MaterialInfo));
		
	while( old->bytesRead < old->length )
	{
		ReadChunk( &ch);

		switch( ch.ID)
		{
		case 0xA000:				// This holds the material name
			ch.bytesRead += fread( mat.strName, 1, ch.length-ch.bytesRead, file);
			break;
		case 0xA010:				// This holds the ambient color of the object/material
			{
				ReadChunk( &temp);
				fread( color, 1, temp.length-temp.bytesRead, file);
				mat.ambient.set( (float)color[0]/255.f, (float)color[1]/255.f, (float)color[2]/255.f );
				ch.bytesRead += temp.length;
			}
			break;
		case 0xA020:				// This holds the diffuse color of the object/material
			{
				ReadChunk( &temp);
				fread( color, 1, temp.length-temp.bytesRead, file);
				mat.diffuse.set( (float)color[0]/255.f, (float)color[1]/255.f, (float)color[2]/255.f );
				ch.bytesRead += temp.length;
			}
			break;
		case 0xA030:				// This holds the specular color of the object/material
			{
				ReadChunk( &temp);
				fread( color, 1, temp.length-temp.bytesRead, file);
				mat.specular.set( (float)color[0]/255.f, (float)color[1]/255.f, (float)color[2]/255.f );
				ch.bytesRead += temp.length;
			}
			break;
		case 0xA040:				// This holds the specular shininess of the object/material
			{
				ReadChunk( &temp);
				fread( color, 1, temp.length-temp.bytesRead, file);
				mat.shininess = (float)color[0];
				ch.bytesRead += temp.length;
			}
			break;
		case 0xA041:				// This holds the specular level of the object/material
			{
				ReadChunk( &temp);
				fread( color, 1, temp.length-temp.bytesRead, file);
				mat.specular = mat.specular*0.01f*(256.f*color[1]+color[0]);
				ch.bytesRead += temp.length;
			}
			break;
		case 0xA084:				// This holds the specular level of the object/material
			{
				ReadChunk( &temp);
				fread( color, 1, temp.length-temp.bytesRead, file);
				mat.emission = mat.diffuse*0.01f*color[0];
				ch.bytesRead += temp.length;
			}
			break;
		case 0xA200:				// This is a header for a new material
			ReadEditMaterialTexture( &ch, mat);
			break;
		case 0xA300:				// This holds the file name of the texture
			ch.bytesRead += fread( mat.strFile, 1, ch.length-ch.bytesRead, file);
			break;
		default:
			fread( buf, 1, ch.length-ch.bytesRead, file);
		//	fseek( file, ch.length-ch.bytesRead, SEEK_CUR);
			ch.bytesRead = ch.length;
		}
		old->bytesRead += ch.bytesRead;
	}
	materials.push_back(mat);
}

void C3ds::ReadEditMaterialTexture(z3ds_Chunk *old, z3ds_MaterialInfo &mat)
{
	z3ds_Chunk ch;
	char buf[100];
	
	while( old->bytesRead < old->length )
	{
		ReadChunk( &ch);

		switch( ch.ID)
		{
		case 0xA300:				// This holds the file name of the texture
			ch.bytesRead += fread( mat.strFile, 1, ch.length-ch.bytesRead, file);
			break;
		default:
			fread( buf, 1, ch.length-ch.bytesRead, file);
		//	fseek( file, ch.length-ch.bytesRead, SEEK_CUR);
			ch.bytesRead = ch.length;
		}
		old->bytesRead += ch.bytesRead;
	}
}

void C3ds::ReadEditObject(z3ds_Chunk *old)
{
	z3ds_Chunk ch;
	z3ds_Object obj;// = {0};
	memset( &obj, 0, sizeof(z3ds_Object));
	old->bytesRead += GetString(obj.strName);

	while( old->bytesRead < old->length )
	{
		ReadChunk( &ch);

		switch( ch.ID)
		{
		case OBJ_TRIMESH:				// 0x4100
			ReadEditObjectTriMesh( &ch, obj);
			break;
	//	case OBJ_LIGHT     0x4600
	//	case OBJ_CAMERA    0x4700

	//	case OBJ_UNKNWN01  0x4010
	//	case OBJ_UNKNWN02  0x4012 //---- Could be shadow
		default:
			fseek( file, ch.length-ch.bytesRead, SEEK_CUR);
			ch.bytesRead = ch.length;
		}
		old->bytesRead += ch.bytesRead;
	}
	objects.push_back( obj);
}


int C3ds::GetString(char *pBuffer)
{
	int index = 0;

	fread(pBuffer, 1, 1, file);
	// Loop until we get NULL
	while (*(pBuffer + index++) != 0) 
		fread(pBuffer + index, 1, 1, file);		// Read in a character at a time until we hit NULL.
	return strlen(pBuffer) + 1;	// Return the string length, which is how many bytes we read in (including the NULL)
}


void C3ds::ReadEditObjectTriMesh(z3ds_Chunk *old, z3ds_Object &obj)
{
	z3ds_Chunk ch;

	while( old->bytesRead < old->length )
	{
		ReadChunk( &ch);

		switch( ch.ID)
		{
		case TRI_VERTEXL:			// 0x4110
			ReadEditObjectTriMeshVertex( &ch, obj);
			break;
//		case TRI_FACEL2:			// 0x4111 
		case TRI_FACEL1:			// 0x4120
			ReadEditObjectTriMeshFace( &ch, obj);
			break;
		case 0x4130:				// Face Material Chunk 
			ReadEditObjectTriMeshMaterial( &ch, obj);
			break;
		case 0x4140:				// Mapping coordinates
			ReadEditObjectTriMeshUV( &ch, obj);
			break;
		case TRI_SMOOTH:			// 0x4150
		case TRI_LOCAL:				// 0x4160
		case TRI_VISIBLE:			// 0x4165
		default:
			fseek( file, ch.length-ch.bytesRead, SEEK_CUR);
			ch.bytesRead = ch.length;
		}
		old->bytesRead += ch.bytesRead;
	}
}

void C3ds::ReadEditObjectTriMeshVertex(z3ds_Chunk *old, z3ds_Object &obj)
{
	old->bytesRead += fread( &obj.numOfVerts, 1, 2, file);
	obj.pVerts = new vec[obj.numOfVerts];
	if(obj.pVerts==NULL){ error=1; return;}
	old->bytesRead += fread( obj.pVerts, 1, sizeof(vec)*obj.numOfVerts, file);
}

void C3ds::ReadEditObjectTriMeshFace(z3ds_Chunk *old, z3ds_Object &obj)
{
	old->bytesRead += fread( &obj.numOfFaces, 1, 2, file);
	obj.pFaces = new z3ds_Face[obj.numOfFaces];
	if(obj.pFaces==NULL){ error=1; return;}

	unsigned short *temp;
	temp = new unsigned short[8*obj.numOfFaces];
	if(temp==NULL){ error=1; return;}
	old->bytesRead += fread( temp, 1, 2*4*obj.numOfFaces, file);
	for(unsigned short i=0; i<obj.numOfFaces; i++)
	{
		obj.pFaces[i].vertIndex[0] = temp[i*4+0];
		obj.pFaces[i].vertIndex[1] = temp[i*4+1];
		obj.pFaces[i].vertIndex[2] = temp[i*4+2];
	}
	delete [] temp;
}

void C3ds::ReadEditObjectTriMeshMaterial(z3ds_Chunk *old, z3ds_Object &obj)
{
	char strMaterial[255] = {0};
	old->bytesRead += GetString( strMaterial );

	for(int i=0; i<materials.size(); i++)
	{
		if(strcmp(strMaterial, materials[i].strName) == 0)	// If the material we just read in matches the current texture name
		{
			obj.materialID = i;
			if(strlen(materials[i].strFile) > 0) obj.bHasTexture = true;
			break;
		}
		else
			obj.materialID = -1;	// Set the ID to -1 to show there is no material for this object
	}
	fseek( file, old->length-old->bytesRead, SEEK_CUR);
	old->bytesRead = old->length;
}

void C3ds::ReadEditObjectTriMeshUV(z3ds_Chunk *old, z3ds_Object &obj)
{
	old->bytesRead += fread( &obj.numTexVertex, 1, 2, file);
	obj.pTexVerts = new vec2[ obj.numTexVertex];
	old->bytesRead += fread( obj.pTexVerts, 1, old->length-old->bytesRead, file);
}

void C3ds::ComputeNormals()
{
	for(int i=0; i<this->objects.size(); i++)
	{
		vec* faceN;
		z3ds_Object *obj;
		obj = &objects[i];

		faceN = new vec[obj->numOfFaces];
		if(faceN==NULL)return;
		obj->pNormals = new vec[obj->numOfVerts];
		if(obj->pNormals==NULL)return;

		for(int j=0; j<obj->numOfFaces; j++)
		{
			vec a,b;
			a = obj->pVerts[obj->pFaces[j].vertIndex[1]] - obj->pVerts[obj->pFaces[j].vertIndex[0]];
			b = obj->pVerts[obj->pFaces[j].vertIndex[2]] - obj->pVerts[obj->pFaces[j].vertIndex[0]];
			faceN[j] = CROSS( a, b);
			if(isShadowObject)
			{
				obj->pFacesShadow[j].normal=faceN[j];
				obj->pFacesShadow[j].normal.Normalize();
				obj->pFacesShadow[j].delta = -DOT3( obj->pFacesShadow[j].normal, obj->pVerts[obj->pFaces[j].vertIndex[0]]);
			}
		//	if(isFaceNormal)
			//	faceN[j].Normalize();
		}
		if(isFaceNormal)
		{
			obj->pNormals = faceN;
			faceN = NULL;
			continue;
		}

		int count;
		for(j=0; j<obj->numOfVerts; j++)			// Go through all of the vertices
		{
			count=0;
			obj->pNormals[j].clear();				
			for(int k=0; k<obj->numOfFaces; k++)	// Go through all of the triangles
			{										// Check if the vertex is shared by another face
				if( obj->pFaces[k].vertIndex[0]==j || 
					obj->pFaces[k].vertIndex[1]==j ||
					obj->pFaces[k].vertIndex[2]==j )
				{
					obj->pNormals[j] += faceN[k];
					count++;
				}
			}
			obj->pNormals[j] *= 1.f/(float)count;
			obj->pNormals[j].Normalize();
		}
		if(faceN!=NULL)delete [] faceN;
	}
}

// texturing: 
// 0-vypnuty	// nenastavuje sa akt. textura, glDisable(GL_TEXTURE_2D);, nezadavaju sa Tex Coord
// 1-zapnuty	// nastavuje sa akt. textura, glEnable(GL_TEXTURE_2D);, zadavaju sa Tex Coord
// 2-environmental mapping	// nenastavuje sa akt. textura, nezadavaju sa Tex Coord
// 3-bez zmeny	// nenastavuje sa akt. textura, zadavaju sa Tex Coord
// material
// 0-ziadne zmeny ohladne nastavenia farieb
// 1-farby su nastavene podla materialu, ak je zapnute osvetlenie nastavuje sa material

void C3ds::Render(int texturing, int material, DWORD hVertexShader)
{
	for(int i=0; i<dxobjs.size(); i++)
	{
		z3ds_MaterialInfo *mat=NULL;

		if( dxobjs[i].materialID>=0 && dxobjs[i].materialID<materials.size() )
			mat = &materials[dxobjs[i].materialID];
		
		if(mat!=NULL)
		{
			if(material)
			{
				D3DMATERIAL8 mtrl;
				ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
				mtrl.Diffuse.r = mtrl.Ambient.r = mat->diffuse.x;
				mtrl.Diffuse.g = mtrl.Ambient.g = mat->diffuse.y;
				mtrl.Diffuse.b = mtrl.Ambient.b = mat->diffuse.z;
				mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
				pd->SetMaterial( &mtrl );
			}

			switch(texturing)
			{
			case 0:
				pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
				break;
			case 1:
				if(dxobjs[i].have_texture_coord_0)
				{
					textures[mat->texureId].SetTexture(0);
					pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
					pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
					pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
					pd->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
				}
				else
					pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
				break;
			}
		}
		pd->SetStreamSource( 0, dxobjs[i].pvb, sizeof(z3ds_CUSTOMVERTEX) );
		
		pd->SetVertexShader( hVertexShader );

		for(int j=0; j<dxobjs[i].count-21000; j+=21000)
			pd->DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
		pd->DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].count-j );

		if(texturing==1)pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
	}

	if(material)
	{
		D3DMATERIAL8 mtrl;
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		pd->SetMaterial( &mtrl );
	}

/*	int lighting = glIsEnabled( GL_LIGHTING);
	vec4 ambient, diffuse, specular, emission;
	float shininess;
	
	if(material)
	{
		if(lighting)
		{
			glGetMaterialfv( GL_FRONT, GL_AMBIENT, ambient.v);
			glGetMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse.v);
			glGetMaterialfv( GL_FRONT, GL_SPECULAR, specular.v);
			glGetMaterialfv( GL_FRONT, GL_EMISSION, emission.v);
			glGetMaterialfv( GL_FRONT, GL_SHININESS, &shininess);
		}
		else
		{
			glGetFloatv( GL_CURRENT_COLOR, diffuse.v);
		}
	}

	// Since we know how many objects our model has, go through each of them.
	for(int i = 0; i < objects.size(); i++)
	{
		int texturing_=texturing;
		// Make sure we have valid objects just in case. (size() is in the vector class)
		if(objects.size() <= 0) break;

		// Get the current object that we are displaying
		z3ds_Object *obj = &objects[i];
			
		// Check to see if this object has a texture map, if so bind the texture to it.
		if( texturing_==1 )
		{
			if(obj->bHasTexture ) 
				glBindTexture(GL_TEXTURE_2D, textures[obj->materialID].id);// Bind the texture map to the object by it's materialID
			else 
				texturing_ = 0;
		}
		if( obj->pTexVerts==NULL && texturing_%2) texturing_=0;
		if( texturing_==0) glDisable(GL_TEXTURE_2D);
		if( texturing_==1) glEnable(GL_TEXTURE_2D);

		if( material && materials.size() && obj->materialID >= 0) 
		{
			if(lighting)
			{
				glMaterialfv( GL_FRONT, GL_AMBIENT, materials[obj->materialID].ambient.v);
				glMaterialfv( GL_FRONT, GL_DIFFUSE, materials[obj->materialID].diffuse.v);
				glMaterialfv( GL_FRONT, GL_SPECULAR, materials[obj->materialID].specular.v);
				glMaterialfv( GL_FRONT, GL_EMISSION, materials[obj->materialID].emission.v);
				glMaterialf( GL_FRONT, GL_SHININESS, materials[obj->materialID].shininess);
			}
			else
				glColor3fv( (diffuse*materials[obj->materialID].diffuse).v);
		}

		glBegin(GL_TRIANGLES);	// Begin drawing

			// Go through all of the faces (polygons) of the object and draw them
			for(int j = 0; j < obj->numOfFaces; j++)
			{

				if(isFaceNormal)glNormal3fv( obj->pNormals[j].v );

				// Go through each corner of the triangle and draw it.
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// Get the index for each point of the face
					int index = obj->pFaces[j].vertIndex[whichVertex];
			
					// Give OpenGL the normal for this vertex.
					if(!isFaceNormal)glNormal3f(obj->pNormals[ index ].x, obj->pNormals[ index ].y, obj->pNormals[ index ].z);
				
					// If the object has a texture associated with it, give it a texture coordinate.
					if( texturing_%2 )	glTexCoord2f(obj->pTexVerts[ index ].x, obj->pTexVerts[ index ].y);

					// Pass in the current vertex of the object (Corner of current face)
					glVertex3f(obj->pVerts[ index ].x, obj->pVerts[ index ].y, obj->pVerts[ index ].z);
				}
			}

		glEnd();								// End the drawing
	}
	
	if(material)
	{
		if(lighting)
		{
			glMaterialfv( GL_FRONT, GL_AMBIENT, ambient.v);
			glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse.v);
			glMaterialfv( GL_FRONT, GL_SPECULAR, specular.v);
			glMaterialfv( GL_FRONT, GL_EMISSION, emission.v);
			glMaterialf( GL_FRONT, GL_SHININESS, shininess);
		}
		else
		{
			glColor3fv( diffuse.v);
		}
	}*/
}


void C3ds::Render_line(int texturing, int material)
{
/*	int lighting = glIsEnabled( GL_LIGHTING);
	vec4 ambient, diffuse, specular, emission;
	float shininess;
	
	if(material)
	{
		if(lighting)
		{
			glGetMaterialfv( GL_FRONT, GL_AMBIENT, ambient.v);
			glGetMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse.v);
			glGetMaterialfv( GL_FRONT, GL_SPECULAR, specular.v);
			glGetMaterialfv( GL_FRONT, GL_EMISSION, emission.v);
			glGetMaterialfv( GL_FRONT, GL_SHININESS, &shininess);
		}
		else
		{
			glGetFloatv( GL_CURRENT_COLOR, diffuse.v);
		}
	}

	// Since we know how many objects our model has, go through each of them.
	for(int i = 0; i < objects.size(); i++)
	{
		int texturing_=texturing;
		// Make sure we have valid objects just in case. (size() is in the vector class)
		if(objects.size() <= 0) break;

		// Get the current object that we are displaying
		z3ds_Object *obj = &objects[i];
			
		// Check to see if this object has a texture map, if so bind the texture to it.
		if( texturing_==1 )
		{
			if(obj->bHasTexture ) 
				glBindTexture(GL_TEXTURE_2D, textures[obj->materialID].id);// Bind the texture map to the object by it's materialID
			else 
				texturing_ = 0;
		}
		if( obj->pTexVerts==NULL && texturing_%2) texturing_=0;
		if( texturing_==0) glDisable(GL_TEXTURE_2D);
		if( texturing_==1) glEnable(GL_TEXTURE_2D);

		if( material && materials.size() && obj->materialID >= 0) 
		{
			if(lighting)
			{
				glMaterialfv( GL_FRONT, GL_AMBIENT, materials[obj->materialID].ambient.v);
				glMaterialfv( GL_FRONT, GL_DIFFUSE, materials[obj->materialID].diffuse.v);
				glMaterialfv( GL_FRONT, GL_SPECULAR, materials[obj->materialID].specular.v);
				glMaterialfv( GL_FRONT, GL_EMISSION, materials[obj->materialID].emission.v);
				glMaterialf( GL_FRONT, GL_SHININESS, materials[obj->materialID].shininess);
			}
			else
				glColor3fv( (diffuse*materials[obj->materialID].diffuse).v);
		}

		// Go through all of the faces (polygons) of the object and draw them
		for(int j = 0; j < obj->numOfFaces; j++)
		{
			if(isFaceNormal)glNormal3fv( obj->pNormals[j].v );
			glBegin(GL_LINE_LOOP);
				// Go through each corner of the triangle and draw it.
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// Get the index for each point of the face
					int index = obj->pFaces[j].vertIndex[whichVertex];
			
					// Give OpenGL the normal for this vertex.
					if(!isFaceNormal)glNormal3f(obj->pNormals[ index ].x, obj->pNormals[ index ].y, obj->pNormals[ index ].z);
			
				// If the object has a texture associated with it, give it a texture coordinate.
				if( texturing_%2 )	glTexCoord2f(obj->pTexVerts[ index ].x, obj->pTexVerts[ index ].y);

				// Pass in the current vertex of the object (Corner of current face)
				glVertex3f(obj->pVerts[ index ].x, obj->pVerts[ index ].y, obj->pVerts[ index ].z);
			}
			glEnd();
		}
	}
	
	if(material)
	{
		if(lighting)
		{
			glMaterialfv( GL_FRONT, GL_AMBIENT, ambient.v);
			glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse.v);
			glMaterialfv( GL_FRONT, GL_SPECULAR, specular.v);
			glMaterialfv( GL_FRONT, GL_EMISSION, emission.v);
			glMaterialf( GL_FRONT, GL_SHININESS, shininess);
		}
		else
		{
			glColor3fv( diffuse.v);
		}
	}*/
}


void C3ds::Render_normals( float size)
{
/*	glPushAttrib( GL_ENABLE_BIT | GL_LIGHTING_BIT );
	glDisable(GL_TEXTURE_2D);
	glColor3ub(255, 255, 0);
	glDisable( GL_LIGHTING);
	for(int i = 0; i < objects.size(); i++)
	{
		if(objects.size() <= 0) break;
		z3ds_Object *obj = &objects[i];

		glBegin( GL_LINES );
		if(isFaceNormal)
		{
			for( int j=0; j<obj->numOfFaces; j++)
			{
				vec center;

				center = obj->pVerts[obj->pFaces[j].vertIndex[0]];
				center += obj->pVerts[obj->pFaces[j].vertIndex[1]];
				center += obj->pVerts[obj->pFaces[j].vertIndex[2]];
				center *=1/3.f;
				glVertex3fv( center.v );
				glVertex3fv( (center+size*obj->pNormals[j]).v );
			}
		}
		else
		{
			for( int j=0; j<obj->numOfVerts; j++)
			{
				glVertex3fv( obj->pVerts[j].v );
				glVertex3fv( (obj->pVerts[j]+size*obj->pNormals[j]).v );
			}
		}
		glEnd();
	}
	glColor3ub(255, 255, 255);
	glPopAttrib();
	*/
}

void C3ds::SetConnectivity()
{
	for( int iobj=0; iobj<objects.size(); iobj++)
	{
		for(int iface=0; iface<objects[iobj].numOfFaces; iface++)
		{
			objects[iobj].pFacesShadow[iface].neighbourIndices[0]=-1;
			objects[iobj].pFacesShadow[iface].neighbourIndices[1]=-1;
			objects[iobj].pFacesShadow[iface].neighbourIndices[2]=-1;
		}
	}

	for( iobj=0; iobj<objects.size(); iobj++)
	{
		z3ds_Object *obj = &objects[iobj];

		for(int ifaceA=0; ifaceA<((int)obj->numOfFaces)-1; ifaceA++)
		{
			for(int edgeA=0; edgeA<3; edgeA++)
			{
				if(obj->pFacesShadow[ifaceA].neighbourIndices[edgeA]>-1)continue;

				for(int ifaceB=ifaceA+1; ifaceB<obj->numOfFaces; ifaceB++)
				{
					for(int edgeB=0; edgeB<3; edgeB++)
					{
						unsigned short a1,a2,b1,b2;

						a1 = obj->pFaces[ifaceA].vertIndex[edgeA];
						a2 = obj->pFaces[ifaceA].vertIndex[(edgeA+1)%3];
						b1 = obj->pFaces[ifaceB].vertIndex[edgeB];
						b2 = obj->pFaces[ifaceB].vertIndex[(edgeB+1)%3];

						if( (a1==b1 && a2==b2) || (a1==b2 && a2==b1) )
						{
							obj->pFacesShadow[ifaceA].neighbourIndices[edgeA]=ifaceB;
							obj->pFacesShadow[ifaceB].neighbourIndices[edgeB]=ifaceA;
						}
					}
				}
			}
		}
	}
}

void C3ds::SetVisibility(const vec &lg)
{
	if(!isShadowObject)return;
	for(int iobj=0; iobj<objects.size(); iobj++)
	{
		z3ds_Object *obj = &objects[iobj];
		for(int i=0; i<obj->numOfFaces; i++)
		{
			if( DOT3( lg, obj->pFacesShadow[i].normal) + obj->pFacesShadow[i].delta > 0)
				obj->pFacesShadow[i].visible = 1;
			else 
				obj->pFacesShadow[i].visible = 0;
		}
	}
}

void C3ds::RenderShadowVolumes(const vec &lg, int z_fail)
{
/*	if(!isShadowObject)return;
	for(int iobj=0; iobj<objects.size(); iobj++)
	{
		z3ds_Object *obj = &objects[iobj];

		for( int i=0; i<obj->silhouette.size(); i++)
		{
			glBegin( GL_TRIANGLE_STRIP );
				glVertex4f( obj->pVerts[obj->silhouette[i].b].x-lg.x, obj->pVerts[obj->silhouette[i].b].y-lg.y, obj->pVerts[obj->silhouette[i].b].z-lg.z, 0);
				glVertex3fv( obj->pVerts[obj->silhouette[i].b].v );
				glVertex4f( obj->pVerts[obj->silhouette[i].a].x-lg.x, obj->pVerts[obj->silhouette[i].a].y-lg.y, obj->pVerts[obj->silhouette[i].a].z-lg.z, 0);
				glVertex3fv( obj->pVerts[obj->silhouette[i].a].v );
			glEnd();
		}

		if(z_fail)
		{
			glBegin( GL_TRIANGLES );
			for( i=0; i<obj->numOfFaces; i++)
			{
				if(obj->pFacesShadow[i].visible)
				{
					for( int j=0; j<3; j++)
					{
						glVertex3fv( obj->pVerts[ obj->pFaces[i].vertIndex[j] ].v );
					}
				}
				else
				{
					for( int j=0; j<3; j++)
					{
						glVertex4f( obj->pVerts[ obj->pFaces[i].vertIndex[j] ].x-lg.x, obj->pVerts[ obj->pFaces[i].vertIndex[j] ].y-lg.y, obj->pVerts[ obj->pFaces[i].vertIndex[j] ].z-lg.z, 0);
					}
				}
			}
			glEnd();
		}
*/
	/*	// nepouziva obj->silhouette
		for(int i=0; i<obj->numOfFaces; i++)
		{
			if( obj->pFacesShadow[i].visible )
			{
				for(int j=0; j<3; j++)
				{
					int neighbourIndex = obj->pFacesShadow[i].neighbourIndices[j];
					if( neighbourIndex==-1 || obj->pFacesShadow[neighbourIndex].visible==0 )
					{
						unsigned short a,b;
						a = obj->pFaces[i].vertIndex[j];
						b = obj->pFaces[i].vertIndex[(j+1)%3];
						glBegin( GL_TRIANGLE_STRIP );
							glVertex4f( obj->pVerts[b].x-lg.x, obj->pVerts[b].y-lg.y, obj->pVerts[b].z-lg.z, 0);
							glVertex3fv( obj->pVerts[b].v );
							glVertex4f( obj->pVerts[a].x-lg.x, obj->pVerts[a].y-lg.y, obj->pVerts[a].z-lg.z, 0);
							glVertex3fv( obj->pVerts[a].v );
						glEnd();
					}
				}
			}
		}
		*/
//	}
}

void C3ds::RenderShadowSilhouette()
{
/*	if(!isShadowObject)return;
//	glPolygonOffset( -1.f, -2.f);
//	glEnable( GL_POLYGON_OFFSET_FILL);

	glEnable( GL_LINE_SMOOTH);
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable( GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glBegin( GL_LINES  );
	for(int iobj=0; iobj<objects.size(); iobj++)
	{
		z3ds_Object *obj = &objects[iobj];
		for(int i=0; i<obj->numOfFaces; i++)
		{
			if( obj->pFacesShadow[i].visible )
			{
				for(int j=0; j<3; j++)
				{
					int neighbourIndex = obj->pFacesShadow[i].neighbourIndices[j];
					if( neighbourIndex==-1 || obj->pFacesShadow[neighbourIndex].visible==0 )
					{
						unsigned short a,b;
						a = obj->pFaces[i].vertIndex[j];
						b = obj->pFaces[i].vertIndex[(j+1)%3];
						
						glVertex3fv( obj->pVerts[b].v );
						glVertex3fv( obj->pVerts[a].v );
					}
				}
			}
		}
	}
	glEnd();
	glDisable( GL_BLEND);
	glDisable( GL_LINE_SMOOTH);
//	glDisable( GL_POLYGON_OFFSET_FILL);
*/
}

void C3ds::MakeSilhouette()
{
	z3ds_Line line;
	if(!isShadowObject)return;
	for(int iobj=0; iobj<objects.size(); iobj++)
	{
		z3ds_Object *obj = &objects[iobj];
		obj->silhouette.clear();
		for(int i=0; i<obj->numOfFaces; i++)
		{
			if( obj->pFacesShadow[i].visible )
			{
				for(int j=0; j<3; j++)
				{
					int neighbourIndex = obj->pFacesShadow[i].neighbourIndices[j];
					if( neighbourIndex==-1 || obj->pFacesShadow[neighbourIndex].visible==0 )
					{
						line.a = obj->pFaces[i].vertIndex[j];
						line.b = obj->pFaces[i].vertIndex[(j+1)%3];
						obj->silhouette.push_back( line);
					}
				}
			}
		}
	}
}


void C3ds::RotateX90()
{
	for(int i=0; i<objects.size(); i++)
	{
		z3ds_Object *obj = &objects[i];

		if(obj->pVerts!=NULL)
		{
			for(int j=0; j<obj->numOfVerts; j++)
			{
				float z = -obj->pVerts[j].y;
				obj->pVerts[j].y = obj->pVerts[j].z;
				obj->pVerts[j].z = z;
			}
		}
		if(obj->pNormals!=NULL)
		{
			for(int j=0; j<obj->numOfVerts; j++)
			{
				float z = -obj->pNormals[j].y;
				obj->pNormals[j].y = obj->pNormals[j].z;
				obj->pNormals[j].z = z;
			}
		}

		if(obj->pFacesShadow!=NULL)
		{
			for(int j=0; j<obj->numOfFaces; j++)
			{
				float z = -obj->pFacesShadow[j].normal.y;
				obj->pFacesShadow[j].normal.y = obj->pFacesShadow[j].normal.z;
				obj->pFacesShadow[j].normal.z = z;
			}
		}
	}
}

void C3ds::Rescale(float scale)
{
	for(int i=0; i<objects.size(); i++)
	{
		z3ds_Object *obj = &objects[i];

		if(obj->pVerts!=NULL)
		{
			for(int j=0; j<obj->numOfVerts; j++)obj->pVerts[j]*=scale;
		}
	}
}

int C3ds::CreateDXObjects()
{
	z3ds_DXobj dxobj;
	// Since we know how many objects our model has, go through each of them.
	for(int i = 0; i < objects.size(); i++)
	{
		z3ds_Object *obj = &objects[i];

		if(obj->numOfFaces==0)continue;
		// zistime ci mame nacitane texturove koordinacie
		if(obj->bHasTexture && obj->pTexVerts!=NULL)dxobj.have_texture_coord_0 = 1;
		else dxobj.have_texture_coord_0 = 0;

		dxobj.materialID = obj->materialID;
		dxobj.count = obj->numOfFaces;
		dxobj.pvb = NULL;
		
		if( FAILED( pd->CreateVertexBuffer( dxobj.count*3*sizeof(z3ds_CUSTOMVERTEX),D3DUSAGE_WRITEONLY, 
								D3DFVF_CUSTOMVERTEX_3ds,D3DPOOL_DEFAULT, &dxobj.pvb ) ) )return 0;

		void* p;
		if( FAILED( (dxobj.pvb)->Lock( 0, dxobj.count*3*sizeof(z3ds_CUSTOMVERTEX), (BYTE**)&p, 0 ) ) )return 0;
		
		int i_vec=0;
		// Go through all of the faces (polygons) of the object and draw them
		for(int j = 0; j < obj->numOfFaces; j++)
		{
			// Go through each corner of the triangle and draw it.
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				int index = obj->pFaces[j].vertIndex[whichVertex];

				// normal
				((z3ds_CUSTOMVERTEX*)p)[i_vec].n = obj->pNormals[ index ];
				// texture coordinates
				if(dxobj.have_texture_coord_0)((z3ds_CUSTOMVERTEX*)p)[i_vec].t0 = obj->pTexVerts[ index ];
				((z3ds_CUSTOMVERTEX*)p)[i_vec].t0.y = 1.f -((z3ds_CUSTOMVERTEX*)p)[i_vec].t0.y;
				// vertex
				((z3ds_CUSTOMVERTEX*)p)[i_vec++].v = obj->pVerts[ index ];
			}
		}
		dxobj.pvb->Unlock();

		dxobjs.push_back(dxobj);
		dxobj.pvb = NULL;
	}
	DeleteObjects();
	return 1;
}

void C3ds::DeleteObjects()
{
	for(int i=0; i<this->objects.size(); i++)
	{
		if(objects[i].pFaces!=NULL) delete [] objects[i].pFaces;
		if(objects[i].pFacesShadow!=NULL) delete [] objects[i].pFacesShadow;
		if(objects[i].pNormals!=NULL) delete [] objects[i].pNormals;
		if(objects[i].pTexVerts!=NULL) delete [] objects[i].pTexVerts;
		if(objects[i].pVerts!=NULL) delete [] objects[i].pVerts;
	}
	objects.clear();
}
