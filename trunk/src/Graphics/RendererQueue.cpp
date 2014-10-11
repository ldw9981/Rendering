#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"
#include "Graphics/Graphics.h"
#include "Graphics/MaterialEx.h"
#include "Scene/CameraNode.h"
#include "Graphics/RscTexture.h"
#include "Scene/MeshNode.h"


namespace Sophia
{


cRendererQueue::cRendererQueue()
{

}

cRendererQueue::~cRendererQueue()
{
	
}


void cRendererQueue::Insert( cMeshNode* pItem ,Material* pMaterial)
{
	std::pair<cMeshNode*,Specific> info;
	info.first = pItem;
	info.second.pMaterial = pMaterial;
	m_vecNode.push_back(info);
}

void cRendererQueue::Insert( cRendererQueue& renderQueue )
{
	m_vecNode.insert(m_vecNode.end(),renderQueue.m_vecNode.begin(),renderQueue.m_vecNode.end());
}

void cRendererQueue::Render()
{
	auto it=m_vecNode.begin();
	for ( ; it!=m_vecNode.end(); ++it )
	{		
		(*it).first->Render();
	}
}


void cRendererQueue::Clear()
{
	m_vecNode.clear();
	m_materialOrder.clear();
	m_entityMeshNameOrder.clear();
}


void cRendererQueue::RenderAlphaBlendByDistanceOrder(std::vector<D3DXHANDLE>& vecTechnique,cCameraNode* pCamera)
{
	SortByCamera(pCamera);
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics::m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();	
	

	std::vector<MESH_SPEC_PAIR>	containerTemp;

	Material* pPrevMaterial=NULL;
	for ( auto it=m_vecNode.begin() ; it!=m_vecNode.end(); ++it )
	{		
		Specific& item = (*it).second;		
		if (pPrevMaterial!=item.pMaterial && !containerTemp.empty())
		{
			SubRenderAlphaBlend(vecTechnique,containerTemp);
			containerTemp.clear();
		}		
		containerTemp.push_back(*it);				
		pPrevMaterial = item.pMaterial;
	}
	SubRenderAlphaBlend(vecTechnique,containerTemp);

	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::SubRenderAlphaBlend( std::vector<D3DXHANDLE>& vecTechnique,std::vector<MESH_SPEC_PAIR>& containerTemp )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();	
	UINT passes = 0;		

	for ( auto it=containerTemp.begin() ; it!=containerTemp.end(); ++it )
	{		
		Specific& item = (*it).second;		
		
		int i = item.pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);
		pEffect->Begin(&passes, 0);	
		// MaterialÀû¿ë
		ChangeMaterial(*item.pMaterial,false);

		pEffect->BeginPass(0);	
		(*it).first->Render();

		pEffect->EndPass();
		pEffect->End();	
	}
}



static bool GreateDistance(std::pair<cMeshNode*,cRendererQueue::Specific>& a,std::pair<cMeshNode*,cRendererQueue::Specific>& b)
{
	if ( a.second.distancesq > b.second.distancesq)
		return true;

	return false;
}

void cRendererQueue::SortByCamera(cCameraNode* pCamera)
{
	D3DXVECTOR3 temp;
	const D3DXVECTOR3* pVecCam;
	const D3DXVECTOR3* pVecRender;
	pVecCam = pCamera->GetWorldPositionPtr();
	auto it=m_vecNode.begin();
	for ( ; it!=m_vecNode.end(); ++it )
	{		
		Specific& item = (*it).second;		
		pVecRender = (*it).first->GetWorldPositionPtr();

		temp = *pVecCam - *pVecRender;
		item.distancesq = D3DXVec3LengthSq(&temp);	
	}

	std::sort(m_vecNode.begin(),m_vecNode.end(),&GreateDistance);
}

static bool GreateMaterial(std::pair<cMeshNode*,cRendererQueue::Specific>& a,std::pair<cMeshNode*,cRendererQueue::Specific>& b)
{
	if ( a.second.pMaterial > b.second.pMaterial)
		return true;

	return false;
}

