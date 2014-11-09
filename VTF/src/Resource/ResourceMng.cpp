#include "stdafx.h"
#include "Foundation/interface.h"
#include "Foundation/Trace.h"
#include "ResourceMng.h"
#include "Resource.h"
#include "Graphics/RscTexture.h"
#include "Graphics/RscVertexBuffer.h"
#include "Graphics/RscIndexBuffer.h"
#include "Graphics/Animation.h"
#include "Graphics/MaterialEx.h"
#include "Graphics/MatrixStreamVertexBuffer.h"
#include "Graphics/IndexStreamVertexBuffer.h"
#include "Graphics/BoneStreamTexture.h"

namespace Sophia
{

cResourceMng* cResourceMng::m_pInstance;

cResourceMng::cResourceMng( void )
{
	m_pInstance = this;
}

cResourceMng::~cResourceMng( void )
{
	assert(GetCount()==0);
	m_contEntityAnimation.clear();
	m_contEntityMaterial.clear();
	m_contTexture.clear();
	m_contIndexBuffer.clear();
	m_contVertexBuffer.clear();	
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

cRscVertexBuffer* cResourceMng::CreateRscVertexBuffer(const char* rootName,const char* meshName, DWORD bufferSize,D3DPOOL type/*D3DPOOL_MANAGED*/ )
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
	pItem->SetPool(type);
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

cRscIndexBuffer* cResourceMng::CreateRscIndexBuffer(const char* rootName,const char* meshName, DWORD bufferSize,D3DPOOL type/*D3DPOOL_MANAGED*/ )
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

int cResourceMng::GetCount()
{
	int cnt=0;
	cnt += m_contTexture.size();
	cnt += m_contIndexBuffer.size();
	cnt += m_contVertexBuffer.size();
	cnt += m_contEntityAnimation.size();
	cnt += m_contEntityMaterial.size();
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

void cResourceMng::GetKeyEntityAnimation( std::string& key,const char* filePath )
{
	char fileName[256];
	_splitpath_s(filePath,NULL,0,NULL,0,fileName,256,NULL,0);
	key += fileName;
}

EntityAnimation* cResourceMng::CreateEntityAnimation( const char* filePath )
{
	EntityAnimation* pItem=NULL;
	std::string strKey;
	GetKeyEntityAnimation(strKey,filePath);	

	auto it=m_contEntityAnimation.find(strKey);
	if (it!=m_contEntityAnimation.end())
	{
		pItem=static_cast<EntityAnimation*>(it->second);
		return pItem;
	}

	pItem = new EntityAnimation;
	pItem->SetUniqueKey(strKey);
	//pItem->SetFilePath(filePath);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_contEntityAnimation.insert(make_pair(strKey,pItem));
	return pItem;	
}

void cResourceMng::EraseEntityAnimation( const std::string& strKey )
{
	m_contEntityAnimation.erase(strKey);
}

void cResourceMng::GetKeyEntityMaterial( std::string& key,const char* filePath )
{
	char fileName[256];
	_splitpath_s(filePath,NULL,0,NULL,0,fileName,256,NULL,0);
	key += fileName;
}

EntityMaterial* cResourceMng::CreateEntityMaterial( const char* filePath )
{
	EntityMaterial* pItem=NULL;
	std::string strKey;
	GetKeyEntityMaterial(strKey,filePath);	

	auto it=m_contEntityMaterial.find(strKey);
	if (it!=m_contEntityMaterial.end())
	{
		pItem=static_cast<EntityMaterial*>(it->second);
		return pItem;
	}

	pItem = new EntityMaterial;
	pItem->SetUniqueKey(strKey);
	//pItem->SetFilePath(filePath);
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_contEntityMaterial.insert(make_pair(strKey,pItem));
	return pItem;	
}

void cResourceMng::EraseEntityMaterial( const std::string& strKey )
{
	m_contEntityMaterial.erase(strKey);
}

MatrixStreamVertexBuffer* cResourceMng::CreateMatrixStreamVertexBuffer( SCENE_KEY& key )
{
	MatrixStreamVertexBuffer* pItem=NULL;
	
	auto it=m_contMatrixStreamVertexBuffer.find(key);
	if (it!=m_contMatrixStreamVertexBuffer.end())
	{
		pItem = static_cast<MatrixStreamVertexBuffer*>(it->second);
		return pItem;
	}	

	pItem = new MatrixStreamVertexBuffer;
	pItem->m_key = key;
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_contMatrixStreamVertexBuffer.insert(std::make_pair(key,pItem));
	return pItem;
}

void cResourceMng::EraseMatrixStreamVertexBuffer( SCENE_KEY& key )
{
	m_contMatrixStreamVertexBuffer.erase(key);
}

IndexStreamVertexBuffer* cResourceMng::CreateIndexStreamVertexBuffer( SCENE_KEY& key )
{
	IndexStreamVertexBuffer* pItem=NULL;

	auto it=m_contIndexStreamVertexBuffer.find(key);
	if (it!=m_contIndexStreamVertexBuffer.end())
	{
		pItem = static_cast<IndexStreamVertexBuffer*>(it->second);
		return pItem;
	}	

	pItem = new IndexStreamVertexBuffer;
	pItem->m_key = key;
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_contIndexStreamVertexBuffer.insert(std::make_pair(key,pItem));
	return pItem;
}

void cResourceMng::EraseIndexStreamVertexBuffer( SCENE_KEY& key )
{
	m_contIndexStreamVertexBuffer.erase(key);
}

BoneStreamTexture* cResourceMng::CreateBoneStreamTexture( SCENE_KEY& key )
{
	BoneStreamTexture* pItem=NULL;

	auto it=m_contBoneStreamTexture.find(key);
	if (it!=m_contBoneStreamTexture.end())
	{
		pItem = static_cast<BoneStreamTexture*>(it->second);
		return pItem;
	}	

	pItem = new BoneStreamTexture;
	pItem->m_key = key;
	if(!pItem->Create())	
	{
		delete pItem;
		return NULL;
	}
	m_contBoneStreamTexture.insert(std::make_pair(key,pItem));
	return pItem;
}

void cResourceMng::EraseBoneStreamTexture( SCENE_KEY& key )
{
	m_contBoneStreamTexture.erase(key);
}




}