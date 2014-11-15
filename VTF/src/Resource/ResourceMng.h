#pragma once

#include "Restore.h"
#include "Graphics/RendererQueue.h"

namespace Sophia
{

class cResource;
class cIRestore;
class cRscTexture;
class cRscVertexBuffer;
class cRscIndexBuffer;
class cResourceMng;
class EntityAnimation;
class EntityMaterial;
class Material;
class IndexInstancingBuffer;
class MatrixInstancingTexture;
class VertexTexture;
class VertexInstancingBuffer;
class cResourceMng:
	private cStaticRestoreList
{
public:
	cResourceMng(void);
	~cResourceMng(void);
	static cResourceMng* m_pInstance;
private:
	std::map<std::string,cResource*>					m_contTexture;
	std::map<std::string,cResource*>					m_contIndexBuffer;
	std::map<std::string,cResource*>					m_contVertexBuffer;
	std::map<std::string,cResource*>					m_contEntityAnimation;
	std::map<std::string,cResource*>					m_contEntityMaterial;
	std::map<SCENE_KEY,cResource*>						m_contMatrixTexture;
	std::map<SCENE_KEY,cResource*>						m_contVertexTexture;
	std::map<cRscIndexBuffer*,cResource*>						m_contIndexInstancingBuffer;
	std::map<cRscVertexBuffer*,cResource*>						m_contVertexInstancingBuffer;
public:		
	int					GetCount();
	BOOL				InsertResource(cResource* in);
	void				EraseResource(const std::string& strKey);	

	void				GetKeyTexture(std::string& key,const  char* filePath );
	cRscTexture*		CreateRscTexture(const char* filePath);
	void				EraseRscTexture(const std::string& strKey);	

	void				GetKeyVertexBuffer(std::string& key, const char* rootName,const char* meshName );
	cRscVertexBuffer*	CreateRscVertexBuffer(std::string& strKey,DWORD bufferSize);
	void				EraseRscVertexBuffer( const std::string& strKey );

	void				GetKeyIndexBuffer(std::string& key, const char* rootName, const char* meshName );
	cRscIndexBuffer*	CreateRscIndexBuffer(std::string& strKey,DWORD bufferSize);
	void				EraseRscIndexBuffer( const std::string& strKey );

	void				GetKeyEntityAnimation(std::string& key,const  char* filePath );
	EntityAnimation*	CreateEntityAnimation(const char* filePath);
	void				EraseEntityAnimation(const std::string& strKey);	
	
	void				GetKeyMaterial(std::string& key,const  char* rootName,int refIndex,int subIndex);
	Material*			CreateMaterial(const  char* rootName,int refIndex,int subIndex);
	void				EraseMaterial(const std::string& strKey);	

	void				GetKeyEntityMaterial(std::string& key,const  char* filePath );
	EntityMaterial*		CreateEntityMaterial(const char* filePath);
	void				EraseEntityMaterial(const std::string& strKey);	
	

	IndexInstancingBuffer*	CreateIndexInstancingBuffer(cRscIndexBuffer* key,DWORD buffersize,DWORD count);
	void				EraseIndexInstancingBuffer(cRscIndexBuffer* key);

	MatrixInstancingTexture*		CreateMatrixTexture(SCENE_KEY& key,DWORD size);
	void				EraseBoneStreamTexture(SCENE_KEY& key);

// 	VertexTexture*		CreateVertexTexture(SCENE_KEY& key,DWORD size);
// 	void				EraseVertexTexture(SCENE_KEY& key);

	VertexInstancingBuffer*	CreateVertexInstancingBuffer(cRscVertexBuffer* pKey,DWORD buffersize ,DWORD count);
	void				EraseVertexInstancingBuffer(cRscVertexBuffer* pKey);
};

}