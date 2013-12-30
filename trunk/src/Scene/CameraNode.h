#pragma once
#include "SceneNode.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "Math/CollisionDetector.h"
#include "WinInput/WinInput.h"

// Plane Bit
#define PB_LEFT			0x0001
#define PB_RIGHT		0x0002
#define PB_TOP			0x0004
#define PB_BOTTOM		0x0008
#define PB_NEAR			0x000F
#define PB_FAR			0x0010

class cPlane;
class cSceneRoot;
class cLine;
class cSphere;
class cRendererQueue;
class cCameraNode :
	public cSceneNode,
	public IControlable,
	private StaticD3DDEVICE9,
	private cStaticWinInput
{
public:
	cCameraNode();
	virtual ~cCameraNode();

	// Plane Number
	enum PLANE_NUM { PN_LEFT,PN_RIGHT,PN_TOP,PN_BOTTOM,PN_NEAR,PN_FAR};	
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
//	BOOL				m_bViewModified;
	bool				m_bProjectionModified;	
	
	cPlane*				m_pWorldFrustumPlane;	
	static cCameraNode*	m_pActiveCamera;
	
public:


	virtual void		Update(DWORD elapseTime);	// 카메라 정보 메트릭스로 뷰변환매트릭스를 만든다.
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
	void				MakeWorldFrustum();

	cCollision::STATE					CheckWorldFrustum(cSphere& sphere,WORD plane);
	cCollision::STATE					CheckWorldFrustum(cSphere& sphere);
	cCollision::STATE					CheckWorldFrustumWithoutYAxis(cSphere& sphere);
	BOOL				InsideWorldFrustum(D3DXVECTOR3& pos);
	const D3DXMATRIX&	GetMatViewProjection() { return m_matViewProjection; }
	const D3DXMATRIX&	GetMatView() { return m_matView; }
	const D3DXMATRIX&	GetMatProjection() { return m_matProjection; }
	cPlane&				GetWorldFrustumPlane(int side);
	void				SetActive();	
	bool GetProcessInput() const { return m_bProcessInput; }
	void SetProcessInput(bool val) { m_bProcessInput = val; }

	static 	cCameraNode* GetActiveCamera()  { return m_pActiveCamera; }
	static void			SetActiveCamera(cCameraNode* val) { m_pActiveCamera = val; }	
};
