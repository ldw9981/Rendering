#include "AsePpl.h"

DWORD AsePpl::dwDecl[] =
{
	D3DVSD_STREAM(0),
	D3DVSD_REG(0, D3DVSDT_FLOAT3),		// the position data
	D3DVSD_REG(1, D3DVSDT_FLOAT3),		// normal
	D3DVSD_REG(2, D3DVSDT_FLOAT2),		// texture coord 0
	D3DVSD_REG(3, D3DVSDT_FLOAT3),		// Tangent
	D3DVSD_REG(4, D3DVSDT_FLOAT3),		// Binormal
	D3DVSD_REG(5, D3DVSDT_FLOAT2),		// light address
	D3DVSD_END()
};

AsePpl::AsePpl()
{
	allocated_size_of_intenzity = 0;
}

AsePpl::~AsePpl()
{
	DeleteAll();
}

int AsePpl::Init( char *file_name, int MipLevels, int rotateX90, float scale, int loadHeightMap)
{
	DeleteAll();
	if(!AseFile::Init( file_name, MipLevels, rotateX90, scale, 1, loadHeightMap))return 0;
	
	if(!vs11_one_light.LoadVS( NULL, dwDecl, vs11_one_light_text))return 0;
	if(!ps11_one_light.LoadPS( NULL, ps11_one_light_text))return 0;
	if(!ps11_one_light_with_ambient.LoadPS( NULL, ps11_one_light_with_ambient_text))return 0;

	if( HIBYTE(LOWORD(caps.PixelShaderVersion))>=2 ||
	   (HIBYTE(LOWORD(caps.PixelShaderVersion))==1 && LOBYTE(LOWORD(caps.PixelShaderVersion))>=4) )
	{
		if(!ps14_one_light.LoadPS( NULL, ps14_one_light_text))return 0;
		if(!ps14_one_light_with_ambient.LoadPS( NULL, ps14_one_light_with_ambient_text))return 0;
	}

	for(int i=0; i<static_lights.size(); i++)
		lights.push_back(static_lights[i]);

	if(!CreateDXObjects())return 0;
	if(!ppl_compute_light_intenzity())return 0;
	if(!ppl_MakeVertexBuffers_ps11(100))return 0;
	return 1;
}

int AsePpl::CreateDXObjects()
{
	zAsePpl_DXobj dxobj;
	for(int i = 0; i < objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];

		if( obj->pTexVerts!=NULL && obj->pFaceCoord!=NULL ) dxobj.have_texture_coord_0=1;
		else dxobj.have_texture_coord_0=0;

		dxobj.materialID = obj->materialID;
		dxobj.count = obj->numOfFaces;
		dxobj.pvb = NULL;
		dxobj.numOfIntensity = 0;
		dxobj.numOfppl = 0;
		dxobj.pIntensity = NULL;
		dxobj.ppl = NULL;
		if(obj->numOfFaces<1)
		{
			dxobjs.push_back(dxobj);
			continue;
		}
		
		if( FAILED( pd->CreateVertexBuffer( dxobj.count*3*sizeof(zAsePpl_CUSTOMVERTEX),D3DUSAGE_WRITEONLY, 
								D3DFVF_CUSTOMVERTEX_ASEPPL,D3DPOOL_DEFAULT, &dxobj.pvb ) ) )return 0;

		zAsePpl_CUSTOMVERTEX* p;
		if( FAILED( (dxobj.pvb)->Lock( 0, dxobj.count*3*sizeof(zAsePpl_CUSTOMVERTEX), (BYTE**)&p, 0 ) ) )return 0;
		
		int i_vec=0;
		for(int j = 0; j < obj->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = obj->pFaces[j].index[whichVertex];

				// normal
				p[i_vec].n = obj->pNormals[ index ];
								
				// texture coordinates
				if(dxobj.have_texture_coord_0)
				{
					p[i_vec].t0.x = obj->pTexVerts[ obj->pFaceCoord[j].index[whichVertex] ].x;
					p[i_vec].t0.y = 1.f - obj->pTexVerts[ obj->pFaceCoord[j].index[whichVertex] ].y;
				}

				// Tangent
				if(obj->pTangent)p[i_vec].s = obj->pTangent[ index ];
				// Binormal
				if(obj->pBinormal)p[i_vec].t = obj->pBinormal[ index ];
				
				// vertex
				p[i_vec++].v = obj->pVerts[ index ];
			}
		}
		dxobj.pvb->Unlock();

		dxobjs.push_back(dxobj);
		dxobj.pvb = NULL;
	}
	return 1;
}

void AsePpl::DeleteAll()
{
	for(int i=0; i<dxobjs.size(); i++)
	{
		if( dxobjs[i].pvb != NULL )dxobjs[i].pvb->Release();
		if( dxobjs[i].ppl != NULL)
		{
			for(int j=0; j<dxobjs[i].numOfppl; j++)
				if( dxobjs[i].ppl[j].pvb != NULL )dxobjs[i].ppl[j].pvb->Release();
			delete [] dxobjs[i].ppl;
		}
		if( dxobjs[i].pIntensity != NULL )
		{
			for(int j=0; j<dxobjs[i].numOfIntensity; j++)
			{
				if(dxobjs[i].pIntensity[j].intensity!=NULL)delete [] dxobjs[i].pIntensity[j].intensity;
				if(dxobjs[i].pIntensity[j].light_index!=NULL)delete [] dxobjs[i].pIntensity[j].light_index;
			}
			delete [] dxobjs[i].pIntensity;
		}
	}
	dxobjs.clear();
	lights.clear();
	allocated_size_of_intenzity = 0;
}

