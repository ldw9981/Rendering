#include "stdafx.h"
#include "../Common/interface.h"
#include "../Log/Trace.h"
#include "ResourceMng.h"
#include "Resource.h"
#include "RscTexture.h"
#include "RscVertexBuffer.h"
#include "RscIndexBuffer.h"
#include "RscTransformAnm.h"

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



void cResourceMng::EraseResource(const wstring& strKey )
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
cRscTexture* cResourceMng::CreateRscTexture( UINT nUniqueNumber/*=0*/)
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



cRscVertexBuffer* cResourceMng::CreateRscVertexBuffer( DWORD bufferSize, UINT nUniqueNumber/*=0*/)
{
	cRscVertexBuffer* pItem=NULL;
	wstring strUniqeKey;	
	if (nUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"VERTEXBUFFER_%d",nUniqueNumber);	
		strUniqeKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<cRscVertexBuffer*>(it->second);
			return pItem;
		}
	}

	pItem = new cRscVertexBuffer;
	pItem->SetBufferSize(bufferSize);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(pItem->GetUniqeKey(),pItem));
	return pItem;
}

cRscIndexBuffer* cResourceMng::CreateRscIndexBuffer( DWORD bufferSize, UINT nUniqueNumber/*=0*/ )
{
	cRscIndexBuffer* pItem=NULL;
	wstring strUniqeKey;	
	if (nUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"INDEXBUFFER_%d",nUniqueNumber);	
		strUniqeKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<cRscIndexBuffer*>(it->second);
			return pItem;
		}
	}

	pItem = new cRscIndexBuffer;	
	pItem->SetBufferSize(bufferSize);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(pItem->GetUniqeKey(),pItem));
	return pItem;
}

cRscTransformAnm* cResourceMng::CreateRscTransformAnm( UINT findUniqueNumber/*=0*/ )
{
	cRscTransformAnm* pItem=NULL;
	wstring strUniqeKey;	
	if (findUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"TRANSFORMANM_%d",findUniqueNumber);	
		strUniqeKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<cRscTransformAnm*>(it->second);
			return pItem;
		}
	}

	pItem = new cRscTransformAnm;	
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(pItem->GetUniqeKey(),pItem));
	return pItem;		
}