#pragma once
#include "./Framework/D3DFramework.h"


class cTestScene;
class cMenuScene;
class cGlobalScene;

class cTestGameApp :
	public cD3DFramework
{
public:
	cTestGameApp(LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight);
	virtual ~cTestGameApp(void);
public:
	
	cMenuScene*	m_pMenuScene;
	cTestScene*	m_pTestScene;
	cGlobalScene* m_pGlobalScene;

public:
	// //cD3DFramework
	virtual void OnInitApplication();
	virtual	void OnCloseApplication();
	

	virtual BOOL OnWM_Keyboard(MSG& msg);
	virtual BOOL OnWM_Mouse(MSG& msg);
	virtual BOOL OnWM_General(MSG& msg);
};
