#pragma once

#include "Restore.h"

class cResource;
class cIRestore;
class cRscTexture;
class cRscVertexBuffer;
class cRscIndexBuffer;
class cResourceMng;
class cRscTransformAnm;
class Animation;


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
	std::map<std::string,cResource*>					m_contTransformAnm;
	std::map<std::string,cResource*>					m_contAnimation;
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

	void				GetKeyTransformAnm( std::string& key, const  char* rootName, const char* meshName, const char* anmName );
	cRscTransformAnm*	CreateRscTransformAnm(const char* rootName,const char* meshName, const char* anmName);
	void				EraseRscTransformAnm( const std::string& strKey );

	void				GetKeyAnimation(std::string& key,const  char* filePath );
	Animation*			CreateAnimation(const char* filePath);
	void				EraseAnimation(const std::string& strKey);	

	int					GetCount();
};

