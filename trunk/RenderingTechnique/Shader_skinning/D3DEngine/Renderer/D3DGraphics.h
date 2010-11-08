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
	private cStaticD3DDEVICE9,
	private cStaticD3DGraphics,
	private cStaticHWND,
	private cStaticResourceMng,
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
	cCamera*			m_pDefaultTopCamera;
	
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
	cCamera*			GetDefaultTopCamera();	
	cResourceMng*		GetResourceMng()	{ return &m_ResourceMng; }

	//가상함수
	virtual void		Update(DWORD elapseTime);
	virtual void		Render();

};
