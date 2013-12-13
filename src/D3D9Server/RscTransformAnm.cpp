#include "stdafx.h"
#include "RscTransformAnm.h"
#include "Foundation/Define.h"

cRscTransformAnm::cRscTransformAnm()
:m_dwTimeLength(0),
m_dwTimeAccum(0)
{
	
}

cRscTransformAnm::~cRscTransformAnm()
{

}

BOOL cRscTransformAnm::Create()
{

	return TRUE;
}

void cRscTransformAnm::Free()
{
//	for_each(m_arrayTransformAnm.begin(),m_arrayTransformAnm.end(),FuncDeleteType<TRANSFORMANM*>);
	m_ResourceMng.EraseResource(GetUniqueKey());
	delete this;
}

void cRscTransformAnm::ProcessMakeUniqueKey()
{
	// �����̸��� ������ ���ξ�_�����̸�
	// ������ ���ξ�_����ũ��ȣ
	std::string temp="TRANSFORMANM_";	

	if (!GetFilePath().empty())
	{			
		temp += GetFilePath();		
	}
	else
	{
		char buffer[4];
		_itoa_s(GetUniqueNumber(),buffer,4,10);
		temp += buffer;
	}
	SetUniqueKey(temp);
}



D3DXMATRIX& cRscTransformAnm::GetTransform( DWORD dwTimeDelta )
{
	m_dwTimeAccum += dwTimeDelta;
	m_dwTimeAccum %= m_dwTimeLength;

	float fIndexRate = (float)m_dwTimeAccum / (float)m_dwTimeLength;	
	int nIndex = (int)((float)m_arrayANMKEY.size() * fIndexRate);
	int nIndexPrev=nIndex;
	int nIndexAfter=nIndex;

	// �����ε����� �ð��� ��ǥ����  ũ�� �ε��� -1
	if(m_dwTimeAccum < m_arrayANMKEY[nIndex].AnmTick)
	{
		if(nIndex>0)	nIndexPrev--;
	}
	else if( m_dwTimeAccum >= m_arrayANMKEY[nIndex].AnmTick) 
	{
		if (nIndex<(int)m_arrayANMKEY.size()-1)		nIndexAfter++;
	}

	float fValue=GetInterpolateValue(m_arrayANMKEY[nIndexPrev].AnmTick,m_arrayANMKEY[nIndexAfter].AnmTick,m_dwTimeAccum);
	
	ANMKEY stTempAnmKey=m_arrayANMKEY[nIndex];
	
	// Todo: scale�� ������
	D3DXQuaternionSlerp(&stTempAnmKey.RotationAccum,&m_arrayANMKEY[nIndexPrev].RotationAccum,&m_arrayANMKEY[nIndexAfter].RotationAccum,fValue);
	D3DXVec3Lerp(&stTempAnmKey.TranslationAccum,&m_arrayANMKEY[nIndexPrev].TranslationAccum,&m_arrayANMKEY[nIndexAfter].TranslationAccum,fValue);

	// �����п�����  TM���ϱ�
	D3DXMatrixScaling(&m_tmSCL,
			stTempAnmKey.ScaleAccum.x,
			stTempAnmKey.ScaleAccum.y,
			stTempAnmKey.ScaleAccum.z);

	D3DXMatrixRotationQuaternion(&m_tmROT,
			&stTempAnmKey.RotationAccum);					

	D3DXMatrixTranslation(&m_tmPOS,
			stTempAnmKey.TranslationAccum.x,
			stTempAnmKey.TranslationAccum.y,
			stTempAnmKey.TranslationAccum.z);			

	// TM	
	m_tmAnm = m_tmSCL * m_tmROT * m_tmPOS;		
	return m_tmAnm;
}

float cRscTransformAnm::GetInterpolateValue( int start_time,int end_time,int inter_time )
{
	float ret;
	int delta_time,offset_time;
	delta_time = end_time - start_time;
	offset_time = inter_time - start_time;

	if (delta_time==0)
	{
		return 0.0f;
	}

	ret=(float)offset_time / (float)delta_time;	
	return ret;
}