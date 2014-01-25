#include "stdafx.h"
#include "Foundation/interface.h"
#include "Foundation/Trace.h"
#include "ResourceMng.h"
#include "Resource.h"
#include "Graphics/RscTexture.h"
#include "Graphics/RscVertexBuffer.h"
#include "Graphics/RscIndexBuffer.h"
#include "Graphics/RscTransformAnm.h"

cResourceMng* cResourceMng::m_pResourceMng;

cResourceMng::cResourceMng( void )
{
	m_pResourceMng = this;
}

cResourceMng::~cResourceMng( void )
{
	assert(GetCount()==0);
	m_contTexture.clear();
	m_contIndexBuffer.clear();
	m_contVertexBuffer.clear();
	m_contTransformAnm.clear();
}


// 파일명이 찾은후 없으면 생성
// NULL이면 생성 

void cResourceMng::GetKeyTexture(std::string& key, const char* filePath )
{
	char fileName[256];
	_splitpath_s(filePath,NULL,0,NULL,0,fileName,256,NULL,0);
	key += fileName;
}

cRscTexture* cResourceMng::CreateRscTexture( const char* filePath )
{	
	cRscTexture* pItem=NULL;
	std::string strKey;
	GetKeyTexture(strKey,filePath);	

	std::map<std::string,cResource*>::iterator it=m_contTexture.find(strKey);
	if (it!=m_contTexture.end())
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
	m_contTexture.insert(make_pair(strKey,pItem));
	return pItem;	
}


void cResourceMng::GetKeyVertexBuffer( std::string& key, const char* rootName,const char* meshName )
{
	key = rootName;
	key += std::string("_");
	key += meshName;
}

cRscVertexBuffer* cResourceMng::CreateRscVertexBuffer(const char* rootName,const char* meshName, DWORD bufferSize,D3DPOOL type/*D3DPOOL_DEFAULT*/ )
{
	cRscVertexBuffer* pItem=NULL;
	std::string strKey;	
	GetKeyVertexBuffer(strKey,rootName,meshName);

	std::map<std::string,cResource*>::iterator it=m_contVertexBuffer.find(strKey);
	if (it!=m_contVertexBuffer.end())
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
	m_contVertexBuffer.insert(make_pair(strKey,pItem));
	return pItem;
}

void cResourceMng::GetKeyIndexBuffer( std::string& key, const char* rootName,const char* meshName )
{
	key = rootName;
	key += std::string("_");
	key += meshName;
}

cRscIndexBuffer* cResourceMng::CreateRscIndexBuffer(const char* rootName,const char* meshName, DWORD bufferSize,D3DPOOL type )
{
	cRscIndexBuffer* pItem=NULL;
	std::string strKey;	
	GetKeyIndexBuffer(strKey,rootName,meshName);

	std::map<std::string,cResource*>::iterator it=m_contIndexBuffer.find(strKey);
	if (it!=m_contIndexBuffer.end())
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
	m_contIndexBuffer.insert(make_pair(strKey,pItem));
	return pItem;
}


void cResourceMng::GetKeyTransformAnm( std::string& key,const char* rootName,const char* meshName,const char* anmName )
{
	key = rootName;
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

	std::map<std::string,cResource*>::iterator it=m_contTransformAnm.find(strKey);
	if (it!=m_contTransformAnm.end())
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
	m_contTransformAnm.insert(make_pair(strKey,pItem));
	return pItem;		
}

int cResourceMng::GetCount()
{
	int cnt=0;
	cnt += m_contTexture.size();
	cnt += m_contIndexBuffer.size();
	cnt += m_contVertexBuffer.size();
	cnt += m_contTransformAnm.size();
	return cnt;
}

void cResourceMng::EraseRscTexture( const std::string& strKey )
{
	m_contTexture.erase(strKey);
}

void cResourceMng::EraseRscIndexBuffer( const std::string& strKey )
{
	m_contIndexBuffer.erase(strKey);
}

void cResourceMng::EraseRscVertexBuffer( const std::string& strKey )
{
	m_contVertexBuffer.erase(strKey);
}

void cResourceMng::EraseRscTransformAnm( const std::string& strKey )
{
	m_contTransformAnm.erase(strKey);
}


