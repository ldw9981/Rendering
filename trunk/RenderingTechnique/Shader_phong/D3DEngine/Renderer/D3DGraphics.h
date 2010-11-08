#pragma once
#include "../Common/Interface.h"
#include "../Framework/StaticHWND.h"
#include "StaticD3DDevice9.h"
#include "../Resource/ResourceMng.h"


inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

class cCamera;
class cD3DGraphics;
class cStaticD3DGraphics
{
public:
	cStaticD3DGraphics(void) {};
	~cStaticD3DGraphics(void) {};
protected:
	static cD3DGraphics* m_pGraphics;	
};

class cD3DGraphics :
	public cStaticD3DDEVICE9,
	public cStaticD3DGraphics,
	public cStaticHWND,
	public cStaticResourceMng,
	public IUnknownObject,
	public cIRenderable,
	public cIProgressable
	
{
public:
	cD3DGraphics();
	~cD3DGraphics(void);
	
private:
	// D3D 변수추가	
	D3DPRESENT_PARAMETERS m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9			m_pD3D9;
	LPD3DXFONT			m_pFont;
	wstring 			m_DebugString;
	cCamera*			m_pDefaultCamera;

	BOOL				m_bUsingDefaultCamera;

	
public:
	// D3D 초기화함수 
	void				InitD3D();
	void				InitFont();
	void				RenderDebugString();
	void				AddDebugString(LPCTSTR format, ...);
	void				ClearDebugString();
	void				SetupLight();

	// Render함수
	void				RenderStart();
	void				RenderEnd();
	
	
	
	//Set/Get
	cCamera*			GetDefaultCamera();	
	BOOL	GetUsingDefaultCamera() const { return m_bUsingDefaultCamera; }
	void	SetUsingDefaultCamera(BOOL val) { m_bUsingDefaultCamera = val; }

	//가상함수
	virtual void		Update();
	virtual void		Render();

};
