#pragma once
#include "./Scene/Scene.h"


class cGUIFont;
class cGUIButton;
class cGlobalScene :
	public cScene
{
public:
	cGlobalScene(void);
	virtual ~cGlobalScene(void);
private:
	cGUIButton* m_pBtNextScene;
	cGUIButton* m_pBtExit;
	cGUIFont*	m_pFont;
public:
	// cScene
	virtual void Open(void* arg);
	virtual void Close();

	virtual void ProcessRender();

	// INotifiable
	virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