void cRendererQueue::SortByMaterial()
{
	std::sort(m_vecNode.begin(),m_vecNode.end(),&GreateMaterial);	
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

		ChangeMaterial(*pMaterial,false);
	
		pEffect->BeginPass(0);	
		std::list<MESH_SPEC_PAIR>& vecMesh = it->second;
		for (auto it_sub = vecMesh.begin() ; it_sub!=vecMesh.end();++it_sub)
		{
			MESH_SPEC_PAIR& subItem = *it_sub;
			subItem.first->Render();
		}
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::ChangeMaterial( Material& material,bool bForShadow )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();


	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, material.AlphaTestEnable); 	
	if (material.AlphaTestEnable)
	{
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)material.AlphaTestRef);
	}

	cRscTexture* pRscTexture;
	pRscTexture = material.GetMapOpacity();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Opacity_Tex",pRscTexture->GetD3DTexture());

	if(!bForShadow)
	{
		pRscTexture = material.GetMapDiffuse();
		if( pRscTexture != NULL )	
			pEffect->SetTexture("Tex0",pRscTexture->GetD3DTexture());

		pRscTexture = material.GetMapNormal();
		if( pRscTexture != NULL )	
			pEffect->SetTexture("Tex1",pRscTexture->GetD3DTexture());

		pRscTexture = material.GetMapLight();
		if( pRscTexture != NULL )	
			pEffect->SetTexture("Tex3",pRscTexture->GetD3DTexture());

		pRscTexture = material.GetMapSpecular();
		if( pRscTexture != NULL )	
			pEffect->SetTexture("Tex2",pRscTexture->GetD3DTexture());
	}
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
		
		ChangeMaterial(*pMaterial,true);		

		pEffect->BeginPass(0);	
		std::list<MESH_SPEC_PAIR>& vecMesh = it->second;
		for (auto it_sub = vecMesh.begin() ; it_sub!=vecMesh.end();++it_sub)
		{
			MESH_SPEC_PAIR& subItem = *it_sub;
			subItem.first->Render();
		}
		pEffect->EndPass();
		pEffect->End();		
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}


void cRendererQueue::InsertIntoMaterialOrder( cRendererQueue& renderQueue )
{
	size_t size = renderQueue.m_vecNode.size();
	for (size_t i=0;i<size;i++)
	{
		MESH_SPEC_PAIR& item = renderQueue.m_vecNode[i];

		std::list<MESH_SPEC_PAIR>& listMesh = m_materialOrder[item.second.pMaterial];
		listMesh.push_back(item);
	}

}

void cRendererQueue::InsertIntoEntityMeshNameOrder(cRendererQueue& renderQueue )
{	
	for (auto it = renderQueue.m_vecNode.begin() ; it!= renderQueue.m_vecNode.end() ; it++)
	{
		MESH_SPEC_PAIR& item = (*it);		

		SCENE key(item.first->GetRscVetextBuffer(),item.second.pMaterial,item.first->GetRscIndexBuffer());
		std::list<MESH_SPEC_PAIR>& list = m_entityMeshNameOrder[key];
		list.push_back(item);
	}
}

