#pragma once

class cFramework
{
public:
	cFramework(void);
	virtual ~cFramework(void);
private:

public:
	virtual bool Open()=0;		// 게임 어플리케이션 초기화처리
	virtual void Run()=0;
	virtual void Close()=0;		// 게임 어플리케이션 종료처리


};
