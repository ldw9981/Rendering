#pragma once
#include "Resource.h"

struct TRANSFORMANM
{
	DWORD	anmTime;
	D3DXMATRIX TM;
};

class cRscTransformAnm:
	public cResource
{
public:
	cRscTransformAnm();
	~cRscTransformAnm();

public:
	vector<TRANSFORMANM*>  m_arrayTransformAnm;
	

public:
	vector<TRANSFORMANM*>&	GetArrayTransformAnm()  { return m_arrayTransformAnm; }
	TRANSFORMANM*			GetTransformAnm(UINT index) 
	{ 
		assert(index<m_arrayTransformAnm.size());
		return m_arrayTransformAnm[index]; 
	}	


	// cResource
	virtual void			ProcessMakeUniqueKey();
	virtual	BOOL			Create();	
	virtual	void			Free();
};