#pragma once
#include "Framework/D3DFramework.h"

class Framework : public Sophia::cD3DFramework
{
public:
	Framework(const char* szTitleName,BOOL bFullScreen,int nWidth,int nHeight);
	
protected:	
	virtual void InitWindow();
public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Finalize();
};

