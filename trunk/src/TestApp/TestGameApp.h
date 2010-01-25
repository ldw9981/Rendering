#pragma once

#include "../Framework/App/app.h"


class Scene;

namespace Application
{

class TestGameApp :
	public App
{
public:
	TestGameApp(LPCWSTR szTitleName,BOOL bFullScreen,int nWidth,int nHeight);
	virtual ~TestGameApp(void);
public:
	Scene* m_pMenuScene;
	Scene* m_pTestScene;
	Scene* m_pGlobalScene;

	virtual BOOL Open();
	virtual	void Close();
	

	virtual BOOL OnWM_Keyboard(MSG& msg);
	virtual BOOL OnWM_Mouse(MSG& msg);
	virtual BOOL OnWM_General(MSG& msg);
};


}