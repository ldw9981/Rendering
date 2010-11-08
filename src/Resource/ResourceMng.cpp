#include "stdafx.h"
#include "Foundation/interface.h"
#include "Foundation/Trace.h"
#include "ResourceMng.h"
#include "Resource.h"
#include "D3D9Server/RscTexture.h"
#include "D3D9Server/RscVertexBuffer.h"
#include "D3D9Server/RscIndexBuffer.h"
#include "D3D9Server/RscTransformAnm.h"

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
	in->ProcessMakeUniqueKey();
	if( m_mapResource.insert(make_pair(in->GetUniqueKey(),in)).second )
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



void cResourceMng::EraseResource(const string& strKey )
{
	m_mapResource.erase(strKey);
}



// ���ϸ��� ã���� ������ ����
// NULL�̸� ���� 
cRscTexture* cResourceMng::CreateRscTexture( const char* szFilePath)
{	
	cRscTexture* pItem=NULL;
	string strUniqeKey;	
	if (szFilePath!=NULL)
	{
		strUniqeKey += "TEXTURE_";
		strUniqeKey += szFilePath;
		map<string,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
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
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;	
}
/*
Unique ��ȣ ã���� ������ ����
�Է¹�ȣNULL�̸� ����
*/
cRscTexture* cResourceMng::CreateRscTexture( UINT nUniqueNumber/*=0*/)
{
	cRscTexture* pItem=NULL;
	string strFindKey;	
	if (nUniqueNumber!=0)
	{
		char Buffer[256]={0,};
		sprintf_s(Buffer,"TEXTURE_%d",nUniqueNumber);	
		strFindKey = Buffer;

		map<string,cResource*>::iterator it=m_mapResource.find(strFindKey);
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
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;		
}



cRscVertexBuffer* cResourceMng::CreateRscVertexBuffer( DWORD bufferSize,D3DPOOL type/*D3DPOOL_DEFAULT*/, UINT nUniqueNumber/*=0*/)
{
	cRscVertexBuffer* pItem=NULL;
	string strFindKey;	
	// nUniqueNumber�� �ְ� �ʿ� �ִ� Ű�̸� �ִ°����� �����Ѵ�.
	if (nUniqueNumber!=0)
	{
		char Buffer[256]={0,};
		sprintf_s(Buffer,"VERTEXBUFFER_%d",nUniqueNumber);	
		strFindKey = Buffer;

		map<string,cResource*>::iterator it=m_mapResource.find(strFindKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<cRscVertexBuffer*>(it->second);
			return pItem;
		}
	}

	pItem = new cRscVertexBuffer;
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

cRscIndexBuffer* cResourceMng::CreateRscIndexBuffer( DWORD bufferSize,D3DPOOL type, UINT nUniqueNumber/*=0*/ )
{
	cRscIndexBuffer* pItem=NULL;
	string strFindKey;	
	if (nUniqueNumber!=0)
	{
		char Buffer[256]={0,};
		sprintf_s(Buffer,"INDEXBUFFER_%d",nUniqueNumber);	
		strFindKey = Buffer;

		map<string,cResource*>::iterator it=m_mapResource.find(strFindKey);
		if (it!=m_mapResource.end())
		{
			pItem=static_cast<cRscIndexBuffer*>(it->second);
			return pItem;
		}
	}

	pItem = new cRscIndexBuffer;	
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

cRscTransformAnm* cResourceMng::CreateRscTransformAnm( UINT findUniqueNumber/*=0*/ )
{
	cRscTransformAnm* pItem=NULL;
	string strUniqeKey;	
	if (findUniqueNumber!=0)
	{
		char Buffer[256]={0,};
		sprintf_s(Buffer,"TRANSFORMANM_%d",findUniqueNumber);	
		strUniqeKey = Buffer;

		map<string,cResource*>::iterator it=m_mapResource.find(strUniqeKey);
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
	pItem->ProcessMakeUniqueKey();
	m_mapResource.insert(make_pair(pItem->GetUniqueKey(),pItem));
	return pItem;		
}