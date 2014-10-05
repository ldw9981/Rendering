#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"
#include "Graphics/Graphics.h"
#include "Graphics/MaterialEx.h"
#include "Scene/CameraNode.h"
#include "Graphics/RscTexture.h"
namespace Sophia
{


cRendererQueue::cRendererQueue()
{

}

cRendererQueue::~cRendererQueue()
{
	
}


void cRendererQueue::Insert( IRenderer* pItem ,MultiSub* pMultiSub,Material* pMaterial)
{
	std::pair<IRenderer*,Specific> info;
	info.first = pItem;
	info.second.pMaterial = pMaterial;
	info.second.pMultiSub = pMultiSub;

	m_listNode.push_back(info);
}

void cRendererQueue::Insert( cRendererQueue& renderQueue )
{
	m_listNode.insert(m_listNode.end(),renderQueue.m_listNode.begin(),renderQueue.m_listNode.end());
}

void cRendererQueue::Render()
{
 	auto it=m_listNode.begin();
 	for ( ; it!=m_listNode.end(); ++it )
 	{		
 		(*it).first->Render( (*it).second.pMultiSub,(*it).second.pMaterial );
 	}
}

bool cRendererQueue::IsEmpty()
{
	return m_listNode.empty();
}

void cRendererQueue::Clear()
{
	m_listNode.clear();
}


void cRendererQueue::RenderAlphaBlend(std::vector<D3DXHANDLE>& vecTechnique,cCameraNode* pCamera)
{
	SortByCamera(pCamera);
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics::m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();	
	UINT passes = 0;		

	Material* pPrevMaterial=NULL;
	for ( auto it=m_listNode.begin() ; it!=m_listNode.end(); ++it )
	{		
		Specific& item = (*it).second;		
		if (pPrevMaterial!=item.pMaterial)
		{
			if (pPrevMaterial != NULL)
			{
				pEffect->EndPass();
				pEffect->End();				
			}

			int i = item.pMaterial->index_renderer_queue();
			pEffect->SetTechnique(vecTechnique[i]);
			pEffect->Begin(&passes, 0);	
			pEffect->BeginPass(0);	

			// Material적용
			SetMaterial(*item.pMaterial);
		}

		(*it).first->Render( (*it).second.pMultiSub,(*it).second.pMaterial );
		pPrevMaterial = item.pMaterial;
	}

	if (pPrevMaterial)
	{
		pEffect->EndPass();
		pEffect->End();	
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}


static bool GreateDistance(std::pair<IRenderer*,cRendererQueue::Specific>& a,std::pair<IRenderer*,cRendererQueue::Specific>& b)
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
	pVecCam = pCamera->GetWorldPos();
	auto it=m_listNode.begin();
	for ( ; it!=m_listNode.end(); ++it )
	{		
		Specific& item = (*it).second;		
		pVecRender = (*it).first->GetRenderWorldPos();

		temp = *pVecCam - *pVecRender;
		item.distancesq = D3DXVec3LengthSq(&temp);	
	}

	m_listNode.sort(&GreateDistance);
}

static bool GreateMaterial(std::pair<IRenderer*,cRendererQueue::Specific>& a,std::pair<IRenderer*,cRendererQueue::Specific>& b)
{
	if ( a.second.pMaterial > b.second.pMaterial)
		return true;

	return false;
}

void cRendererQueue::SortByMaterial()
{
	m_listNode.sort(&GreateMaterial);
}

void cRendererQueue::RenderNotAlphaBlend(std::vector<D3DXHANDLE>& vecTechnique)
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	SortByMaterial();
	UINT passes = 0;		

	Material* pPrevMaterial=NULL;
	for ( auto it=m_listNode.begin() ; it!=m_listNode.end(); ++it )
	{		
		Specific& item = (*it).second;		
		if (pPrevMaterial!=item.pMaterial)
		{
			if (pPrevMaterial != NULL)
			{
				pEffect->EndPass();
				pEffect->End();				
			}

			int i = item.pMaterial->index_renderer_queue();
			pEffect->SetTechnique(vecTechnique[i]);
			pEffect->Begin(&passes, 0);	
			pEffect->BeginPass(0);	

			// Material적용
			SetMaterial(*item.pMaterial);
		}
		
		(*it).first->Render( (*it).second.pMultiSub,(*it).second.pMaterial );
		pPrevMaterial = item.pMaterial;
	}

	if (pPrevMaterial)
	{
		pEffect->EndPass();
		pEffect->End();	
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

void cRendererQueue::SetMaterial( Material& material )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();


	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, material.AlphaTestEnable); 	
	if (material.AlphaTestEnable)
	{
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)material.AlphaTestRef);
	}

	cRscTexture* pRscTexture;
	pRscTexture = material.GetMapDiffuse();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex0",pRscTexture->GetD3DTexture());

	pRscTexture = material.GetMapNormal();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex1",pRscTexture->GetD3DTexture());

	pRscTexture = material.GetMapLight();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex3",pRscTexture->GetD3DTexture());

	pRscTexture = material.GetMapOpacity();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Opacity_Tex",pRscTexture->GetD3DTexture());

	pRscTexture = material.GetMapSpecular();
	if( pRscTexture != NULL )	
		pEffect->SetTexture("Tex2",pRscTexture->GetD3DTexture());
}

void cRendererQueue::RenderShadow( D3DXHANDLE hTechniqueNotAlphaTest,D3DXHANDLE hTechniqueAlphaTest )
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	SortByMaterial();
	UINT passes = 0;		
	
	//테크닉을 바꾸는 조건, material다르고 AlphaTestEnable 다를경우
	int	 nPrevAlphaTestEnable = -1;
	Material* pPrevMaterial=NULL;
	for ( auto it=m_listNode.begin() ; it!=m_listNode.end(); ++it )
	{		
		Specific& item = (*it).second;		
		if (pPrevMaterial!=item.pMaterial )
		{
			if (nPrevAlphaTestEnable != (int)item.pMaterial->AlphaTestEnable)
			{
				if (nPrevAlphaTestEnable != -1 )
				{
					pEffect->EndPass();
					pEffect->End();				
				}
				int i = item.pMaterial->index_renderer_queue();

				if (item.pMaterial->AlphaTestEnable)
					pEffect->SetTechnique(hTechniqueAlphaTest);
				else
					pEffect->SetTechnique(hTechniqueNotAlphaTest );
				
				pEffect->Begin(&passes, 0);	
				pEffect->BeginPass(0);	
			}

			// Material적용
			SetMaterial(*item.pMaterial);
		}

		(*it).first->Render( (*it).second.pMultiSub,(*it).second.pMaterial );
		pPrevMaterial = item.pMaterial;
		nPrevAlphaTestEnable = (int)item.pMaterial->AlphaTestEnable;
	}

	if (nPrevAlphaTestEnable!=-1)
	{
		pEffect->EndPass();
		pEffect->End();	
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

}