#pragma once
#include "SceneNode.h"
#include "Math/CollisionDetector.h"
#include "Math/Frustum.h"

namespace Sophia
{


class cPlane;
class cSceneRoot;
class cLine;
class cSphere;
class cRendererQueue;
class cCameraNode :
	public cSceneNode,
	public IControlable
{
public:
	cCameraNode();
	virtual ~cCameraNode();
private:	

	D3DXMATRIX			m_matProjection;
	D3DXMATRIX			m_matView;		
	D3DXMATRIX			m_matViewProjection;
	
	
	D3DXMATRIX			m_matViewProjectionInv;

	float				m_FOV;
	float				m_Near;
	float				m_Far;
	float				m_ScreenWidth;
	float				m_ScreenHeight;
	
	bool				m_bProcessInput;
	bool				m_bProjectionModified;	
	
	Frustum				m_frustum;
	
public:

	virtual void		Render();
	virtual void		Control();


	void				SetLookAt(const D3DXVECTOR3 * pEye,const D3DXVECTOR3 * pAt,const D3DXVECTOR3 * pUp);

	// Make Projection Matrix
	void				SetPerspective(float fovy,float zn,float zf,float ScreenWidth,float ScreenHeight);
	void				SetFOV(float val) { m_FOV = val; m_bProjectionModified=true; }
	void				SetScreenWidthHeight(float ScreenWidth,float ScreenHeight );	
	void				SetNear(float val) { m_Near = val; m_bProjectionModified=true;}
	void				SetFar(float val) { m_Far = val; m_bProjectionModified=true; }
	void				MakeWorldPickingRay(float ScreenX,float ScreenY,cLine& Output);

	float				GetNear() const { return m_Near; }
	float				GetFar() const { return m_Far; }
	const D3DXMATRIX&	GetMatViewProjection() { return m_matViewProjection; }
	const D3DXMATRIX&	GetMatView() { return m_matView; }
	const D3DXMATRIX&	GetMatProjection() { return m_matProjection; }
	Frustum&			GetFrustum()  { return m_frustum; }

	bool GetProcessInput() const { return m_bProcessInput; }
	void SetProcessInput(bool val) { m_bProcessInput = val; }
	virtual void		UpdateLocalMatrix();
};

}