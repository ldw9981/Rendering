#pragma once
#include "./Framework/D3DFramework.h"
#include "./Renderer/D3DGraphics.h"



class cObjDragon;
class cGround;
class cBackWall;

class cTestGameApp :
	public cD3DFramework
{
public:
	cTestGameApp(LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight);
	virtual ~cTestGameApp(void);
public:
	
	cObjDragon*	m_pDragon;
	cGround*	m_pGround;
	cBackWall*	m_pBackWall;



public:
	// //cD3DFramework
	virtual void OnInitApplication();
	virtual	void OnCloseApplication();
	

	virtual BOOL OnWM_Keyboard(MSG& msg);
	virtual BOOL OnWM_Mouse(MSG& msg);
	virtual BOOL OnWM_General(MSG& msg);
};
