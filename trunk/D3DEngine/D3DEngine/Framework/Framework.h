#pragma once

class cFramework
{
public:
	cFramework(void);
	virtual ~cFramework(void);
private:

public:
	virtual void InitApplication() PURE;		// ���� ���ø����̼� �ʱ�ȭó��
	virtual void Run() PURE;
	virtual void CloseApplication() PURE;		// ���� ���ø����̼� ����ó��


};
