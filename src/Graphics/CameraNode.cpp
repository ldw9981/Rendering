#include "StdAfx.h"
#include "Graphics/Entity.h"
#include "Math/CollisionDetector.h"
#include "Math/Plane.h"
#include "Math/Line.h"
#include "CameraNode.h"
#include "Foundation/Define.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"

namespace Sophia
{
	cCameraNode::cCameraNode()
	{

		//SetNodeType(CAMEARA);
		D3DXMatrixIdentity(&m_matProjection);
		D3DXMatrixIdentity(&m_matView);

		D3DXMatrixIdentity(&m_matViewProjection);
		D3DXMatrixIdentity(&m_matViewProjectionInv);

		m_FOV = 0.0f;
		m_Near = 0.0f;
		m_Far = 0.0f;
		m_ScreenWidth = 0.0f;
		m_ScreenHeight = 0.0f;
		m_bProjectionModified = false;
	}

	cCameraNode::~cCameraNode(void)
	{

	}



	/*
		cScene내부에서 ActiveCamera를 얻어 Render를 호출한다.
	*/
	void cCameraNode::Render(DWORD elapseTime)
	{
		Graphics::m_pInstance->GetEffect()->SetMatrix(Graphics::m_pInstance->m_hmView, &m_matView);
		Graphics::m_pInstance->GetEffect()->SetVector(Graphics::m_pInstance->m_hvWorldCameraPosition, (D3DXVECTOR4*)&m_matWorld.m[3][0]);
		Graphics::m_pInstance->GetEffect()->SetMatrix(Graphics::m_pInstance->m_hmProjection, &m_matProjection);
		Graphics::m_pInstance->GetEffect()->SetMatrix(Graphics::m_pInstance->m_hmViewProjection, &m_matViewProjection);
		Graphics::m_pInstance->GetEffect()->CommitChanges();
	}

	/*
	WorldTM이
	*/

	void cCameraNode::Update(DWORD elapse)
	{
		cSceneNode::Update(elapse);
		D3DXMatrixInverse(&m_matView, NULL, &m_matWorld);
		if (m_bProjectionModified)
		{
			D3DXMatrixPerspectiveFovLH(&m_matProjection, m_FOV, m_ScreenWidth / m_ScreenHeight, m_Near, m_Far);
			m_bProjectionModified = false;
		}
		m_matViewProjection = m_matView * m_matProjection;
		m_frustum.Make(m_matViewProjection);
	}


	void cCameraNode::SetLookAt(const D3DXVECTOR3 * pEye, const D3DXVECTOR3 * pAt, const D3DXVECTOR3 * pUp)
	{		
		D3DXMatrixLookAtLH(&m_matLocal, pEye, pAt, pUp);
	}

	void cCameraNode::SetPerspective(float fovy, float zn, float zf, float ScreenWidth, float ScreenHeight)
	{
		m_FOV = fovy;
		m_ScreenWidth = ScreenWidth;
		m_ScreenHeight = ScreenHeight;
		m_Near = zn;
		m_Far = zf;
		m_bProjectionModified = true;
	}




	void cCameraNode::SetScreenWidthHeight(float ScreenWidth, float ScreenHeight)
	{
		m_ScreenHeight = ScreenHeight;
		m_ScreenWidth = ScreenWidth;
		m_bProjectionModified = true;
	}

	void cCameraNode::MakeWorldPickingRay(float ScreenX, float ScreenY, cLine& Output)
	{
		D3DXVECTOR3 posEnd, posStart;
		float x, y;

		x = 2.0f *  ScreenX / m_ScreenWidth - 1.0f;
		y = 2.0f * -ScreenY / m_ScreenHeight + 1.0f;

		posStart = D3DXVECTOR3(x, y, 0.0f);
		posEnd = D3DXVECTOR3(x, y, 1.0f);

		D3DXMATRIX matViewProjectionInv;

		D3DXMatrixInverse(&matViewProjectionInv, NULL, &m_matViewProjection);

		D3DXVec3TransformCoord(&posStart, &posStart, &matViewProjectionInv);

		D3DXVec3TransformCoord(&posEnd, &posEnd, &matViewProjectionInv);
		Output.Make(posStart, posEnd);
	}
}