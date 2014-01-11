#pragma once
class Window
{
public:
	Window(void);
	~Window(void);
public:
	RECT					m_AdjustRect;	 	 
	HWND					m_hWnd;
	WNDCLASS				m_wndclass;			// 메인 윈도우 클래스
	const char*				m_szClassName;		// 윈도우 클래스 등록 이름
	const char*				m_szTitleName;		// 윈도우 타이틀 이름
public:
	// Window관련
	BOOL					ProcessWindowMessage();		//윈도우 메시지 루프
	void					Initialize(RECT& request);
	void					SetTitleName( const char* lpszString );
	void					ScreenToClient(LPRECT lpRect) const;
	void					ClientToScreen(LPRECT lpRect) const;
	void					MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint = TRUE );
	void					MoveWindow(LPCRECT lpRect,BOOL bRepaint = TRUE );	
	DWORD					GetStyle() const;
	DWORD					GetExStyle() const;
	void					Shutdown();
};

