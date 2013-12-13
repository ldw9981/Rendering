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

	cRscTexture*		CreateRscTexture(const char* szFilePath);
	cRscTexture*		CreateRscTexture(UINT nUniqueNumber=0);
	cRscVertexBuffer*	CreateRscVertexBuffer(DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT,UINT nUniqueNumber=0);
	cRscIndexBuffer*	CreateRscIndexBuffer(DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT,UINT nUniqueNumber=0);
	cRscTransformAnm*	CreateRscTransformAnm(UINT findUniqueNumber=0);	
};

class cStaticResourceMng
{
public:
	cStaticResourceMng(void){};
	~cStaticResourceMng(void){};
protected:
	static cResourceMng m_ResourceMng;
};