// texturing: 0-on, 1-off
// material
// 0 - don't use materials
// 1 - use materials
void AsePpl::Render(int texturing, int material, DWORD hVertexShader)
{
	D3DMATERIAL8 m_temp;
	if(material)
		pd->GetMaterial( &m_temp );
	if(!visible)return;

	for(int i=0; i<dxobjs.size(); i++)
	{
		zASE_Material *mat=NULL;
		zASE_Material *mat0=NULL;
		zASE_Material *mat1=NULL;
		if(dxobjs[i].pvb==NULL)continue;
		if(!objects[i].visible)continue;

		if( dxobjs[i].materialID>=0 && dxobjs[i].materialID<materials.size() )
			mat = &materials[dxobjs[i].materialID];
		
		if(mat!=NULL)
		{
			if(mat->pSubMaterials!=NULL)
				mat0 = &mat->pSubMaterials[0];
			if(mat->pSubMaterials!=NULL && mat->numOfSubMaterials>1)
				mat1 = &mat->pSubMaterials[1];
			
			if(texturing && dxobjs[i].have_texture_coord_0)
			{
				if(mat0!=NULL && mat0->textureId>=0)
				{
					tDiffuse[mat0->textureId].SetTexture(0);
					pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
					pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
					pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
					pd->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
				}
				else if(mat->textureId>=0)
				{
					tDiffuse[mat->textureId].SetTexture(0);
					pd->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
					pd->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
					pd->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
					pd->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
				}
			}
			else
				pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );

			if(material)
			{
				D3DMATERIAL8 mtrl;
				ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
				mtrl.Ambient  = *(D3DCOLORVALUE*)mat->ambient.v;
				mtrl.Diffuse  = *(D3DCOLORVALUE*)mat->diffuse.v;
				mtrl.Specular = *(D3DCOLORVALUE*)(mat->shineStrength*mat->specular).v;
				mtrl.Emissive = *(D3DCOLORVALUE*)(mat->selfIllum*mat->diffuse).v;
				mtrl.Power    = mat->shine*100.f;
				pd->SetMaterial( &mtrl );
			}
		}
		
		pd->SetStreamSource( 0, dxobjs[i].pvb, sizeof(zAsePpl_CUSTOMVERTEX) );
		pd->SetVertexShader( hVertexShader );

		for(int j=0; j<dxobjs[i].count-21000; j+=21000)
			pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
		pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].count-j );

		pd->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
		pd->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
		pd->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU | 0);
	}
	if(material)
		pd->SetMaterial( &m_temp );
}

int AsePpl::ppl_allocate_intenzity()
{
	int new_size = lights.size() + 3;
	for(int i = 0; i < objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];
		zAsePpl_DXobj *dxobj = &dxobjs[i];
		dxobj->numOfIntensity = obj->numOfFaces;
		if(obj->numOfFaces<=0)continue;
		if(allocated_size_of_intenzity>0 && dxobj->pIntensity!=NULL)
		{
			for(int k=0; k<dxobj->numOfIntensity; k++)
			{
				zAsePpl_intensity inte = dxobj->pIntensity[k];
				zAsePpl_intensity *pint = &dxobj->pIntensity[k];
				pint->intensity = new float[new_size];	if(pint->intensity==NULL)return 0;
				pint->light_index = new int[new_size];	if(pint->light_index==NULL)return 0;
				memcpy(pint->intensity, inte.intensity, sizeof(float)*allocated_size_of_intenzity);
				memcpy(pint->light_index, inte.light_index, sizeof(int)*allocated_size_of_intenzity);
				delete [] inte.intensity;
				delete [] inte.light_index;
				for(int l=allocated_size_of_intenzity; l<new_size; l++)
				{
					pint->intensity[l] = 0.f;
					pint->light_index[l] = l;
				}
			}
		}
		else
		{
			if(dxobj->pIntensity!=NULL)delete [] dxobj->pIntensity;
			dxobj->pIntensity = new zAsePpl_intensity[dxobj->numOfIntensity];
			if(dxobj->pIntensity==NULL)return 0;
			for(int k=0; k<dxobj->numOfIntensity; k++)
			{
				zAsePpl_intensity *pint = &dxobj->pIntensity[k];
				pint->intensity = NULL;
				pint->light_index = NULL;
				pint->intensity = new float[new_size];	if(pint->intensity==NULL)return 0;
				pint->light_index = new int[new_size];	if(pint->light_index==NULL)return 0;
				for(int l=0; l<new_size; l++)
				{
					pint->intensity[l] = 0.f;
					pint->light_index[l] = l;
				}
			}
		}
	}
	for(int k=0; k<lights.size(); k++)
		lights[k].modification = 1;
	allocated_size_of_intenzity = new_size;
	return 1;
}

