#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"
#include "Graphics/Graphics.h"
#include "Graphics/MaterialEx.h"
#include "Scene/CameraNode.h"
#include "Graphics/RscTexture.h"
#include "Scene/MeshNode.h"
#include "Scene/SkinnedMeshNode.h"
#include "Graphics/MatrixStreamVertexBuffer.h"
#include "Graphics/IndexStreamVertexBuffer.h"
#include "Graphics/BoneStreamTexture.h"
#include "Graphics/VertexTransformationTexture.h"
#include "Graphics/VertexStream.h"
#include "Graphics/Vertex.h"
#include "Foundation/Trace.h"

namespace Sophia
{


cRendererQueue::cRendererQueue()
{

}

cRendererQueue::~cRendererQueue()
{
	
}


void cRendererQueue::InsertIntoMeshList( cMeshNode* pItem)
{
	m_vecMesh.push_back(pItem);
}



static bool GreateDistance(cRendererQueue::MESH_DISTANCE_PAIR& a,cRendererQueue::MESH_DISTANCE_PAIR& b)
{
	if ( a.second > b.second)
		return true;

	return false;
}

void cRendererQueue::InsertIntoDistanceOrder( cRendererQueue& renderQueue , D3DXVECTOR3* pCameraWorldPosition )
{
	auto it = renderQueue.m_vecMesh.begin();
	auto it_end = renderQueue.m_vecMesh.end();
	D3DXVECTOR3 temp;
	for ( ; it!=it_end ; ++it )
	{		
		auto& pMesh = *it;		
		temp = *pCameraWorldPosition - *(pMesh->GetWorldPositionPtr());		
		m_distanceOrder.push_back(MESH_DISTANCE_PAIR(pMesh,D3DXVec3LengthSq(&temp)));
	}
}



void cRendererQueue::Clear()
{
	m_vecMesh.clear();
	m_materialOrder.clear();
	m_sceneOrder.clear();
	m_distanceOrder.clear();
}


void cRendererQueue::RenderAlphaBlendByDistanceOrder(std::vector<D3DXHANDLE>& vecTechnique)
{
	//그릴때 한번에 정렬
	std::sort(m_distanceOrder.begin(),m_distanceOrder.end(),&GreateDistance);

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics::m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();		

	std::vector<MESH_DISTANCE_PAIR>	containerTemp;

	Material* pPrevMaterial=NULL;
	for ( auto it=m_distanceOrder.begin() ; it!=m_distanceOrder.end(); ++it )
	{		
		MESH_DISTANCE_PAIR& item = *it;		
		Material* pCurrMaterial = item.first->GetMaterial();
		if (pPrevMaterial!= pCurrMaterial && !containerTemp.empty())
		{
			SubRenderAlphaBlend(vecTechnique,containerTemp);
			containerTemp.clear();
		}		
		containerTemp.push_back(*it);				
		pPrevMaterial = pCurrMaterial;
	}
	SubRenderAlphaBlend(vecTechnique,containerTemp);

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::SubRenderAlphaBlend( std::vector<D3DXHANDLE>& vecTechnique,std::vector<MESH_DISTANCE_PAIR>& containerTemp )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();	
	UINT passes = 0;		

	for ( auto it=containerTemp.begin() ; it!=containerTemp.end(); ++it )
	{		
		MESH_DISTANCE_PAIR& item = *it;		
		Material* pMaterial = item.first->GetMaterial();
		
		int i = pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);
		pEffect->Begin(&passes, 0);	
		// Material적용
		ChangeMaterial(pMaterial);

		pEffect->BeginPass(0);	
		(*it).first->Render();

		pEffect->EndPass();
		pEffect->End();	
	}
}

