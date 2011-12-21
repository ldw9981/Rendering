#pragma once

#include "../Common/Interface.h"
#include "D3DGraphics.h"

class cPlane;
class cSphere;
class cLine;
class cScene;
class cCamera :
	private cStaticD3DDEVICE9,
	private cStaticD3DGraphics,	
	public IUnknownObject,
	public cIProgressable,
	public cIRenderable
	
{
public:
	cCamera();
	virtual ~cCamera();
	
	enum FRUSTUM_PLANE { PLANE_LEFT,PLANE_RIGHT,PLANE_TOP,PLANE_BOTTOM,PLANE_NEAR,PLANE_FAR};
private:	

	D3DXMATRIX  m_matRotation;
	D3DXMATRIX	m_matProjection;
	
	
	D3DXMATRIX	m_matView;
	
	
	D3DXMATRIX	m_matCamera;
	D3DXMATRIX  m_matCameraBackup;
	D3DXMATRIX	m_matViewProjection;
	D3DXMATRIX	m_matViewProjectionInv;

	float		m_FOV;
	float		m_Near;
	float		m_Far;
	float		m_ScreenWidth;
	float		m_ScreenHeight;

	BOOL		m_bModifiedCamera;
	BOOL		m_bModifiedProjection;
	BOOL		m_bValidViewProjectionInv;
	int			m_NumOutSide;

	cScene*	m_pAttachedModel;
	cPlane*		m_pWorldFrustumPlane;

	
public:

	// cObject
	virtual void	Init();
	virtual void	Uninit();

	virtual void	Update();	// ī�޶� ���� ��Ʈ������ �亯ȯ��Ʈ������ �����.
	virtual void	Render();
	

	void			MakeWorldPickingRay(float ScreenX,float ScreenY,cLine& Output);
	D3DXMATRIX&		GetMatProjection()  { return m_matProjection; }
	D3DXMATRIX&		GetMatView()  { return m_matView; }

	// ī�޶� ���� ��Ʈ������ �����.
	void			SetLookAt(const D3DXVECTOR3 * pEye,const D3DXVECTOR3 * pAt,const D3DXVECTOR3 * pUp);

	// Make Projection Matrix
	void			SetPerspective(float fovy,float zn,float zf,float ScreenWidth,float ScreenHeight);


	// ī�޶� �̵���Ų��.
	void			MoveOnLocalAxis( float deltaX,float deltaY,float deltaZ );
	void			RotateByLocalAxis( float angleX,float angleY,float angleZ );

	D3DXMATRIX		GetMatCamera() const { return m_matCamera; }
	void			SetMatCamera(D3DXMATRIX* val) {  memcpy(&m_matCamera,val,sizeof(m_matCamera)); m_bModifiedCamera=TRUE; }


	void			SetFOV(float val) { m_FOV = val; m_bModifiedProjection=TRUE; }

	void			SetScreenWidthHeight(float ScreenWidth,float ScreenHeight );
	
	void			SetNear(float val) { m_Near = val; m_bModifiedProjection=TRUE;}

	void			SetFar(float val) { m_Far = val; m_bModifiedProjection=TRUE; }

	void			MakeWorldFrustum();
	BOOL			CullWorldFrustum(cSphere& Sphere);
	cPlane&			GetWorldFrustumPlane(int side);

	void			AttachToModel(cScene* pModel);
	void			DettachToModel();
	BOOL			IsAttached();	
};