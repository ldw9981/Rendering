#pragma once
#include "./Framework/D3DFramework.h"


class cTestView;
class cMenuView;
class cGlobalView;

class cTestGameApp :
	public cD3DFramework
{
public:
	cTestGameApp(const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight);
	virtual ~cTestGameApp(void);
public:
	
	cMenuView*	m_pMenuScene;
	cTestView*	m_pTestScene;
	cGlobalView* m_pGlobalScene;

public:
	// //cD3DFramework
	virtual bool Open();
	virtual	void Close();
	

	virtual BOOL OnWM_Keyboard(MSG& msg);
	virtual BOOL OnWM_Mouse(MSG& msg);
	virtual BOOL OnWM_General(MSG& msg);
};
