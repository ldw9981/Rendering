#pragma once

#include "Restore.h"

class cResource;
class cIRestore;
class cRscTexture;
class cRscVertexBuffer;
class cRscIndexBuffer;
class cResourceMng;
class cRscTransformAnm;


class cResourceMng:
	private cStaticRestoreList
{
public:
	cResourceMng(void);
	~cResourceMng(void);
private:
	std::map<std::string,cResource*>					m_mapResource;
	
public:		
	BOOL				InsertResource(cResource* in);
	void				EraseResource(const std::string& strKey);	
	void				RestoreAll();

	void				GetKeyTexture(std::string& key,const  char* filePath );
	cRscTexture*		CreateRscTexture(const char* filePath);

	void				GetKeyVertexBuffer(std::string& key, const char* rootName,const char* meshName );
	cRscVertexBuffer*	CreateRscVertexBuffer(const char* rootName,const char* meshName,DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT);

	void				GetKeyIndexBuffer(std::string& key, const char* rootName, const char* meshName );
	cRscIndexBuffer*	CreateRscIndexBuffer(const char* rootName,const char* meshName,DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT);

	void				GetKeyTransformAnm( std::string& key, const  char* rootName, const char* meshName, const char* anmName );
	cRscTransformAnm*	CreateRscTransformAnm(const char* rootName,const char* meshName, const char* anmName);
	
	int					GetCount();
};

class cStaticResourceMng
{
public:
	cStaticResourceMng(void){};
	~cStaticResourceMng(void){};
public:
	static cResourceMng* m_pResourceMng;
};


