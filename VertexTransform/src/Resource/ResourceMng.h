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
class MatrixStreamVertexBuffer;
class IndexStreamVertexBuffer;
class BoneStreamTexture;
class VertexTransformationTexture;
class VertexStream;
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
	std::map<SCENE_KEY,cResource*>						m_contMatrixStreamVertexBuffer;
	std::map<SCENE_KEY,cResource*>						m_contIndexStreamVertexBuffer;
	std::map<SCENE_KEY,cResource*>						m_contBoneStreamTexture;
	std::map<SCENE_KEY,cResource*>						m_contVertexTransformationTexture;
	std::map<SCENE_KEY,cResource*>						m_contVertexSteam;
public:		
	int					GetCount();
	BOOL				InsertResource(cResource* in);
	void				EraseResource(const std::string& strKey);	

	void				GetKeyTexture(std::string& key,const  char* filePath );
	cRscTexture*		CreateRscTexture(const char* filePath);
	void				EraseRscTexture(const std::string& strKey);	

	void				GetKeyVertexBuffer(std::string& key, const char* rootName,const char* meshName );
	cRscVertexBuffer*	CreateRscVertexBuffer(const char* rootName,const char* meshName,DWORD bufferSize,D3DPOOL type=D3DPOOL_MANAGED);
	void				EraseRscVertexBuffer( const std::string& strKey );

	void				GetKeyIndexBuffer(std::string& key, const char* rootName, const char* meshName );
	cRscIndexBuffer*	CreateRscIndexBuffer(const char* rootName,const char* meshName,DWORD bufferSize,D3DPOOL type=D3DPOOL_MANAGED);
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
	
	MatrixStreamVertexBuffer*	CreateMatrixStreamVertexBuffer(SCENE_KEY& key);
	void				EraseMatrixStreamVertexBuffer(SCENE_KEY& key);

	IndexStreamVertexBuffer*	CreateIndexStreamVertexBuffer(SCENE_KEY& key);
	void				EraseIndexStreamVertexBuffer(SCENE_KEY& key);

	BoneStreamTexture*	CreateBoneStreamTexture(SCENE_KEY& key);
	void				EraseBoneStreamTexture(SCENE_KEY& key);

	VertexTransformationTexture*	CreateVertexTransformationTexture(SCENE_KEY& key);
	void				EraseVertexTransformationTexture(SCENE_KEY& key);

	VertexStream*		CreateVertexStream(SCENE_KEY& key,DWORD size );
	void				EraseVertexStream(SCENE_KEY& key);
};

}