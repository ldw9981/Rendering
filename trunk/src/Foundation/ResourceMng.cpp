#include "../Framework/interface.h"
#include "../Foundation/SharedTemplate.h"
#include "../Foundation/Trace.h"
#include "ResourceMng.h"
#include "Resource.h"
#include "../D3D9/Texture.h"
#include "../D3D9/VertexBuffer.h"
#include "../D3D9/IndexBuffer.h"
#include "../D3D9/TransformAnm.h"



ResourceMng::ResourceMng( void )
{

}

ResourceMng::~ResourceMng( void )
{
	
	m_mapResource.clear();
}



BOOL ResourceMng::InsertResource(cResource* in )
{
	in->ProcessMakeUniqueKey();
	if( m_mapResource.insert(make_pair(in->GetUniqueKey(),in)).second )
		return TRUE;

	return FALSE;
}

void ResourceMng::RestoreAll()
{
	list<IRestore*>::iterator it=m_listRestore.begin();	
	for ( ;it!=m_listRestore.end();++it)
	{
		IRestore* pItem=*it;
		pItem->Restore();
	}	
}



void ResourceMng::EraseResource(const wstring& strKey )
{
	m_mapResource.erase(strKey);
}



// 파일명이 찾은후 없으면 생성
// NULL이면 생성 

Texture* ResourceMng::CreateTexture( LPCWSTR szFilePath)
{	
	Texture* pItem=NULL;
	wstring strUniqeKey;	
	if (szFilePath!=NULL)
	{
		strUniqeKey += L"TEXTURE_";
		strUniqeKey += szFilePath;
		map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<Texture*>(it->second);
			return pItem;
		}
	}
	pItem = new Texture;
	pItem->SetFilePath(szFilePath);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;	
}

/*
Unique 번호 찾은후 없으면 생성
입력번호NULL이면 생성
*/


Texture* ResourceMng::CreateTexture( UINT nUniqueNumber)
{
	Texture* pItem=NULL;
	wstring strFindKey;	
	if (nUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"TEXTURE_%d",nUniqueNumber);	
		strFindKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strFindKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<Texture*>(it->second);
			return pItem;
		}
	}

	pItem = new Texture;	
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;		
}



VertexBuffer* ResourceMng::CreateVertexBuffer( DWORD bufferSize,D3DPOOL type, UINT nUniqueNumber)
{
	VertexBuffer* pItem=NULL;
	wstring strFindKey;	
	// nUniqueNumber가 있고 맵에 있는 키이면 있는것으로 리턴한다.
	if (nUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"VERTEXBUFFER_%d",nUniqueNumber);	
		strFindKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strFindKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<VertexBuffer*>(it->second);
			return pItem;
		}
	}

	pItem = new VertexBuffer;
	pItem->SetBufferSize(bufferSize);
	pItem->SetType(type);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;
}



IndexBuffer* ResourceMng::CreateIndexBuffer( DWORD bufferSize,D3DPOOL type, UINT nUniqueNumber )
{
	IndexBuffer* pItem=NULL;
	wstring strFindKey;	
	if (nUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"INDEXBUFFER_%d",nUniqueNumber);	
		strFindKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strFindKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<IndexBuffer*>(it->second);
			return pItem;
		}
	}

	pItem = new IndexBuffer;	
	pItem->SetBufferSize(bufferSize);
	pItem->SetType(type);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;
}




TransformAnm* ResourceMng::CreateTransformAnm( UINT findUniqueNumber )
{
	TransformAnm* pItem=NULL;
	wstring strUniqeKey;	
	if (findUniqueNumber!=0)
	{
		WCHAR Buffer[256]={0,};
		wsprintf(Buffer,L"TRANSFORMANM_%d",findUniqueNumber);	
		strUniqeKey = Buffer;

		map<wstring,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<TransformAnm*>(it->second);
			return pItem;
		}
	}

	pItem = new TransformAnm;	
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;		
}

ResourceMng* ResourceMng::Instance()
{
	static ResourceMng ResourceMng;
	return &ResourceMng;
}


IMPLEMENT_SHAREDCLASS(ResourceMng);