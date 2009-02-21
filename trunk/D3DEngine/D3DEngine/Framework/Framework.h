#pragma once

class cFramework
{
public:
	cFramework(void);
	virtual ~cFramework(void);
private:

public:
	virtual void InitApplication() PURE;		// 게임 어플리케이션 초기화처리
	virtual void Run() PURE;
	virtual void CloseApplication() PURE;		// 게임 어플리케이션 종료처리


};
