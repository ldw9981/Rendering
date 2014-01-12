#include "stdafx.h"
#include "RscTransformAnm.h"
#include "Foundation/Define.h"
#include "Resource/ResourceMng.h"

cRscTransformAnm::cRscTransformAnm()
:m_dwTimeLength(0)
{
	
}

cRscTransformAnm::~cRscTransformAnm()
{

}

BOOL cRscTransformAnm::Create()
{
	assert(m_RefCounter>=0);
	if (m_RefCounter > 0 )
		return TRUE;

	return TRUE;
}

void cRscTransformAnm::Free()
{
//	for_each(m_arrayTransformAnm.begin(),m_arrayTransformAnm.end(),FuncDeleteType<TRANSFORMANM*>);
	cResourceMng::m_pResourceMng->EraseRscTransformAnm(GetUniqueKey());
	delete this;
}




void cRscTransformAnm::GetTransform(D3DXMATRIX& out ,DWORD& animationTime, DWORD dwTimeDelta )
{
	D3DXMATRIX tmSCL;
	D3DXMATRIX tmROT;
	D3DXMATRIX tmPOS;
	

	animationTime += dwTimeDelta;
	animationTime %= m_dwTimeLength;

	float fIndexRate = (float)animationTime / (float)m_dwTimeLength;	
	int nIndex = (int)((float)m_arrayANMKEY.size() * fIndexRate);
	int nIndexPrev=nIndex;
	int nIndexAfter=nIndex;

	// �����ε����� �ð��� ��ǥ����  ũ�� �ε��� -1
	if(animationTime < m_arrayANMKEY[nIndex].AnmTick)
	{
		if(nIndex>0)	nIndexPrev--;
	}
	else if( animationTime >= m_arrayANMKEY[nIndex].AnmTick) 
	{
		if (nIndex<(int)m_arrayANMKEY.size()-1)		nIndexAfter++;
	}

	float fValue=GetInterpolateValue(m_arrayANMKEY[nIndexPrev].AnmTick,m_arrayANMKEY[nIndexAfter].AnmTick,animationTime);
	
	ANMKEY stTempAnmKey=m_arrayANMKEY[nIndex];
	
	// Todo: scale�� ������
	D3DXQuaternionSlerp(&stTempAnmKey.RotationAccum,&m_arrayANMKEY[nIndexPrev].RotationAccum,&m_arrayANMKEY[nIndexAfter].RotationAccum,fValue);
	D3DXVec3Lerp(&stTempAnmKey.TranslationAccum,&m_arrayANMKEY[nIndexPrev].TranslationAccum,&m_arrayANMKEY[nIndexAfter].TranslationAccum,fValue);

	// �����п�����  TM���ϱ�
	D3DXMatrixScaling(&tmSCL,
			stTempAnmKey.ScaleAccum.x,
			stTempAnmKey.ScaleAccum.y,
			stTempAnmKey.ScaleAccum.z);

	D3DXMatrixRotationQuaternion(&tmROT,
			&stTempAnmKey.RotationAccum);					

	D3DXMatrixTranslation(&tmPOS,
			stTempAnmKey.TranslationAccum.x,
			stTempAnmKey.TranslationAccum.y,
			stTempAnmKey.TranslationAccum.z);			

	// TM	
	out = tmSCL * tmROT * tmPOS;		
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