int AsePpl::ppl_compute_light_intenzity()
{
	int *objlight;
	if(lights.size()<=0)
	{
		z_Light l;
		l.color.set(0,0,0);
		l.light_range = 0.0001f;
		l.lpos.set(1000,1000,1000);
		l.modification=1;
		lights.push_back(l);
	}

	if(allocated_size_of_intenzity<lights.size())
		if(!ppl_allocate_intenzity())return 0;

//	FILE *sub; sub = fopen("sort.txt","wb");
	objlight = new int[lights.size()];

	for(int i = 0; i < objects.size(); i++)		// for all objects
	{
		zASE_Object *obj = &objects[i];
		zAsePpl_DXobj *dxobj = &dxobjs[i];
		if(!obj->visible)continue;
		for(int l=0; l<lights.size(); l++)	// for all lights
			objlight[l] = SphereInBox(lights[l].lpos,lights[l].light_range,obj->min,obj->max);

		for(int j=0; j<obj->numOfFaces; j++)	// for all triangles
		{
			zAsePpl_intensity *pint = &dxobj->pIntensity[j];
			for(int l=0; l<lights.size(); l++)	// for all lights
			{
				pint->light_index[l] = l;
				if(lights[l].modification)		// if change position or light_range
				{
					pint->intensity[l] = 0;
					if(objlight[l])
					{
						if( DOT3( obj->pFaceNormals[j], lights[l].lpos-obj->pVerts[obj->pFaces[j].index[0]]) < 0.f )continue;
						
						for( int vi=0; vi<3; vi++)		// vertex index
						{
							// Lvec = (lpos - vertex)/light_range
							// intenzita += max( 1 - dot(Lvec,Lvec), 0)
							vec Lvec = (lights[l].lpos - obj->pVerts[obj->pFaces[j].index[vi]])/lights[l].light_range;
							pint->intensity[l] += MAX( 1 - DOT3(Lvec,Lvec), 0);
						}
					/*	vec center = obj->pVerts[obj->pFaces[j].index[0]];
						center += obj->pVerts[obj->pFaces[j].index[1]];
						center += obj->pVerts[obj->pFaces[j].index[2]];
						center *= 1/3.f;
						vec Lvec = (lights[l].lpos - center)/lights[l].light_range;
						pint->intensity[l] += MAX( 1 - DOT3(Lvec,Lvec), 0);*/
					}
				}
			}
			// vyplnit pole light_index, s indexmi tak, aby intensity[light_index[i]] bolo usporiadane pole
			// usporiadat pole light_index[i] tak, aby intensity[light_index[i]] bolo usporiadene
			ppl_Quicksort(pint,0,lights.size()-1);
		//	fprintf(sub,"\r\nobject: %d  triangle: %d\r\n",i,j);
		//	for( l=0; l<lights.size(); l++)	// for all lights
		//		fprintf(sub,"%5d, %5d, %f\r\n",l,pint->light_index[l],(double)pint->intensity[pint->light_index[l]]);
		}
	}
	for(int k=0; k<lights.size(); k++)
		lights[k].modification = 0;
//	fclose(sub);
	delete [] objlight;
	return 1;
}

void AsePpl::ppl_Quicksort(zAsePpl_intensity *pint, int l, int r)
{
	int    i,j;							// indexi
	int    x;							// zarazka
	int temp;							// pomocna na vymenu prvkov

	i=l;								// i zacina vlavo
	j=r;								// j zacina vpravo
	x = pint->light_index[ (l+r)/2 ];		// vyber nahodneho prvku, ja vyberam prostredny
	while(i<=j)							// pokial sa nepretnu
	{
		while( pint->intensity[pint->light_index[i]] > pint->intensity[x]) i++;	// hladame prvok, ktory je vacsi alebo rovny ako x
		while( pint->intensity[x] > pint->intensity[pint->light_index[j]]) j--;	// hladame prvok, ktory je mensi alebo rovny ako x
		if(i<=j)						// ak i a j nepresli cez seba ak nie je i>j
		{
			temp = pint->light_index[i];				// vymena prvkov i a j
			pint->light_index[i] = pint->light_index[j];
			pint->light_index[j] = temp;
			i++;						// budeme pokracovat na dalsom prvku
			j--;
		}
	}
	if(l<j)ppl_Quicksort(pint,l,j);			// triedime pokial useky su vacsie ako 1
	if(i<r)ppl_Quicksort(pint,i,r);
}

