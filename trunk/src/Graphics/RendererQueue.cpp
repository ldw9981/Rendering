#include "StdAfx.h"
#include "RendererQueue.h"
#include "Foundation/Interface.h"
#include "Graphics/Graphics.h"
#include "Graphics/MaterialEx.h"
#include "Scene/CameraNode.h"
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
	Specific temp;
	temp.pMultiSub = pMultiSub;
	temp.pMaterial = pMaterial;
	m_listNode.push_back(std::make_pair(pItem,temp));
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

void cRendererQueue::RenderAlphaTest()
{
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	auto it=m_listNode.begin();
	for ( ; it!=m_listNode.end(); ++it )
	{		
		Specific& item = (*it).second;		
		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)item.pMaterial->AlphaTestRef);
		(*it).first->Render( (*it).second.pMultiSub,(*it).second.pMaterial );
	}
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false); 
}

void cRendererQueue::RenderAlphaBlendAndTest(cCameraNode* pCamera)
{
	Sort(pCamera);
	Graphics::m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true); 	
	Graphics::m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics::m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	auto it=m_listNode.begin();
	for ( ; it!=m_listNode.end(); ++it )
	{		
		Specific& item = (*it).second;		

		Graphics::m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, item.pMaterial->AlphaTestEnable); 			
		if (item.pMaterial->AlphaTestEnable)
		{		
			Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			Graphics::m_pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)item.pMaterial->AlphaTestRef);
		}

		(*it).first->Render( (*it).second.pMultiSub,(*it).second.pMaterial );
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

void cRendererQueue::Sort(cCameraNode* pCamera)
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

}