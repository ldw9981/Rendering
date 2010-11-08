#include "stdafx.h"
#include "../Common/interface.h"
#include "../Log/Trace.h"
#include "ResourceMng.h"
#include "Resource.h"
#include "RscTexture.h"

cResourceMng cStaticResourceMng::m_ResourceMng;

cResourceMng::cResourceMng( void )
{

}

cResourceMng::~cResourceMng( void )
{
	m_mapResource.clear();
}



BOOL cResourceMng::InsertResource(cResource* in )
{
	in->MakeUniqeKey();
	if( m_mapResource.insert(make_pair(in->GetUniqeKey(),in)).second )
		return TRUE;

	return FALSE;
}

void cResourceMng::RestoreAll()
{
	list<cIRestore*>::iterator it=m_listRestore.begin();	
	for ( ;it!=m_listRestore.end();++it)
	{
		cIRestore* pItem=*it;
		pItem->Restore();
	}	
}



void cResourceMng::DeleteResource( wstring& strKey )
{
	m_mapResource.erase(strKey);
}



// 파일명이 찾은후 없으면 생성
// NULL이면 생성 
cRscTexture* cResourceMng::CreateRscTexture( LPCWSTR szFilePath)
{	
	cRscTexture* pItem=NULL;
	wstring strUniqeKey;	
	if (szFilePath!=NULL)
	{
		strUniqeKey += L"TEXTURE_";
		strUniqeKey += szFilePath;
		map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<cRscTexture*>(it->second);
			return pItem;
		}
	}
	pItem = new cRscTexture;
	pItem->SetFilePath(szFilePath);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(pItem->GetUniqeKey(),pItem));
	return pItem;	
}
/*
Unique 번호 찾은후 없으면 생성
입력번호NULL이면 생성
*/
cRscTexture* cResourceMng::CreateRscTexture( UINT nUniqueNumber )
{
	cRscTexture* pItem=NULL;
	wstring strUniqeKey;	
	if (nUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"TEXTURE_%d",nUniqueNumber);	
		strUniqeKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<cRscTexture*>(it->second);
			return pItem;
		}
	}

	pItem = new cRscTexture;	
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(pItem->GetUniqeKey(),pItem));
	return pItem;		
}


//	맵에 있으면 Destroy호출
void cResourceMng::DestroyRscTexture( LPCWSTR szFilePath )
{
	assert(szFilePath!=NULL);
	wstring strUniqueKey;
	strUniqueKey += L"TEXTURE_";
	strUniqueKey += szFilePath;
	map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqueKey);
	cRscTexture* pItem=NULL;
	if (it!=m_mapResource.end())	
	{
		pItem=static_cast<cRscTexture*>(it->second);
		if(pItem->Destroy())
		{	
			m_mapResource.erase(strUniqueKey);			
		}
	}	
}

void cResourceMng::DestroyRscTexture( UINT nUniqueNumber )
{
	assert(nUniqueNumber!=0);
	wstring strUniqueKey;
	WCHAR Buffer[256]={0,};
	wsprintf(Buffer,L"TEXTURE_%d",nUniqueNumber);
	strUniqueKey=Buffer;
	map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqueKey);
	cRscTexture* pItem=NULL;
	if (it!=m_mapResource.end())	
	{
		pItem=static_cast<cRscTexture*>(it->second);
		if(pItem->Destroy())
		{	
			m_mapResource.erase(strUniqueKey);			
		}
	}		
}