int AsePpl::ppl_MakeVertexBuffers_ps11(int max_lights)
{
	if(max_lights>lights.size())max_lights = lights.size();
	if(max_lights<1)return 0;
	
	int capacity_vs = (caps.MaxVertexShaderConst-6)/2;		// max count of light in vertex shader constants
	for(int i = 0; i < objects.size(); i++)
	{
		zAsePpl_DXobj *dxobj;
		zASE_Object *obj = &objects[i];
		
		if(i>=dxobjs.size())
		{
			zAsePpl_DXobj dxo;
			dxo.materialID = obj->materialID;
			dxo.count = 0;
			dxo.pvb = NULL;
			dxo.numOfppl = 0;
			dxo.ppl = NULL;
			dxo.numOfIntensity = 0;
			dxo.pIntensity = NULL;
			dxobjs.push_back(dxo);
		}
		dxobj = &dxobjs[i];
		
		if( obj->pTexVerts!=NULL && obj->pFaceCoord!=NULL ) dxobj->have_texture_coord_0=1;
		else dxobj->have_texture_coord_0=0;

		if(!obj->visible)continue;
		if(obj->numOfFaces<1)continue;

		if( dxobj->numOfppl<max_lights || dxobj->ppl==NULL)
		{
			if(dxobj->numOfppl>0 && dxobj->ppl!=NULL)
			{
				int k;
				zAsePpl_DXobj_ppl *ppl_old = dxobj->ppl;
				int numOfppl_old = dxobj->numOfppl;
				dxobj->numOfppl = max_lights;
				dxobj->ppl = new zAsePpl_DXobj_ppl[max_lights];
				if(dxobj->ppl==NULL){ delete [] ppl_old; return 0;}
				for( k=0; k<numOfppl_old; k++)
				{
					dxobj->ppl[k].count = ppl_old[k].count;
					dxobj->ppl[k].pvb = ppl_old[k].pvb;
				}
				for( ; k<dxobj->numOfppl; k++)
				{
					dxobj->ppl[k].count = 0;
					dxobj->ppl[k].render_count = 0;
					dxobj->ppl[k].pvb = NULL;
				}
				delete [] ppl_old;
			}
			else
			{
				dxobj->numOfppl = max_lights;
				dxobj->ppl = new zAsePpl_DXobj_ppl[max_lights];
				if(dxobj->ppl==NULL)return 0;
				for(int k=0; k<dxobj->numOfppl; k++)
				{
					dxobj->ppl[k].count = 0;
					dxobj->ppl[k].render_count = 0;
					dxobj->ppl[k].pvb = NULL;
				}
			}
		}

		for(int l=0; l<max_lights; l++)
		{
			zAsePpl_DXobj_ppl *ppl = &dxobj->ppl[l];
			int k;

			ppl->render_count = 0;
			for( k=0; k<obj->numOfFaces; k++)		// count of triangles
			{
				if(l==0 || dxobj->pIntensity[k].intensity[dxobj->pIntensity[k].light_index[l]]>0.f)
					ppl->render_count++;
			}
			if( ppl->render_count<1 )continue;
			
			if( ppl->count<ppl->render_count || ppl->pvb==NULL)
			{
				if(ppl->pvb!=NULL)ppl->pvb->Release();
				ppl->count = ppl->render_count;
				if( FAILED( pd->CreateVertexBuffer( ppl->count*3*sizeof(zAsePpl_CUSTOMVERTEX),
								D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 
								D3DFVF_CUSTOMVERTEX_ASEPPL,D3DPOOL_DEFAULT, &ppl->pvb ) ) )
				{
					ppl->pvb = NULL;
					return 0;
				}
			}

			zAsePpl_CUSTOMVERTEX* p;
			if( FAILED( (ppl->pvb)->Lock( 0, ppl->count*3*sizeof(zAsePpl_CUSTOMVERTEX), (BYTE**)&p, D3DLOCK_DISCARD ) ) )return 0;
			
			int i_vec=0;
			for( k=0; k<obj->numOfFaces; k++)
			{
				if(l!=0 && dxobj->pIntensity[k].intensity[dxobj->pIntensity[k].light_index[l]]<=0.f)
					continue;
				if(dxobj->pIntensity[k].light_index[l]>=capacity_vs)
				{
					ppl->render_count--;
					continue;
				}

				for(int vi = 0; vi < 3; vi++)		// vi - vertex index
				{
					int index = obj->pFaces[k].index[vi];

					// normal
					p[i_vec].n = obj->pNormals[ index ];
					
					// texture coordinates
					if(dxobj->have_texture_coord_0)
					{
						p[i_vec].t0.x = obj->pTexVerts[ obj->pFaceCoord[k].index[vi] ].x;
						p[i_vec].t0.y = 1.f - obj->pTexVerts[ obj->pFaceCoord[k].index[vi] ].y;
					}

					// Tangent
					if(obj->pTangent)p[i_vec].s = obj->pTangent[ index ];
					// Binormal
					if(obj->pBinormal)p[i_vec].t = obj->pBinormal[ index ];
					
					if(dxobj->pIntensity[k].intensity[dxobj->pIntensity[k].light_index[l]]>0.f)
						p[i_vec].ladr.x = (float)dxobj->pIntensity[k].light_index[l]*2.f+0.1f+6.f;
					else
						p[i_vec].ladr.x = (float)capacity_vs*2.f+0.1f+6.f;		// black light
					p[i_vec].ladr.y = 0;		// no use with ps 1.1
					// vertex
					p[i_vec++].v = obj->pVerts[ index ];
				}
			}
			ppl->pvb->Unlock();
		}
	}
	return 1;
}