void cRendererQueue::RenderNotAlphaBlendByMaterialOrder(std::vector<D3DXHANDLE>& vecTechnique)
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	
	UINT passes = 0;		
	Material* pPrevMaterial=NULL;

	for ( auto it = m_materialOrder.begin() ; it!=m_materialOrder.end();++it)
	{
		Material* pMaterial = it->first;
		int i = pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);
		pEffect->Begin(&passes, 0);	

		ChangeMaterial(pMaterial);
	
		pEffect->BeginPass(0);	
		std::list<cMeshNode*>& vecMesh = it->second;
		for (auto it_sub = vecMesh.begin() ; it_sub!=vecMesh.end();++it_sub)
		{
			(*it_sub)->Render();
		}
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::ChangeMaterial(Material* pMaterial )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();


	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, pMaterial->AlphaTestEnable); 	
	if (pMaterial->AlphaTestEnable)
	{
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)pMaterial->AlphaTestRef);
	}

	cRscTexture* pRscTexture;
	pRscTexture = pMaterial->GetMapOpacity();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Opacity",pRscTexture->GetD3DTexture());

	pRscTexture = pMaterial->GetMapDiffuse();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Diffuse",pRscTexture->GetD3DTexture());

	pRscTexture = pMaterial->GetMapNormal();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Normal",pRscTexture->GetD3DTexture());

	pRscTexture = pMaterial->GetMapLight();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Light",pRscTexture->GetD3DTexture());

	pRscTexture = pMaterial->GetMapSpecular();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Specular",pRscTexture->GetD3DTexture());	
}


void cRendererQueue::ChangeMaterialForShadow( Material* pMaterial )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, pMaterial->AlphaTestEnable); 	
	if (pMaterial->AlphaTestEnable)
	{
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)pMaterial->AlphaTestRef);
	}

	cRscTexture* pRscTexture;
	pRscTexture = pMaterial->GetMapOpacity();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex_Opacity",pRscTexture->GetD3DTexture());	
}


