#pragma once
class Window
{
public:
	Window(void);
	~Window(void);
public:
	RECT					m_AdjustRect;	 	 
	HWND					m_hWnd;
	WNDCLASS				m_wndclass;			// ���� ������ Ŭ����
	const char*				m_szClassName;		// ������ Ŭ���� ��� �̸�
	const char*				m_szTitleName;		// ������ Ÿ��Ʋ �̸�
public:
	// Window����
	BOOL					ProcessWindowMessage();		//������ �޽��� ����
	void					Initialize(RECT& request);
	void					SetTitleName( const char* lpszString );
	void					ScreenToClient(LPRECT lpRect) const;
	void					ClientToScreen(LPRECT lpRect) const;
	void					MoveWindow(int x,int y,int nWidth,int nHeight,BOOL bRepaint = TRUE );
	void					MoveWindow(LPCRECT lpRect,BOOL bRepaint = TRUE );	
	DWORD					GetStyle() const;
	DWORD					GetExStyle() const;
	void					Finalize();
};

