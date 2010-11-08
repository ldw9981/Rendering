#pragma once

#include "Restore.h"

class cResource;
class cIRestore;
class cRscTexture;
class cResourceMng;




class cResourceMng:
	public cStaticRestoreList
{
public:
	cResourceMng(void);
	~cResourceMng(void);
private:
	map<wstring,cResource*>					m_mapResource;
	
private:
	BOOL			InsertResource(cResource* in);
	void			DeleteResource(wstring& strKey);
public:		
	

	void			RestoreAll();
	


	cRscTexture*	CreateRscTexture(LPCWSTR szFilePath);
	cRscTexture*	CreateRscTexture(UINT nUniqueNumber);
	void			DestroyRscTexture(LPCWSTR szFilePath);
	void			DestroyRscTexture(UINT nUniqueNumber);
};

class cStaticResourceMng
{
public:
	cStaticResourceMng(void){};
	~cStaticResourceMng(void){};
public:
	static cResourceMng m_ResourceMng;
};


