#pragma once
#include "Resource/Resource.h"

struct TRANSFORMANM
{
	DWORD	anmTime;
	D3DXMATRIX TM;
};

struct POSKEY
{
	DWORD AnmTick;
	D3DXVECTOR3 TranslationAccum;
};


struct ROTKEY
{
	DWORD AnmTick;
	D3DXQUATERNION RotationAccum;
};


struct SCLKEY
{
	DWORD AnmTick;
	D3DXVECTOR3 ScaleAccum;
};


struct ANMKEY
{
	DWORD AnmTick;
	D3DXVECTOR3 ScaleAccum;
	D3DXVECTOR3 TranslationAccum;
	D3DXQUATERNION RotationAccum;

	ANMKEY()
		:AnmTick(0),ScaleAccum(0.0f,0.0f,0.0f),TranslationAccum(0.0f,0.0f,0.0f),RotationAccum(0.0f,0.0f,0.0f,0.0f)
	{

	}
};


class cRscTransformAnm:
	public cResource
{
public:
	cRscTransformAnm();
	~cRscTransformAnm();

protected:
	DWORD	m_dwTimeAccum;
public:
	std::vector<ANMKEY> m_arrayANMKEY;

	D3DXMATRIX m_tmSCL;
	D3DXMATRIX m_tmROT;
	D3DXMATRIX m_tmPOS;
	D3DXMATRIX m_tmAnm;
	DWORD	m_dwTimeLength;
	
protected:
	float					GetInterpolateValue( int start_time,int end_time,int inter_time );
public:
	DWORD					GetTimeLength() const { return m_dwTimeLength; }
	void					SetTimeLength(DWORD val) { m_dwTimeLength = val; }
	std::vector<ANMKEY>&			GetArrayANMKEY()  { return m_arrayANMKEY; }
	D3DXMATRIX&				GetTransform(DWORD dwTimeDelta);	
	

	// cResource
	virtual void			ProcessMakeUniqueKey();
	virtual	BOOL			Create();	
	virtual	void			Free();
};