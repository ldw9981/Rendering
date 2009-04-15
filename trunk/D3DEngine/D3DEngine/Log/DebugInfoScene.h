#include "../Scene/Scene.h"
#include "../Common/Interface.h"
#include "../Input/WinInput.h"

class cGUIFont;
class cDebugInfoScene:
	public cScene	
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
	
	// INotifiable
	virtual void		Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam);
};