void cRendererQueue::RenderShadowByMaterialOrder( D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		
	Material* pPrevMaterial=NULL;

	for ( auto it = m_materialOrder.begin() ; it!=m_materialOrder.end();++it)
	{
		Material* pMaterial = it->first;

		if (pMaterial->AlphaTestEnable)
			pEffect->SetTechnique(hTShadowAlphaTest);
		else
			pEffect->SetTechnique(hTShadowNotAlphaTest );
		
		pEffect->Begin(&passes, 0);	
		
		ChangeMaterialForShadow(pMaterial);		

		pEffect->BeginPass(0);	
		std::list<cMeshNode*>& vecMesh = it->second;
		for (auto it_sub = vecMesh.begin() ; it_sub!=vecMesh.end();++it_sub)
		{
			(*it_sub)->Render();
		}
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::InsertNotAlphaBlend( cRendererQueue& renderQueue )
{
	size_t size = renderQueue.m_vecMesh.size();
	for (size_t i=0;i<size;i++)
	{
		cMeshNode* pMesh = renderQueue.m_vecMesh[i];	

		Material* pMaterial = pMesh->GetMaterial();
		if (pMesh->GetInstancingEnable())
		{
			SCENE_KEY key(pMesh->GetRscVetextBuffer(),pMesh->GetMaterial(),pMesh->GetRscIndexBuffer());
			std::list<cMeshNode*>& list = m_sceneOrder[key];
			list.push_back(pMesh);
		}
		else
		{
			std::list<cMeshNode*>& list = m_materialOrder[pMaterial];
			list.push_back(pMesh);
		}
	}
}


void cRendererQueue::InsertIntoMaterialOrder( cRendererQueue& renderQueue )
{
	size_t size = renderQueue.m_vecMesh.size();
	for (size_t i=0;i<size;i++)
	{
		Material* pMaterial = renderQueue.m_vecMesh[i]->GetMaterial();

		std::list<cMeshNode*>& listMesh = m_materialOrder[pMaterial];
		listMesh.push_back(renderQueue.m_vecMesh[i]);
	}

}

void cRendererQueue::InsertIntoSceneOrder(cRendererQueue& renderQueue )
{	
	for (auto it = renderQueue.m_vecMesh.begin() ; it!= renderQueue.m_vecMesh.end() ; it++)
	{
		auto& pMesh = (*it);	

		SCENE_KEY key(pMesh->GetRscVetextBuffer(),pMesh->GetMaterial(),pMesh->GetRscIndexBuffer());
		std::list<cMeshNode*>& list = m_sceneOrder[key];
		list.push_back(*it);
	}
}

void cRendererQueue::RenderNotAlphaBlendNormalInstancing( std::vector<D3DXHANDLE>& vecTechnique )
{
	HRESULT hr;
//	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalVertexDeclaration);
	V( Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalInstancingVertexDeclaration) );
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		
	
	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY& refScene = it->first;
		std::list<cMeshNode*>& list = it->second;

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();

		cMeshNode* pMeshNode = *it_sub;

		VertexStream* pVertexStream = pMeshNode->GetVertexStream();
		MatrixStreamVertexBuffer* pMatrixStreamVertexBuffer = pMeshNode->GetMatrixStreamVertexBuffer();
		
		
		NORMALVERTEX* pNormalVertex = (NORMALVERTEX*) pVertexStream->Lock( nCount*refScene.pVertexBuffer->GetBufferSize(),D3DLOCK_DISCARD );
		NORMALVERTEX* pSrc = (NORMALVERTEX*)refScene.pVertexBuffer->Lock(refScene.pVertexBuffer->GetBufferSize(),0);
		NORMALINSTANCE* pNormalInstance = (NORMALINSTANCE*) pMatrixStreamVertexBuffer->Lock( nCount*sizeof(NORMALINSTANCE),D3DLOCK_DISCARD );
		
		int instanceIndex=0;
		for (  ; it_sub!=list.end();++it_sub)
		{
				pMeshNode = *it_sub;

				memcpy_s(pNormalVertex,refScene.pVertexBuffer->GetBufferSize(),pSrc,refScene.pVertexBuffer->GetBufferSize());								

				memcpy_s(&pNormalInstance->instanceMatrix0,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[0],sizeof(D3DXVECTOR3));
				memcpy_s(&pNormalInstance->instanceMatrix1,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[1],sizeof(D3DXVECTOR3));
				memcpy_s(&pNormalInstance->instanceMatrix2,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[2],sizeof(D3DXVECTOR3));		
				memcpy_s(&pNormalInstance->instanceMatrix3,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[3],sizeof(D3DXVECTOR3));

				pNormalInstance->instanceIndex.u = (float)instanceIndex;

				pNormalInstance++;
				instanceIndex++;


				pNormalVertex += refScene.pVertexBuffer->GetCount();

				assert(refScene.pVertexBuffer == pMeshNode->GetRscVetextBuffer());
				assert(refScene.pIndexBuffer == pMeshNode->GetRscIndexBuffer());
		}
		pMatrixStreamVertexBuffer->Unlock();		
		refScene.pVertexBuffer->Unlock();
		pVertexStream->Unlock();		
		
		
		V( Graphics::m_pDevice->SetStreamSourceFreq(0,1) );
		V( Graphics::m_pDevice->SetStreamSource(0,pVertexStream->GetD3DVertexBuffer(),0,D3DXGetDeclVertexSize(declNormalInstance,0)) );	

		Graphics::m_pDevice->SetStreamSourceFreq(1,refScene.pVertexBuffer->GetCount());
		Graphics::m_pDevice->SetStreamSource(1,pMatrixStreamVertexBuffer->GetD3DVertexBuffer(),0,D3DXGetDeclVertexSize(declNormalInstance,1));

 		Graphics::m_pInstance->BackupRenderTarget(0);		
 		LPDIRECT3DSURFACE9 surface;
 		pMeshNode->GetVertexTransformTexture()->GetD3DTexture()->GetSurfaceLevel(0,&surface);
 		Graphics::m_pDevice->SetRenderTarget(0,surface);
		surface->Release();
		surface=NULL;

		pEffect->SetFloat(Graphics::m_pInstance->m_hfVertexTextureWidth,(float)pMeshNode->GetVertexTransformTexture()->GetWidth());
		pEffect->SetFloat(Graphics::m_pInstance->m_hfVertexTextureHeight,(float)pMeshNode->GetVertexTransformTexture()->GetHeight());

		pEffect->SetTechnique(Graphics::m_pInstance->m_hTVertexTransform);	
		pEffect->Begin(&passes, 0);
		pEffect->BeginPass(0);
		pEffect->CommitChanges();
		Graphics::m_pDevice->DrawPrimitive(D3DPT_POINTLIST,0,refScene.pVertexBuffer->GetCount()*nCount);
		pEffect->EndPass();
		pEffect->End();		
		Graphics::m_pInstance->RestoreRenderTarget(0);	


		//	
		V(Graphics::m_pDevice->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | nCount));		
		V(Graphics::m_pDevice->SetStreamSource(0,refScene.pVertexBuffer->GetD3DVertexBuffer(),0,D3DXGetDeclVertexSize(declNormalInstance,0)));		

		V(Graphics::m_pDevice->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1));
		V(Graphics::m_pDevice->SetStreamSource(1,pMatrixStreamVertexBuffer->GetD3DVertexBuffer(),0,D3DXGetDeclVertexSize(declNormalInstance,1)));

		V(Graphics::m_pDevice->SetIndices(refScene.pIndexBuffer->GetD3DIndexBuffer())); 
		V(pEffect->SetTexture("Tex_TransformedVertex",pMeshNode->GetVertexTransformTexture()->GetD3DTexture()));
		
		int i = refScene.pMaterial->index_renderer_queue();
		V(pEffect->SetTechnique(vecTechnique[i]));	
		V(pEffect->Begin(&passes, 0));	

		ChangeMaterial(refScene.pMaterial);

		V(pEffect->BeginPass(0));
		pMeshNode->RenderIsntancing();
		V(pEffect->EndPass());		
		V(pEffect->End());		
		
		V(Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 ));
		V(Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 ));
		
	}

	V(Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));
	
}