void cRendererQueue::RenderInstancing( std::vector<D3DXHANDLE>& vecTechnique )
{	
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		
	Material* pPrevMaterial=NULL;

	Graphics::m_pInstance->m_pInstanceVertexBuffer->SetStreamSource(1,D3DXGetDeclVertexSize(declInstance,1));
	Graphics::m_pInstance->m_pInstanceVertexBuffer->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1);
	
	for ( auto it = m_entityMeshNameOrder.begin() ; it!=m_entityMeshNameOrder.end();++it)
	{
		
		std::list<MESH_SPEC_PAIR>& list = it->second;
		auto it_sub = list.begin();
		cMeshNode* pMeshNode = NULL;
		Material* pMaterial = (*it_sub).second.pMaterial;

		// Set Matrix Instance
		
		unsigned long nCount=list.size();
		D3DXMATRIX* pMatrix = (D3DXMATRIX*)Graphics::m_pInstance->m_pInstanceVertexBuffer->Lock(0,
			nCount*sizeof(D3DXMATRIX)	,D3DLOCK_DISCARD	);

		for (  ; it_sub!=list.end();++it_sub)
		{
			pMeshNode = (*it_sub).first;		
			memcpy_s(pMatrix,sizeof(D3DXMATRIX),pMeshNode->GetWorldMatrixPtr(),sizeof(D3DXMATRIX));
			pMatrix++;
		}
		Graphics::m_pInstance->m_pInstanceVertexBuffer->Unlock();		
		
		pMeshNode->GetRscVetextBuffer()->SetStreamSource(0,D3DXGetDeclVertexSize(declInstance,0));
		pMeshNode->GetRscVetextBuffer()->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | nCount);
		pMeshNode->GetRscIndexBuffer()->SetIndices();
		int i = pMaterial->index_renderer_queue();
		pEffect->SetTechnique(vecTechnique[i]);	
		pEffect->Begin(&passes, 0);	
		ChangeMaterial(*pMaterial,false);		

		pEffect->BeginPass(0);	
		pMeshNode->RenderIsntancing();
		pEffect->EndPass();
		pEffect->End();		
		
	}
	Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
	Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	
}

void cRendererQueue::RenderShadowInstancing( D3DXHANDLE hTShadowNotAlphaTest,D3DXHANDLE hTShadowAlphaTest )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();

	UINT passes = 0;		
	Material* pPrevMaterial=NULL;

	Graphics::m_pInstance->m_pInstanceVertexBuffer->SetStreamSource(1,D3DXGetDeclVertexSize(declInstance,1));
	Graphics::m_pInstance->m_pInstanceVertexBuffer->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1);

	for ( auto it = m_entityMeshNameOrder.begin() ; it!=m_entityMeshNameOrder.end();++it)
	{

		std::list<MESH_SPEC_PAIR>& list = it->second;
		auto it_sub = list.begin();
		cMeshNode* pMeshNode = NULL;
		Material* pMaterial = (*it_sub).second.pMaterial;

		// Set Matrix Instance
		unsigned long nCount=list.size();
		D3DXMATRIX* pMatrix = (D3DXMATRIX*)Graphics::m_pInstance->m_pInstanceVertexBuffer->Lock(0,
			nCount*sizeof(D3DXMATRIX)	,D3DLOCK_DISCARD	);
		
		for (  ; it_sub!=list.end();++it_sub)
		{
			pMeshNode = (*it_sub).first;		
			memcpy_s(pMatrix,sizeof(D3DXMATRIX),pMeshNode->GetWorldMatrixPtr(),sizeof(D3DXMATRIX));
			pMatrix++;
		}
		Graphics::m_pInstance->m_pInstanceVertexBuffer->Unlock();		

		pMeshNode->GetRscVetextBuffer()->SetStreamSource(0,D3DXGetDeclVertexSize(declInstance,0));
		pMeshNode->GetRscVetextBuffer()->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | nCount);
		pMeshNode->GetRscIndexBuffer()->SetIndices();
		
		if (pMaterial->AlphaTestEnable)
			pEffect->SetTechnique(hTShadowAlphaTest);
		else
			pEffect->SetTechnique(hTShadowNotAlphaTest );

		pEffect->Begin(&passes, 0);	
		ChangeMaterial(*pMaterial,true);		

		pEffect->BeginPass(0);	
		pMeshNode->RenderIsntancing();
		pEffect->EndPass();
		pEffect->End();			
	}
	Graphics::m_pDevice->SetStreamSourceFreq( 0, 1 );
	Graphics::m_pDevice->SetStreamSourceFreq( 1, 1 );
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}







SCENE::SCENE()
{
	pVertexBuffer=NULL;
	pMaterial=NULL;
	pIndexBuffer=NULL;	
}

SCENE::SCENE(cRscVertexBuffer* param0,Material* param1,cRscIndexBuffer* param2)
{
	pVertexBuffer=param0;
	pMaterial=param1;
	pIndexBuffer=param2;	
}

bool SCENE::operator<( const SCENE& other ) const
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

}