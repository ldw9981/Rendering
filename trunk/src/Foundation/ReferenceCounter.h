#pragma once

class ReferenceCounter
{
public:
	ReferenceCounter(void) {m_RefCounter=0;}
	~ReferenceCounter(void) {}

	unsigned int		m_RefCounter;
public:
	unsigned int	AddRef();

	unsigned int	Release();	
	virtual void Free()=0;	
};