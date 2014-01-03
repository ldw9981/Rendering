#include "stdafx.h"
#include "Foundation/interface.h"
#include "Foundation/Trace.h"
#include "ResourceMng.h"
#include "Resource.h"
#include "D3D9Server/RscTexture.h"
#include "D3D9Server/RscVertexBuffer.h"
#include "D3D9Server/RscIndexBuffer.h"
#include "D3D9Server/RscTransformAnm.h"


cResourceMng* cStaticResourceMng::m_pResourceMng;

cResourceMng::cResourceMng( void )
{
	cStaticResourceMng::m_pResourceMng = this;
}

cResourceMng::~cResourceMng( void )
{
	
	m_mapResource.clear();
}



BOOL cResourceMng::InsertResource(cResource* in )
{
	if( m_mapResource.insert(make_pair(in->GetUniqueKey(),in)).second )
		return TRUE;

	return FALSE;
}

void cResourceMng::RestoreAll()
{
	std::list<cIRestore*>::iterator it=m_listRestore.begin();	
	for ( ;it!=m_listRestore.end();++it)
	{
		cIRestore* pItem=*it;
		pItem->Restore();
	}	
}



void cResourceMng::EraseResource(const std::string& strKey )
{
	m_mapResource.erase(strKey);
}



// 파일명이 찾은후 없으면 생성
// NULL이면 생성 

void cResourceMng::GetKeyTexture(std::string& key, const char* filePath )
{
	char fileName[256];
	_splitpath_s(filePath,NULL,0,NULL,0,fileName,256,NULL,0);
	key = std::string("TEXTURE_");
	key += fileName;
}

cRscTexture* cResourceMng::CreateRscTexture( const char* filePath )
{	
	cRscTexture* pItem=NULL;
	std::string strKey;
	GetKeyTexture(strKey,filePath);	

	std::map<std::string,cResource*>::iterator it=m_mapResource.find(strKey);
	if (it!=m_mapResource.end())
	{
		pItem=static_cast<cRscTexture*>(it->second);
		return pItem;
	}

	pItem = new cRscTexture;
	pItem->SetUniqueKey(strKey);
	pItem->SetFilePath(filePath);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(strKey,pItem));
	return pItem;	
}


void cResourceMng::GetKeyVertexBuffer( std::string& key, const char* rootName,const char* meshName )
{
	key = std::string("VERTEXBUFFER_");
	key += rootName;
	key += std::string("_");
	key += meshName;
}

cRscVertexBuffer* cResourceMng::CreateRscVertexBuffer(const char* rootName,const char* meshName, DWORD bufferSize,D3DPOOL type/*D3DPOOL_DEFAULT*/ )
{
	cRscVertexBuffer* pItem=NULL;
	std::string strKey;	
	GetKeyVertexBuffer(strKey,rootName,meshName);

	std::map<std::string,cResource*>::iterator it=m_mapResource.find(strKey);
	if (it!=m_mapResource.end())
	{
		pItem=static_cast<cRscVertexBuffer*>(it->second);
		return pItem;
	}	

	pItem = new cRscVertexBuffer;
	pItem->SetBufferSize(bufferSize);
	pItem->SetType(type);
	pItem->SetUniqueKey(strKey);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(strKey,pItem));
	return pItem;
}

void cResourceMng::GetKeyIndexBuffer( std::string& key, const char* rootName,const char* meshName )
{
	key = std::string("INDEXBUFFER_");
	key += rootName;
	key += std::string("_");
	key += meshName;
}

cRscIndexBuffer* cResourceMng::CreateRscIndexBuffer(const char* rootName,const char* meshName, DWORD bufferSize,D3DPOOL type )
{
	cRscIndexBuffer* pItem=NULL;
	std::string strKey;	
	GetKeyIndexBuffer(strKey,rootName,meshName);

	std::map<std::string,cResource*>::iterator it=m_mapResource.find(strKey);
	if (it!=m_mapResource.end())
	{
		pItem=static_cast<cRscIndexBuffer*>(it->second);
		return pItem;
	}	

	pItem = new cRscIndexBuffer;	
	pItem->SetBufferSize(bufferSize);
	pItem->SetType(type);
	pItem->SetUniqueKey(strKey);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(strKey,pItem));
	return pItem;
}


void cResourceMng::GetKeyTransformAnm( std::string& key,const char* rootName,const char* meshName,const char* anmName )
{
	key = std::string("TRANSFORMANM_");
	key += rootName;
	key += std::string("_");
	key += meshName;
	key += std::string("_");
	key += anmName;
}

cRscTransformAnm* cResourceMng::CreateRscTransformAnm(const char* rootName,const char* meshName,const char* anmName)
{
	assert(rootName!=NULL);
	assert(meshName!=NULL);
	assert(anmName!=NULL);

	cRscTransformAnm* pItem=NULL;
	std::string strKey;	
	GetKeyTransformAnm(strKey,rootName,meshName,anmName);

	std::map<std::string,cResource*>::iterator it=m_mapResource.find(strKey);
	if (it!=m_mapResource.end())
	{
		pItem=static_cast<cRscTransformAnm*>(it->second);
		return pItem;
	}

	pItem = new cRscTransformAnm;	
	pItem->SetUniqueKey(strKey);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_mapResource.insert(make_pair(strKey,pItem));
	return pItem;		
}

int cResourceMng::GetCount()
{
	return m_mapResource.size();
}




