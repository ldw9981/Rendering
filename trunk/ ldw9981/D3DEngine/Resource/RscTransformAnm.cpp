#include "stdafx.h"
#include "RscTransformAnm.h"


cRscTransformAnm::cRscTransformAnm()
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
	for_each(m_arrayTransformAnm.begin(),m_arrayTransformAnm.end(),FuncDeleteType<TRANSFORMANM*>);
	GetResourceMng()->EraseResource(GetUniqueKey());
	delete this;
}

void cRscTransformAnm::ProcessMakeUniqueKey()
{
	// �����̸��� ������ ���ξ�_�����̸�
	// ������ ���ξ�_����ũ��ȣ
	wstring temp=L"TRANSFORMANM_";	

	if (!GetFilePath().empty())
	{			
		temp += GetFilePath();		
	}
	else
	{
		WCHAR buffer[4];
		_itow_s(GetUniqueNumber(),buffer,4,10);
		temp += buffer;
	}
	SetUniqueKey(temp);
}
