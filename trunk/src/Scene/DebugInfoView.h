#pragma once

#include "View.h"
#include "Foundation/Interface.h"
#include "WinInput/WinInput.h"

class cGUIFont;
class cDebugInfoView:
	public cView	
{
public:
	cDebugInfoView();
	~cDebugInfoView();

	static cDebugInfoView* m_pInstance;
private:
	cGUIFont*			m_pFont;
	string 				m_DebugString;
	BOOL				m_bRender;
	

public:
	static cDebugInfoView* GetInstance() { return m_pInstance;}
	virtual void 		Open(void* arg);
	virtual void 		Close();
	virtual void 		Control();

	virtual void 		Update(DWORD elapseTime);

	virtual void 		ProcessRender();

	void				AddDebugString(const char* format, ...);
	
	// cINotifiable
	virtual void		Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};