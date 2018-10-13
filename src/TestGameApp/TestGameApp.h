#pragma once
#include "./Framework/D3DFramework.h"



class TestGameApp :
	public Sophia::cD3DFramework
{
public:
	TestGameApp(const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight);
	virtual ~TestGameApp(void);
public:
	virtual bool Initialize();
	virtual	void Finalize();	

	virtual BOOL OnWM_Keyboard(MSG& msg);
	virtual BOOL OnWM_Mouse(MSG& msg);
	virtual BOOL OnWM_General(MSG& msg);
	virtual void Control();
};
