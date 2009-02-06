#pragma once
#include "SceneGraphNode.h"
#include "../Framework/StaticD3DDevice9.h"

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
	public cSceneGraphNode,
	private cStaticD3DDEVICE9
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
	

	BOOL				m_bViewModified;
	BOOL				m_bProjectionModified;	
	
	cPlane*				m_pWorldFrustumPlane;	
	static cCameraNode*	m_pActiveCamera;
	
public:
	// cObject
	virtual void		Init();
	virtual void		Uninit();

	virtual void		Update(DWORD elapseTime);	// 카메라 정보 메트릭스로 뷰변환매트릭스를 만든다.
	virtual void		Render();
	



	void				SetLookAt(const D3DXVECTOR3 * pEye,const D3DXVECTOR3 * pAt,const D3DXVECTOR3 * pUp);

	// Make Projection Matrix
	void				SetPerspective(float fovy,float zn,float zf,float ScreenWidth,float ScreenHeight);
	void				SetFOV(float val) { m_FOV = val; m_bProjectionModified=TRUE; }
	void				SetScreenWidthHeight(float ScreenWidth,float ScreenHeight );	
	void				SetNear(float val) { m_Near = val; m_bProjectionModified=TRUE;}
	void				SetFar(float val) { m_Far = val; m_bProjectionModified=TRUE; }

	void				MakeWorldPickingRay(float ScreenX,float ScreenY,cLine& Output);
	void				MakeWorldFrustum();

	int					CheckWorldFrustum(cSphere* pSphere,WORD plane);
	int					CheckWorldFrustum(cSphere* pSphere);
	int					CheckWorldFrustumWithoutYAxis(cSphere* pSphere);
	BOOL				InsideWorldFrustum(D3DXVECTOR3& pos);
	cPlane&				GetWorldFrustumPlane(int side);
	void				SetActive();	
	

	static 	cCameraNode* GetActiveCamera()  { return m_pActiveCamera; }
	static void			SetActiveCamera(cCameraNode* val) { m_pActiveCamera = val; }	
};
