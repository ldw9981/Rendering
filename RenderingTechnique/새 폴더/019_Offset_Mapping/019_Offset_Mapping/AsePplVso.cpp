#include "AsePplVso.h"

DWORD AsePplVso::dwDecl[] =
{
    D3DVSD_STREAM(0),
    D3DVSD_REG(0, D3DVSDT_FLOAT3),      // the position data
    D3DVSD_REG(1, D3DVSDT_FLOAT3),		// normal
	D3DVSD_REG(2, D3DVSDT_FLOAT2),		// texture coord 0
	D3DVSD_REG(3, D3DVSDT_FLOAT3),		// Tangent
	D3DVSD_REG(4, D3DVSDT_FLOAT3),		// Binormal
	D3DVSD_REG(5, D3DVSDT_FLOAT4),		// center and radius of triangle
    D3DVSD_END()
};

AsePplVso::AsePplVso()
{

}

AsePplVso::~AsePplVso()
{
	DeleteAll();
}

int AsePplVso::Init( char *file_name, int MipLevels, int rotateX90, float scale, int loadHeightMap)
{
	DeleteAll();
	if(!AseFile::Init( file_name, MipLevels, rotateX90, scale, 1, loadHeightMap))return 0;
	
	for(int i=0; i<static_lights.size(); i++)
		lights.push_back(static_lights[i]);

	return CreateDXObjects();
}

int AsePplVso::CreateDXObjects()
{
	zAsePplVso_DXobj dxobj;

	for(int i = 0; i < objects.size(); i++)
	{
		zASE_Object *obj = &objects[i];

		// zistime ci mame nacitane texturove koordinacie
		if( obj->pTexVerts!=NULL && obj->pFaceCoord!=NULL ) dxobj.have_texture_coord_0=1;
		else dxobj.have_texture_coord_0=0;

		dxobj.materialID = obj->materialID;
		dxobj.count = obj->numOfFaces;
		dxobj.pvb = NULL;
		if(obj->numOfFaces<1)
		{
			dxobjs.push_back(dxobj);
			continue;
		}
		
		if( FAILED( pd->CreateVertexBuffer( dxobj.count*3*sizeof(zAsePplVso_CUSTOMVERTEX),D3DUSAGE_WRITEONLY, 
								D3DFVF_CUSTOMVERTEX_ASEPPLVSO,D3DPOOL_DEFAULT, &dxobj.pvb ) ) )return 0;

		zAsePplVso_CUSTOMVERTEX* p;
		if( FAILED( (dxobj.pvb)->Lock( 0, dxobj.count*3*sizeof(zAsePplVso_CUSTOMVERTEX), (BYTE**)&p, 0 ) ) )return 0;
		
		int i_vec=0;

		for(int j = 0; j < obj->numOfFaces; j++)
		{
			// for optimalization with vertes shader store center and radius of the triangle
			vec4 ct;		// centre of triangle
			ct.xyz  = obj->pVerts[obj->pFaces[j].index[0]];
			ct.xyz += obj->pVerts[obj->pFaces[j].index[1]];
			ct.xyz += obj->pVerts[obj->pFaces[j].index[2]];
			ct.xyz *= 1.f/3.f;
			ct.w = Distance2(ct.xyz, obj->pVerts[obj->pFaces[j].index[0]]);
			ct.w = MAX(ct.w, Distance2(ct.xyz, obj->pVerts[obj->pFaces[j].index[1]]));
			ct.w = MAX(ct.w, Distance2(ct.xyz, obj->pVerts[obj->pFaces[j].index[2]]));
		//	ct.w = (float)sqrt(ct.w);
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
				
				p[i_vec].ct = ct;
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

void AsePplVso::DeleteAll()
{
	for(int i=0; i<dxobjs.size(); i++)
		if( dxobjs[i].pvb != NULL )dxobjs[i].pvb->Release();
	dxobjs.clear();
	lights.clear();
}

// texturing: 0-on, 1-off
// material
// 0 - don't use materials
// 1 - use materials
void AsePplVso::Render(int texturing, int material, DWORD hVertexShader)
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
		
		pd->SetStreamSource( 0, dxobjs[i].pvb, sizeof(zAsePplVso_CUSTOMVERTEX) );
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

void AsePplVso::Render_pixel_shader(int heightMap)
{
	if(heightMap && tHeight==NULL)return;
	if(!visible)return;

	for(int i=0; i<dxobjs.size(); i++)
	{
		zASE_Material *mat=NULL;
		if(dxobjs[i].pvb==NULL)continue;
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
		pd->SetStreamSource( 0, dxobjs[i].pvb, sizeof(zAsePplVso_CUSTOMVERTEX) );
		
		for(int j=0; j<dxobjs[i].count-21000; j+=21000)
			pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
		pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].count-j );
	}
}

void AsePplVso::ppl_Render(DWORD hVertexShader_ambient, DWORD hPixelShader_ambient, DWORD hVertexShader, DWORD hPixelShader, int heightMap, const vec &position, int max_lights)
{
	int *light_intex;
	int i=0;
	if(heightMap && tHeight==NULL)return;
	if(!visible)return;
	if(lights.size()<=0)
	{
		z_Light l;
		l.color.set(0,0,0);
		l.light_range = 0.0001f;
		l.lpos.set(1000,1000,1000);
		l.modification=1;
		lights.push_back(l);
	}
	light_intex = new int[lights.size()];
	for( i=0; i<lights.size(); i++)
		light_intex[i]=i;
	ppl_Quicksort(position,light_intex,0,lights.size()-1);

	i=0;
	{	
		float light_range2 = lights[light_intex[i]].light_range;
		light_range2 *= light_range2;

		pd->SetPixelShaderConstant( 1, vec4(lights[light_intex[i]].color).v, 1);
		pd->SetPixelShaderConstant( 2, vec4(lights[light_intex[i]].color).v, 1);
		pd->SetVertexShaderConstant( 6, vec4(lights[light_intex[i]].lpos,1.f/lights[light_intex[i]].light_range).v, 1 );
		pd->SetVertexShaderConstant( 7, vec4( 1,-1, 1, light_range2).v, 1 );
	}
	pd->SetPixelShader( hPixelShader_ambient);
	pd->SetVertexShader( hVertexShader_ambient );
	Render_pixel_shader(heightMap);

	pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 1 );
	pd->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	pd->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	pd->SetRenderState( D3DRS_ZWRITEENABLE, 0 );
	pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pd->SetPixelShader( hPixelShader);
	pd->SetVertexShader( hVertexShader );
	for( i=1; i<lights.size() && i<max_lights; i++)
	{
		ppl_Render_light(light_intex[i],heightMap);
	}
	pd->SetRenderState( D3DRS_ZBIAS, 0);
	pd->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS);
	pd->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
	pd->SetRenderState( D3DRS_ZWRITEENABLE, 1 );
	delete [] light_intex;
}

