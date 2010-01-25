#include "../Scene/Scene.h"
#include "../Framework/Interface.h"
#include "../Framework/Input/WinInput.h"


class cGUIFont;
class cGUIBase;
class cDebugInfoScene:
	public Scene
{
public:
	cDebugInfoScene();
	~cDebugInfoScene();
private:
	cGUIFont*			m_pFont;
	wstring 			m_DebugString;
	BOOL				m_bRender;
public:
	virtual void 		Open(void* arg);
	virtual void 		Close();
	virtual void 		Control();

	virtual void 		Update(DWORD elapseTime);

	virtual void 		ProcessRender();

	void				AddDebugString(LPCTSTR format, ...);
	
	// cINotifiable
	virtual void		Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};