void cRendererQueue::RenderShadowNormalInstancing( D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest )
{
	return;

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalInstancingVertexDeclaration);
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;			


	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{
		const SCENE_KEY& refScene = it->first;
		std::list<cMeshNode*>& list = it->second;
	
		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();

		cMeshNode* pMeshNode = *it_sub;
		
		MatrixStreamVertexBuffer* pMatrixStreamVertexBuffer = pMeshNode->GetMatrixStreamVertexBuffer();
			
		D3DXVECTOR3* pVector = (D3DXVECTOR3*) pMatrixStreamVertexBuffer->Lock(
			nCount*sizeof(D3DXVECTOR3)*4,D3DLOCK_DISCARD	);

		for (  ; it_sub!=list.end();++it_sub)
		{
			pMeshNode = *it_sub;
			memcpy_s(pVector+0,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[0],sizeof(D3DXVECTOR3));
			memcpy_s(pVector+1,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[1],sizeof(D3DXVECTOR3));
			memcpy_s(pVector+2,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[2],sizeof(D3DXVECTOR3));		
			memcpy_s(pVector+3,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[3],sizeof(D3DXVECTOR3));
			pVector+=4;

			assert(refScene.pVertexBuffer == pMeshNode->GetRscVetextBuffer());
			assert(refScene.pIndexBuffer == pMeshNode->GetRscIndexBuffer());
		}
		pMatrixStreamVertexBuffer->Unlock();	
		pMatrixStreamVertexBuffer->SetValid(true);

		refScene.pVertexBuffer->SetStreamSource(0,D3DXGetDeclVertexSize(declNormalInstance,0));
		refScene.pVertexBuffer->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | nCount);		
		pMatrixStreamVertexBuffer->SetStreamSource(1,D3DXGetDeclVertexSize(declNormalInstance,1));
		pMatrixStreamVertexBuffer->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1);

		refScene.pIndexBuffer->SetIndices();
		
		if (refScene.pMaterial->AlphaTestEnable)
			pEffect->SetTechnique(hTShadowAlphaTest);
		else
			pEffect->SetTechnique(hTShadowNotAlphaTest );

		pEffect->Begin(&passes, 0);	
		ChangeMaterialForShadow(refScene.pMaterial);		

		pEffect->BeginPass(0);	
		pMeshNode->RenderIsntancing();
		pEffect->EndPass();
		pEffect->End();	

		Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
		Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );

		
	}

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::RenderNotAlphaBlendSkinnedInstancing( std::vector<D3DXHANDLE>& vecTechnique )
{
	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedInstancingVertexDeclaration);
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		

	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY&	refScene = it->first;
		std::list<cMeshNode*>& list = it->second;		

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();
		SkinnedMeshNode* pMeshNode = static_cast<SkinnedMeshNode*>(*it_sub);

		IndexStreamVertexBuffer* pIndexStreamVertexBuffer = pMeshNode->GetIndexStreamVertexBuffer();
		BoneStreamTexture* pBoneStreamTexture = pMeshNode->GetBoneStreamTexture();
		if (!pIndexStreamVertexBuffer->GetValid() || !pBoneStreamTexture->GetValid())
		{
			BLENDINSTANCEVERTEX* pVertex = (BLENDINSTANCEVERTEX*)pIndexStreamVertexBuffer->Lock(nCount*sizeof(BLENDINSTANCEVERTEX),D3DLOCK_DISCARD	);

			D3DLOCKED_RECT lock;	
			pBoneStreamTexture->Lock(&lock,D3DLOCK_DISCARD);
			D3DXMATRIX* pMatrix=NULL;

			unsigned int instanceIndex=0;
			for (  ; it_sub!=list.end();++it_sub)
			{
				pMeshNode = static_cast<SkinnedMeshNode*>(*it_sub);
				pVertex->instanceIndex.u = (float)instanceIndex;
				pVertex++;

				assert(refScene.pVertexBuffer == pMeshNode->GetRscVetextBuffer());
				assert(refScene.pIndexBuffer == pMeshNode->GetRscIndexBuffer());

				pMeshNode->UpdateMatrixPallete();
				size_t bone_size = pMeshNode->GetArrayBoneRef().size();

				assert(lock.Pitch >= (INT)(sizeof(D3DXMATRIX)*bone_size));

				pMatrix= (D3DXMATRIX*)((LPBYTE)lock.pBits+instanceIndex*lock.Pitch);
				memcpy_s(pMatrix,sizeof(D3DXMATRIX)*bone_size,pMeshNode->GetMatrixPallete(),sizeof(D3DXMATRIX)*bone_size);
				

				instanceIndex++;
			}

			pBoneStreamTexture->Unlock();
			pIndexStreamVertexBuffer->Unlock();	
		}
		pBoneStreamTexture->SetValid(false);
		pIndexStreamVertexBuffer->SetValid(false);

		pIndexStreamVertexBuffer->SetStreamSource(1,D3DXGetDeclVertexSize(declBlendInstance,1));
		pIndexStreamVertexBuffer->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1);
		refScene.pVertexBuffer->SetStreamSource(0, D3DXGetDeclVertexSize(declBlendInstance,0));
		refScene.pVertexBuffer->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | nCount);
		refScene.pIndexBuffer->SetIndices();

		int i = refScene.pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);	
		pEffect->Begin(&passes, 0);	

		pEffect->SetTexture("Tex_BoneMatrix",pBoneStreamTexture->GetD3DTexture());
		ChangeMaterial(refScene.pMaterial);

		pEffect->BeginPass(0);
		pMeshNode->RenderIsntancing();
		pEffect->EndPass();

		pEffect->End();		

		Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
		Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	}

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::RenderShadowSkinnedInstancing( D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest )
{
	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pSkinnedInstancingVertexDeclaration);
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		
	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY&	refScene = it->first;
		std::list<cMeshNode*>& list = it->second;		

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();
		SkinnedMeshNode* pMeshNode = static_cast<SkinnedMeshNode*>(*it_sub);

		IndexStreamVertexBuffer* pIndexStreamVertexBuffer = pMeshNode->GetIndexStreamVertexBuffer();
		BoneStreamTexture* pBoneStreamTexture = pMeshNode->GetBoneStreamTexture();


		BLENDINSTANCEVERTEX* pVertex = (BLENDINSTANCEVERTEX*)pIndexStreamVertexBuffer->Lock(nCount*sizeof(BLENDINSTANCEVERTEX),D3DLOCK_DISCARD	);
		D3DLOCKED_RECT lock;	
		pBoneStreamTexture->Lock(&lock,D3DLOCK_DISCARD);
		D3DXMATRIX* pMatrix=NULL;

		unsigned int instanceIndex=0;
		for (  ; it_sub!=list.end();++it_sub)
		{
			pMeshNode = static_cast<SkinnedMeshNode*>(*it_sub);
			pVertex->instanceIndex.u = (float)instanceIndex;
			pVertex++;

			assert(refScene.pVertexBuffer == pMeshNode->GetRscVetextBuffer());
			assert(refScene.pIndexBuffer == pMeshNode->GetRscIndexBuffer());

			pMeshNode->UpdateMatrixPallete();
			size_t bone_size = pMeshNode->GetArrayBoneRef().size();

			assert(lock.Pitch >= (INT)(sizeof(D3DXMATRIX)*bone_size));

			pMatrix= (D3DXMATRIX*)((LPBYTE)lock.pBits+instanceIndex*lock.Pitch);			
			memcpy_s(pMatrix,sizeof(D3DXMATRIX)*bone_size,pMeshNode->GetMatrixPallete(),sizeof(D3DXMATRIX)*bone_size);
			
			instanceIndex++;
		}
		pBoneStreamTexture->Unlock();
		pIndexStreamVertexBuffer->Unlock();	
		pBoneStreamTexture->SetValid(true);
		pIndexStreamVertexBuffer->SetValid(true);



		pIndexStreamVertexBuffer->SetStreamSource(1,D3DXGetDeclVertexSize(declBlendInstance,1));
		pIndexStreamVertexBuffer->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1);
		refScene.pVertexBuffer->SetStreamSource(0, D3DXGetDeclVertexSize(declBlendInstance,0));
		refScene.pVertexBuffer->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | nCount);
		refScene.pIndexBuffer->SetIndices();

		if (refScene.pMaterial->AlphaTestEnable)
			pEffect->SetTechnique(hTShadowAlphaTest);
		else
			pEffect->SetTechnique(hTShadowNotAlphaTest );

		pEffect->Begin(&passes, 0);	

		//pEffect->SetTexture("Tex_BoneMatrix",pBoneStreamTexture->GetD3DTexture());
		ChangeMaterialForShadow(refScene.pMaterial);		

		pEffect->BeginPass(0);
		pMeshNode->RenderIsntancing();
		pEffect->EndPass();

		pEffect->End();		

		Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
		Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	}

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::TransformVertextNormal()
{

}

