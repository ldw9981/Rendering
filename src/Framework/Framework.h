#pragma once

class cFramework
{
public:
	cFramework(void);
	virtual ~cFramework(void);
private:

public:
	virtual bool Open()=0;		// ���� ���ø����̼� �ʱ�ȭó��
	virtual void Run()=0;
	virtual void Close()=0;		// ���� ���ø����̼� ����ó��


};
