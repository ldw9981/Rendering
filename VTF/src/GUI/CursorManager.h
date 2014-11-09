#pragma once
//#include "Foundation/Interface.h"
namespace Sophia
{

class CursorManager 
{
public:
	CursorManager(void);
	virtual ~CursorManager(void);

protected:
	HWND m_hWnd;

public:
	static CursorManager* m_pInstance;
	static CursorManager* GetInstance();

public:
	void Initialize(HWND hWnd);
	void Finalize();

	void GetCursorPos(POINT* pPoint);
};

}