void AsePplVso::ppl_Quicksort(const vec &position, int *light_index, int l, int r)
{
	int    i,j;							// indexi
	int    x;							// zarazka
	int temp;							// pomocna na vymenu prvkov

	i=l;								// i zacina vlavo
	j=r;								// j zacina vpravo
	x = light_index[ (l+r)/2 ];		// vyber nahodneho prvku, ja vyberam prostredny
	while(i<=j)							// pokial sa nepretnu
	{
		while( Distance2(position, lights[light_index[i]].lpos) < Distance2(position, lights[x].lpos)) i++;	// hladame prvok, ktory je vacsi alebo rovny ako x
		while( Distance2(position, lights[x].lpos) < Distance2(position, lights[light_index[j]].lpos)) j--;	// hladame prvok, ktory je mensi alebo rovny ako x
		if(i<=j)						// ak i a j nepresli cez seba ak nie je i>j
		{
			temp = light_index[i];				// vymena prvkov i a j
			light_index[i] = light_index[j];
			light_index[j] = temp;
			i++;						// budeme pokracovat na dalsom prvku
			j--;
		}
	}
	if(l<j)ppl_Quicksort(position,light_index,l,j);			// triedime pokial useky su vacsie ako 1
	if(i<r)ppl_Quicksort(position,light_index,i,r);
}

void AsePplVso::ppl_Render_light(int index, int heightMap)
{
//	if(heightMap && tHeight==NULL)return;
	float light_range2 = lights[index].light_range;
	light_range2 *= light_range2;

	pd->SetPixelShaderConstant( 1, vec4(lights[index].color).v, 1);
	pd->SetPixelShaderConstant( 2, vec4(lights[index].color).v, 1);
	pd->SetVertexShaderConstant( 6, vec4(lights[index].lpos,1.f/lights[index].light_range).v, 1 );
	pd->SetVertexShaderConstant( 7, vec4( 1,-1, 1, light_range2).v, 1 );

	for(int i=0; i<dxobjs.size(); i++)
	{
		zASE_Material *mat=NULL;
		if(dxobjs[i].pvb==NULL)continue;
		if(!objects[i].visible)continue;
		if(!SphereInBox(lights[index].lpos, lights[index].light_range, objects[i].min, objects[i].max))continue;

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
		pd->SetStreamSource( 0, dxobjs[i].pvb, sizeof(zAsePplVso_CUSTOMVERTEX) );
		
		for(int j=0; j<dxobjs[i].count-21000; j+=21000)
			pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, 21000 );
		pd__DrawPrimitive( D3DPT_TRIANGLELIST, 3*j, dxobjs[i].count-j );
	}
}
