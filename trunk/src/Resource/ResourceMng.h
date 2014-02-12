#pragma once

#include "Restore.h"

class cResource;
class cIRestore;
class cRscTexture;
class cRscVertexBuffer;
class cRscIndexBuffer;
class cResourceMng;
class EntityAnimation;
class EntityMaterial;

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
	std::map<std::string,cResource*>					m_contAnimation;
	std::map<std::string,cResource*>					m_contMaterial;
public:		
	BOOL				InsertResource(cResource* in);
	void				EraseResource(const std::string& strKey);	

	void				GetKeyTexture(std::string& key,const  char* filePath );
	cRscTexture*		CreateRscTexture(const char* filePath);
	void				EraseRscTexture(const std::string& strKey);	

	void				GetKeyVertexBuffer(std::string& key, const char* rootName,const char* meshName );
	cRscVertexBuffer*	CreateRscVertexBuffer(const char* rootName,const char* meshName,DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT);
	void				EraseRscVertexBuffer( const std::string& strKey );

	void				GetKeyIndexBuffer(std::string& key, const char* rootName, const char* meshName );
	cRscIndexBuffer*	CreateRscIndexBuffer(const char* rootName,const char* meshName,DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT);
	void				EraseRscIndexBuffer( const std::string& strKey );

	void				GetKeyEntityAnimation(std::string& key,const  char* filePath );
	EntityAnimation*	CreateEntityAnimation(const char* filePath);
	void				EraseEntityAnimation(const std::string& strKey);	
	
	void				GetKeyEntityMaterial(std::string& key,const  char* filePath );
	EntityMaterial*		CreateEntityMaterial(const char* filePath);
	void				EraseEntityMaterial(const std::string& strKey);	
	
	int					GetCount();
};
