#include "TransformAnm.h"

#include "../Foundation/Define.h"
#include <algorithm>
#include "../Foundation/Resource.h"
using namespace std;

TransformAnm::TransformAnm()
{

}

TransformAnm::~TransformAnm()
{

}

BOOL TransformAnm::Create()
{

	return TRUE;
}

void TransformAnm::Free()
{
	for_each(m_arrayTransformAnm.begin(),m_arrayTransformAnm.end(),FuncDeleteType<TRANSFORMANM*>);
	m_SharedResourceMng.EraseResource(GetUniqueKey());
	delete this;
}

void TransformAnm::ProcessMakeUniqueKey()
{
	// 파일이름이 있으면 접두어_파일이름
	// 없으면 접두어_유니크번호
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
