#pragma once

#include "../Foundation/Resource.h"
#include <d3dx9math.h>
#include <assert.h>
#include <vector>

using namespace std;

struct TRANSFORMANM
{
	DWORD	anmTime;
	D3DXMATRIX TM;
};

class TransformAnm:
	public cResource
{
public:
	TransformAnm();
	~TransformAnm();

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