// per object
void AsePpl::ppl_Render(int max_lights, DWORD hPixelShader_ambient, DWORD hPixelShader, int heightMap, int two_lights_per_pass)
{
	int step=1;
	if(two_lights_per_pass)step=2;
	if(!visible)return;
	if(heightMap && tHeight==NULL)return;
	int capacity_vs = (caps.MaxVertexShaderConst-6)/2-1;		// max count of light in vertex shader constants

	for(int l=0; l<lights.size() && l<capacity_vs; l++)
	{
		pd->SetVertexShaderConstant(  6+2*l+0, vec4(lights[l].lpos,1.f/lights[l].light_range).v, 1 );
		pd->SetVertexShaderConstant(  6+2*l+1, vec4(lights[l].color).v, 1 );
	}
	// black light
	pd->SetVertexShaderConstant(  6+2*capacity_vs+0, vec4(1000,1000,1000,0.001f).v, 1 );
	pd->SetVertexShaderConstant(  6+2*capacity_vs+1, vec4(0,0,0,0).v, 1 );

	for(int i=0; i<dxobjs.size(); i++)
	{
		int l;
		zASE_Material *mat=NULL;

		if(dxobjs[i].ppl==NULL || dxobjs[i].numOfppl<1)continue;
		if(!objects[i].visible)continue;

		if( dxobjs[i].materialID>=0 && dxobjs[i].materialID<materials.size() )
			mat = &materials[dxobjs[i].materialID];

		if(mat->textureId<0 && mat->numOfSubMaterials>0)
			mat = &mat->pSubMaterials[0];
		
		if(mat!=NULL && mat->textureId>=0 && dxobjs[i].have_texture_coord_0)
		{
			tDiffuse[mat->textureId].SetTexture(0);
			tNormal[mat->textureId].SetTexture(1);
			if(heightMap)
				tHeight[mat->textureId].SetTexture(2);
		}
		else
			continue;

		l = 0;
		if(l>=lights.size() || l>=max_lights)continue;
		if(dxobjs[i].ppl[l].render_count<1)continue;
		pd->SetPixelShader( hPixelShader_ambient );
		pd->SetStreamSource( 0, dxobjs[i].ppl[l].pvb, sizeof(zAsePpl_CUSTOMVERTEX) );
		for(int j=0; j<dxobjs[i].ppl[l].render_count-21000; j+=21000)
			pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
		pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].ppl[l].render_count-j );

		pd->SetPixelShader( hPixelShader );
		pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 1 );
		pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		pd->SetRenderState( D3DRS_ZWRITEENABLE, 0 );
		pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		
		for(l+=step; l<dxobjs[i].numOfppl && l<max_lights; l+=step)
		{
			if(dxobjs[i].ppl[l].render_count<1)break;
		//	pd->SetRenderState( D3DRS_ZBIAS, l);
			
			pd->SetStreamSource( 0, dxobjs[i].ppl[l].pvb, sizeof(zAsePpl_CUSTOMVERTEX) );
			
			for(int j=0; j<dxobjs[i].ppl[l].render_count-21000; j+=21000)
				pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
			pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].ppl[l].render_count-j );
		}

		pd->SetRenderState( D3DRS_ZBIAS, 0);
		pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS);
		pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
		pd->SetRenderState( D3DRS_ZWRITEENABLE, 1 );
	}
}

// per lights
void AsePpl::ppl_Render_(int max_lights, DWORD hPixelShader_ambient, DWORD hPixelShader, int heightMap, int two_lights_per_pass)
{
	int l;
	int step=1;
	if(two_lights_per_pass)step=2;
	int capacity_vs = (caps.MaxVertexShaderConst-6)/2-1;		// max count of light in vertex shader constants
	if(heightMap && tHeight==NULL)return;
	if(!visible)return;
	for( l=0; l<lights.size() && l<capacity_vs; l++)
	{
		pd->SetVertexShaderConstant(  6+2*l+0, vec4(lights[l].lpos,1.f/lights[l].light_range).v, 1 );
		pd->SetVertexShaderConstant(  6+2*l+1, vec4(lights[l].color).v, 1 );
	}
	// black light
	pd->SetVertexShaderConstant(  6+2*capacity_vs+0, vec4(1000,1000,1000,0.001f).v, 1 );
	pd->SetVertexShaderConstant(  6+2*capacity_vs+1, vec4(0,0,0,0).v, 1 );

	l = 0;
	pd->SetPixelShader( hPixelShader_ambient );
	for(int i=0; i<dxobjs.size(); i++)
	{
		zASE_Material *mat=NULL;

		if(dxobjs[i].ppl==NULL || dxobjs[i].numOfppl<1)continue;
		if(!objects[i].visible)continue;

		if( dxobjs[i].materialID>=0 && dxobjs[i].materialID<materials.size() )
			mat = &materials[dxobjs[i].materialID];

		if(mat->textureId<0 && mat->numOfSubMaterials>0)
			mat = &mat->pSubMaterials[0];
		
		if(mat!=NULL && mat->textureId>=0 && dxobjs[i].have_texture_coord_0)
		{
			tDiffuse[mat->textureId].SetTexture(0);
			tNormal[mat->textureId].SetTexture(1);
			if(heightMap)
				tHeight[mat->textureId].SetTexture(2);
		}
		else
			continue;

		if(dxobjs[i].ppl[l].render_count<1)continue;
		pd->SetStreamSource( 0, dxobjs[i].ppl[l].pvb, sizeof(zAsePpl_CUSTOMVERTEX) );
		for(int j=0; j<dxobjs[i].ppl[l].render_count-21000; j+=21000)
			pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
		pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].ppl[l].render_count-j );
	}
	pd->SetPixelShader( hPixelShader );
		
	pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 1 );
	pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	pd->SetRenderState( D3DRS_ZWRITEENABLE, 0 );
	pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	
	for(l+=step; l<lights.size() && l<max_lights; l+=step)
	{
	//	pd->SetRenderState( D3DRS_ZBIAS, l);
		for(int i=0; i<dxobjs.size(); i++)
		{
			zASE_Material *mat=NULL;

			if(l>=dxobjs[i].numOfppl)continue;
			if(dxobjs[i].ppl[l].render_count<1)continue;
			if(dxobjs[i].ppl==NULL || dxobjs[i].numOfppl<1)continue;
			if(!objects[i].visible)continue;

			if( dxobjs[i].materialID>=0 && dxobjs[i].materialID<materials.size() )
				mat = &materials[dxobjs[i].materialID];

			if(mat->textureId<0 && mat->numOfSubMaterials>0)
				mat = &mat->pSubMaterials[0];
			
			if(mat!=NULL && mat->textureId>=0 && dxobjs[i].have_texture_coord_0)
			{
				tDiffuse[mat->textureId].SetTexture(0);
				tNormal[mat->textureId].SetTexture(1);
				if(heightMap)
					tHeight[mat->textureId].SetTexture(2);
			}
			else
				continue;

			pd->SetStreamSource( 0, dxobjs[i].ppl[l].pvb, sizeof(zAsePpl_CUSTOMVERTEX) );
			for(int j=0; j<dxobjs[i].ppl[l].render_count-21000; j+=21000)
				pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
			pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].ppl[l].render_count-j );
		}
	}

	pd->SetRenderState( D3DRS_ZBIAS, 0);
	pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS);
	pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
	pd->SetRenderState( D3DRS_ZWRITEENABLE, 1 );
}