SCENE_KEY::SCENE_KEY()
{
	pVertexBuffer=NULL;
	pMaterial=NULL;
	pIndexBuffer=NULL;	
}

SCENE_KEY::SCENE_KEY(cRscVertexBuffer* param0,Material* param1,cRscIndexBuffer* param2)
{
	pVertexBuffer=param0;
	pMaterial=param1;
	pIndexBuffer=param2;	
}

bool SCENE_KEY::operator<( const SCENE_KEY& other ) const
{
	if (pVertexBuffer < other.pVertexBuffer)
		return true;

	else if (pVertexBuffer == other.pVertexBuffer)
	{
		if (pMaterial < other.pMaterial)
			return true;

		else if ( pMaterial == other.pMaterial)
		{
			return pIndexBuffer < other.pIndexBuffer;
		}

	}

	return false;
}


void cRendererQueue::RenderNotAlphaBlendInstancing( std::vector<D3DXHANDLE>& vecTechnique )
{
	Graphics::m_pDevice->SetVertexDeclaration(Graphics::m_pInstance->m_pNormalInstancingVertexDeclaration);
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		

	for ( auto it = m_sceneOrder.begin() ; it!=m_sceneOrder.end();++it)
	{	
		const SCENE_KEY& refScene = it->first;
		std::list<cMeshNode*>& list = it->second;

		// Set Matrix Instance
		unsigned long nCount=list.size();
		auto it_sub = list.begin();

		cMeshNode* pMeshNode = *it_sub;

		MatrixStreamVertexBuffer* pMatrixStreamVertexBuffer = pMeshNode->GetMatrixStreamVertexBuffer();

		NORMALINSTANCE* pNormalInstance = (NORMALINSTANCE*) pMatrixStreamVertexBuffer->Lock( nCount*sizeof(NORMALINSTANCE),D3DLOCK_DISCARD );

		int instanceIndex=0;
		for (  ; it_sub!=list.end();++it_sub)
		{
			pMeshNode = *it_sub;

			memcpy_s(&pNormalInstance->instanceMatrix0,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[0],sizeof(D3DXVECTOR3));
			memcpy_s(&pNormalInstance->instanceMatrix1,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[1],sizeof(D3DXVECTOR3));
			memcpy_s(&pNormalInstance->instanceMatrix2,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[2],sizeof(D3DXVECTOR3));		
			memcpy_s(&pNormalInstance->instanceMatrix3,sizeof(D3DXVECTOR3),&pMeshNode->m_matWorld.m[3],sizeof(D3DXVECTOR3));

			pNormalInstance->instanceIndex.u = (float)instanceIndex;

			pNormalInstance++;
			instanceIndex++;

			assert(refScene.pVertexBuffer == pMeshNode->GetRscVetextBuffer());
			assert(refScene.pIndexBuffer == pMeshNode->GetRscIndexBuffer());
		}	

		refScene.pVertexBuffer->SetStreamSource(0,D3DXGetDeclVertexSize(declNormalInstance,0));
		refScene.pVertexBuffer->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | nCount);		
		pMatrixStreamVertexBuffer->SetStreamSource(1,D3DXGetDeclVertexSize(declNormalInstance,1));
		pMatrixStreamVertexBuffer->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1);

		pMeshNode->GetRscIndexBuffer()->SetIndices();

		int i = refScene.pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);	
		pEffect->Begin(&passes, 0);	
		ChangeMaterial(refScene.pMaterial);

		pEffect->BeginPass(0);
		pMeshNode->RenderIsntancing();
		pEffect->EndPass();

		pEffect->End();		
		Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
		Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );

		pMatrixStreamVertexBuffer->SetValid(false);
	}

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);

}

}