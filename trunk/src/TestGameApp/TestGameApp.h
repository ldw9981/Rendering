#pragma once
#include "./Framework/D3DFramework.h"



class TestGameApp :
	public cD3DFramework
{
public:
	TestGameApp(const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight);
	virtual ~TestGameApp(void);
public:

public:
	virtual bool Open();
	virtual	void Close();	

	virtual BOOL OnWM_Keyboard(MSG& msg);
	virtual BOOL OnWM_Mouse(MSG& msg);
	virtual BOOL OnWM_General(MSG& msg);
};