int AsePpl::ppl_MakeVertexBuffers_ps14(int max_lights)
{
	if(max_lights>lights.size())max_lights = lights.size();
	if(max_lights<1)return 0;
	int capacity_vs = (caps.MaxVertexShaderConst-6)/2-1;	// max count of light in vertex shader constants
	for(int i = 0; i < objects.size(); i++)
	{
		zAsePpl_DXobj *dxobj;
		zASE_Object *obj = &objects[i];
		
		if(i>=dxobjs.size())
		{
			zAsePpl_DXobj dxo;
			dxo.materialID = obj->materialID;
			dxo.count = 0;
			dxo.pvb = NULL;
			dxo.numOfppl = 0;
			dxo.ppl = NULL;
			dxo.numOfIntensity = 0;
			dxo.pIntensity = NULL;
			dxobjs.push_back(dxo);
		}
		dxobj = &dxobjs[i];

		if( obj->pTexVerts!=NULL && obj->pFaceCoord!=NULL ) dxobj->have_texture_coord_0=1;
		else dxobj->have_texture_coord_0=0;

		if(!obj->visible)continue;
		if(obj->numOfFaces<1)continue;

		if( dxobj->numOfppl<max_lights || dxobj->ppl==NULL)
		{
			if(dxobj->numOfppl>0 && dxobj->ppl!=NULL)
			{
				int k;
				zAsePpl_DXobj_ppl *ppl_old = dxobj->ppl;
				int numOfppl_old = dxobj->numOfppl;
				dxobj->numOfppl = max_lights;
				dxobj->ppl = new zAsePpl_DXobj_ppl[max_lights];
				if(dxobj->ppl==NULL){ delete [] ppl_old; return 0;}
				for( k=0; k<numOfppl_old; k++)
				{
					dxobj->ppl[k].count = ppl_old[k].count;
					dxobj->ppl[k].pvb = ppl_old[k].pvb;
				}
				for( ; k<dxobj->numOfppl; k++)
				{
					dxobj->ppl[k].count = 0;
					dxobj->ppl[k].render_count = 0;
					dxobj->ppl[k].pvb = NULL;
				}
				delete [] ppl_old;
			}
			else
			{
				dxobj->numOfppl = max_lights;
				dxobj->ppl = new zAsePpl_DXobj_ppl[max_lights];
				if(dxobj->ppl==NULL)return 0;
				for(int k=0; k<dxobj->numOfppl; k++)
				{
					dxobj->ppl[k].count = 0;
					dxobj->ppl[k].render_count = 0;
					dxobj->ppl[k].pvb = NULL;
				}
			}
		}

		for(int l=0; l<max_lights; l+=2)
		{
			int k;

			dxobj->ppl[l].render_count = 0;		
			if(l+1<max_lights)
				dxobj->ppl[l+1].render_count = 0;
			for( k=0; k<obj->numOfFaces; k++)		// count of triangles
			{
				if(l==0 || dxobj->pIntensity[k].intensity[dxobj->pIntensity[k].light_index[l]]>0.f )
					dxobj->ppl[l].render_count++;
			}
			if( dxobj->ppl[l].render_count<1 )continue;
			
			if( dxobj->ppl[l].count<dxobj->ppl[l].render_count || dxobj->ppl[l].pvb==NULL)
			{
				if(dxobj->ppl[l].pvb!=NULL)dxobj->ppl[l].pvb->Release();
				dxobj->ppl[l].count = dxobj->ppl[l].render_count;
				if( FAILED( pd->CreateVertexBuffer( dxobj->ppl[l].count*3*sizeof(zAsePpl_CUSTOMVERTEX),
								D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, 
								D3DFVF_CUSTOMVERTEX_ASEPPL,D3DPOOL_DEFAULT, &dxobj->ppl[l].pvb ) ) )
				{
					dxobj->ppl[l].pvb = NULL;
					return 0;
				}
			}

			zAsePpl_CUSTOMVERTEX* p;
			if( FAILED( (dxobj->ppl[l].pvb)->Lock( 0, dxobj->ppl[l].count*3*sizeof(zAsePpl_CUSTOMVERTEX), (BYTE**)&p, D3DLOCK_DISCARD ) ) )return 0;
			
			int i_vec=0;
			for( k=0; k<obj->numOfFaces; k++)
			{
				if(l!=0 && dxobj->pIntensity[k].intensity[dxobj->pIntensity[k].light_index[l]]<=0.f )
					continue;
				if(dxobj->pIntensity[k].light_index[l]>=capacity_vs )
				{
					dxobj->ppl[l].render_count--;
					continue;
				}
				if(l+1<max_lights)
				{
					if(dxobj->pIntensity[k].light_index[l+1]>=capacity_vs )
					{
						dxobj->ppl[l].render_count--;
						continue;
					}
				}

				for(int vi = 0; vi < 3; vi++)		// vi - vertex index
				{
					int index = obj->pFaces[k].index[vi];

					// normal
					p[i_vec].n = obj->pNormals[ index ];
					
					// texture coordinates
					if(dxobj->have_texture_coord_0)
					{
						p[i_vec].t0.x = obj->pTexVerts[ obj->pFaceCoord[k].index[vi] ].x;
						p[i_vec].t0.y = 1.f - obj->pTexVerts[ obj->pFaceCoord[k].index[vi] ].y;
					}

					// Tangent
					if(obj->pTangent)p[i_vec].s = obj->pTangent[ index ];
					// Binormal
					if(obj->pBinormal)p[i_vec].t = obj->pBinormal[ index ];
					
					if(dxobj->pIntensity[k].intensity[dxobj->pIntensity[k].light_index[l]]>0.f)
						p[i_vec].ladr.x = (float)dxobj->pIntensity[k].light_index[l]*2.f+0.1f+6.f;
					else
						p[i_vec].ladr.x = (float)capacity_vs*2.f+0.1f+6.f;		// black light

					if(l+1<max_lights && dxobj->pIntensity[k].intensity[dxobj->pIntensity[k].light_index[l+1]]>0.f)
						p[i_vec].ladr.y = (float)dxobj->pIntensity[k].light_index[l+1]*2.f+0.1f+6.f;
					else
						p[i_vec].ladr.y = (float)capacity_vs*2.f+0.1f+6.f;		// black light
					// vertex
					p[i_vec++].v = obj->pVerts[ index ];
				}
			}
			dxobj->ppl[l].pvb->Unlock();
		}
	}
	return 1;
}

