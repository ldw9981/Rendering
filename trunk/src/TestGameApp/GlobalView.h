#pragma once
#include "Scene/View.h"


class cGUIFont;
class cGUIButton;
class cGlobalView :
	public cView
{
public:
	cGlobalView(void);
	virtual ~cGlobalView(void);
private:
	cGUIButton* m_pBtNextScene;
	cGUIButton* m_pBtExit;
	cGUIFont*	m_pFont;
	
public:
	// cScene
	virtual void Open(void* arg);
	virtual void Close();

	virtual void ProcessRender();

	// cINotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
