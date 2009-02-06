#pragma once
#include "../Common/Interface.h"
#include "../SceneGraph/CameraNode.h"
#include "../Framework/StaticD3DDevice9.h"
#include "../Input/WinInput.h"
#include "TopRenderable.h"

class cCameraNode;
class cScene:
	public IUnknownObject,
	public cIUpdatable,
	public cIRenderable,
	public cIControlable,
	public cINotifiable,
	private cStaticD3DDEVICE9,
	public cStaticWinInput
{
public:
	cScene(void);
	cScene(cScene* pParent);
	virtual ~cScene(void);
private:
	list<cIRenderable*>		m_RenderableList;
	list<cIUpdatable*>		m_ProgressableList;
	list<cIControlable*>	m_ControlableList;

	cScene*					m_pParentScene;				// �θ� ��
	cTopRenderable			m_TopRenderable;

	BOOL					m_bHide;					//�Է��� �����ʰ� �׸����ʴ´�.	������ �Ѵ�.
	
	D3DVIEWPORT9			m_ViewPortInfo;
	
public:
	
public:
	D3DVIEWPORT9&			GetViewPortInfo()  { return m_ViewPortInfo; }
	void					SetViewPortInfo(UINT x,UINT y,UINT width,UINT height);
	void					SetViewPortInfo(D3DVIEWPORT9& val) { m_ViewPortInfo = val; }
	void					SetViewPort();

	BOOL					GetHide() const { return m_bHide; }
	void					SetHide(BOOL val) { m_bHide = val; }
	void					AttachObject(IUnknownObject* pIUnknownObject);
	void					DettachObject(IUnknownObject* pIUnknownObject);

	void					ProcessControlableList();
	void					ProcessRenderableList();
	void					ProcessProgressableList(DWORD elapse);	

	virtual void			Open(void* arg)=0;
	virtual void			Close()=0;

	// cIRenderable
	virtual void			ProcessRender();
	// cIUpdatable
	virtual void			Update(DWORD elapseTime);

	virtual void			Control();

};