void AsePpl::ppl_Render_ps11(int max_lights)
{
	vs11_one_light.SetVertexShader();
	ppl_Render_(max_lights, ps11_one_light_with_ambient.hPixelShader, ps11_one_light.hPixelShader, 0);	    
}

void AsePpl::ppl_Render_ps14(int max_lights)
{
	vs11_one_light.SetVertexShader();
	ppl_Render_(max_lights, ps14_one_light_with_ambient.hPixelShader, ps14_one_light.hPixelShader, 0);	    
}

char AsePpl::ps11_one_light_text[]="\
ps.1.1 \r\n\
; att*( Dif*N.L*BASE + Spec*(N.H)^16 ) \r\n\
; t0 - base \r\n\
; t1 - normal map \r\n\
; t2 - H with cube map normalizer \r\n\
; t3 - L for attenuation \r\n\
; v0 - L \r\n\
; v1 - diffuse and specular color \r\n\
tex       t0             ; base \r\n\
tex       t1             ; normal map \r\n\
tex       t2             ; Cubic Normalized H vector \r\n\
texcoord  t3             ; L for attenuation \r\n\
dp3_sat  r0, t1_bx2, t2_bx2     ; N.H \r\n\
dp3_sat  r1.rgb, t1_bx2, v0_bx2 ; N.L \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^2 \r\n\
mul      r0.rgb, v1, r1         ; D*N.L \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^4 \r\n\
mul      r0.rgb, r0, t0         ; D*N.L*Base \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^8 \r\n\
dp3_sat  r1.rgb, t3_bx2, t3_bx2 ; (Light Space Distance)^2 \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^16 \r\n\
mad      r0.rgb, v1, r0.a, r0   ; S*N.H^16 + D*N.L*Base \r\n\
mul      r0.rgb, 1-r1, r0       ; att*(D*N.L*Base + S*N.H^16) \r\n";
char AsePpl::ps11_one_light_with_ambient_text[]="\
ps.1.1 \r\n\
; att*( Dif*N.L*BASE + Spec*(N.H)^16 ) \r\n\
; t0 - base \r\n\
; t1 - normal map \r\n\
; t2 - H with cube map normalizer \r\n\
; t3 - L for attenuation \r\n\
; v0 - L \r\n\
; v1 - diffuse and specular color \r\n\
; c0 - ambient color \r\n\
 \r\n\
tex       t0             ; base \r\n\
tex       t1             ; normal map \r\n\
tex       t2             ; Cubic Normalized H vector \r\n\
texcoord  t3             ; L for attenuation \r\n\
dp3_sat  r0, t1_bx2, t2_bx2     ; N.H \r\n\
dp3_sat  r1.rgb, t1_bx2, v0_bx2 ; N.L \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^2 \r\n\
mul      r0.rgb, v1, r1         ; D*N.L \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^4 \r\n\
mul      r0.rgb, r0, t0         ; D*N.L*Base \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^8 \r\n\
dp3_sat  r1.rgb, t3_bx2, t3_bx2 ; (Light Space Distance)^2 \r\n\
+mul     r0.a, r0.a, r0.a       ; N.H^16 \r\n\
mad      r0.rgb, v1, r0.a, r0   ; S*N.H^16 + D*N.L*Base \r\n\
+mov     r1.a, 1-r1.b           ; 1 - (Light Space Distance)^2 \r\n\
mul      r1.rgb, t0, c0         ; A*Base \r\n\
mad      r0.rgb, r1.a, r0, r1   ; att*(D*N.L*Base + S*N.H^16) + A*Base";
char AsePpl::ps14_one_light_text[]="\
ps.1.4 \r\n\
; att*( Dif*N.L*BASE + Spec*(N.H)^16 ) \r\n\
; t0 - base \r\n\
; t1 - normal map \r\n\
; t2 - H with cube map normalizer \r\n\
; t3 - L for attenuation \r\n\
; v0 - L \r\n\
; v1 - diffuse and specular color \r\n\
texld    r0, t0                 ; base \r\n\
texld    r1, t1                 ; normal map \r\n\
texld    r2, t2                 ; Cubic Normalized H vector \r\n\
texcrd   r3.rgb, t3             ; L for attenuation \r\n\
texld    r4, t4                 ; Cubic Normalized L vector \r\n\
dp3_sat  r2, r1_bx2, r2_bx2     ; N.H \r\n\
dp3_sat  r1.rgb, r1_bx2, r4_bx2 ; N.L \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^2 \r\n\
mul      r1.rgb, v1, r1         ; D*N.L \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^4 \r\n\
mul      r0.rgb, r1, r0         ; D*N.L*Base \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^8 \r\n\
dp3_sat  r1.rgb, r3_bx2, r3_bx2 ; (Light Space Distance)^2 \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^16 \r\n\
mad      r0.rgb, v1, r2.a, r0   ; S*N.H^16 + D*N.L*Base \r\n\
mul      r0.rgb, 1-r1, r0       ; att*(D*N.L*Base + S*N.H^16)";
char AsePpl::ps14_one_light_with_ambient_text[]="\
ps.1.4 \r\n\
; Amb*Base + att*( Dif*N.L*BASE + Spec*(N.H)^16 ) \r\n\
; t0 - base \r\n\
; t1 - normal map \r\n\
; t2 - H with cube map normalizer \r\n\
; t3 - L for attenuation \r\n\
; v0 - L \r\n\
; v1 - diffuse and specular color \r\n\
; c0 - ambient color \r\n\
 \r\n\
texld    r0, t0                 ; base \r\n\
texld    r1, t1                 ; normal map \r\n\
texld    r2, t2                 ; Cubic Normalized H vector \r\n\
texcrd   r3.rgb, t3             ; L for attenuation \r\n\
texld    r4, t4                 ; Cubic Normalized L vector \r\n\
dp3_sat  r2, r1_bx2, r2_bx2     ; N.H \r\n\
dp3_sat  r1.rgb, r1_bx2, r4_bx2 ; N.L \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^2 \r\n\
mul      r1.rgb, v1, r1         ; D*N.L \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^4 \r\n\
mul      r2.rgb, r1, r0         ; D*N.L*Base \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^8 \r\n\
dp3_sat  r1.rgb, r3_bx2, r3_bx2 ; (Light Space Distance)^2 \r\n\
+mul     r2.a, r2.a, r2.a       ; N.H^16 \r\n\
mad      r2.rgb, v1, r2.a, r2   ; S*N.H^16 + D*N.L*Base \r\n\
mul      r2.rgb, 1-r1, r2       ; att*(D*N.L*Base + S*N.H^16) \r\n\
mad      r0.rgb, r0, c0, r2     ; Amb*Base + att*(D*N.L*Base + S*N.H^16)";
char AsePpl::vs11_one_light_text[]="\
vs.1.1                  // version instruction \r\n\
m4x4 oPos, v0, c0       // transform vertices by view/projection matrix \r\n\
// v2 texture coordination \r\n\
mov  oT0.xy, v2         // base \r\n\
mov  oT1.xy, v2         // normal \r\n\
// c[a0.x+0].xyz = light position in object space \r\n\
// c[a0.x+0].w = 1.f/light range \r\n\
// c[a0.x+1] = light color \r\n\
mov a0.x, v5.x \r\n\
// Light Direction \r\n\
add  r0, c[a0.x+0], -v0  // L = lpos - vertex, L in object space \r\n\
dp3  r1.x, v3, r0    // v3 Tangent,  x \r\n\
dp3  r1.y, v4, r0    // v4 Binormal, y \r\n\
dp3  r1.z, v1, r0    // v1 normal,   z \r\n\
// normalize vector in r1 \r\n\
dp3  r1.w, r1, r1       // r1.w = |r1|^2 \r\n\
rsq  r1.w, r1.w         // r1.w = 1/sqrt(r0.w) = 1/|r1| \r\n\
mul  r2.xyz, r1, r1.w   // r1 - normalized L vector in texture space \r\n\
mad  r1.xyz, r2, c5, c5 // [-1,1] => [0,1] \r\n\
mov  oD0.xyz, r1        // Tangent Space Light Direction \r\n\
mov  oT4.xyz, r2        // Tangent Space Light Direction \r\n\
// Halfangle vector \r\n\
// c[a0.x+0] lpos = r2 \r\n\
// c4 eye \r\n\
// H = HalfVector = Normalize( VertexToEyeVector + LightVector ) = Norm( V + L ) \r\n\
add  r0, c4, -v0        // V = VertexToEyeVector = eye - vertex \r\n\
// V to Tangent Space \r\n\
dp3  r1.x, v3, r0 \r\n\
dp3  r1.y, v4, r0 \r\n\
dp3  r1.z, v1, r0 \r\n\
// normalize vector in r1 \r\n\
dp3  r1.w, r1, r1       // r1.w = |r1|^2 \r\n\
rsq  r1.w, r1.w         // r1.w = 1/sqrt(r0.w) = 1/|r1| \r\n\
mul  r1.xyz, r1, r1.w   // r1 - normalized V vector in Tangent Space \r\n\
add  r1.xyz, r1, r2     // HalfVector = V + L, in Tangent Space \r\n\
mov  oT2.xyz, r1        // Cubic Normalized Tangent Space Halfangle vector \r\n\
//attenuation \r\n\
add  r0, c[a0.x+0], -v0    // L = lpos - vertex, L in object space \r\n\
mul  r0, r0, c[a0.x+0].w   // L/light_range \r\n\
mad  r1.xyz, r0, c5, c5    // [-1,1] => [0,1] \r\n\
mov  oT3.xyz, r1 \r\n\
mov  oD1.xyz, c[a0.x+1]    // light color";
