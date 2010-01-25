#pragma once
#include "Restore.h"

#include <Windows.h>
#include <map>
#include <d3d9types.h>
#include <string>
#include "../Foundation/SharedTemplate.h"

class cResource;
class IRestore;

class ResourceMng;
class TransformAnm;
class Texture;
class VertexBuffer;
class IndexBuffer;



using namespace std;



class ResourceMng:
	private cStaticRestoreList
{
public:
	ResourceMng(void);
	~ResourceMng(void);
private:
	
	map<wstring,cResource*>					m_mapResource;
	
public:		
	static				ResourceMng* Instance();
	BOOL				InsertResource(cResource* in);
	void				EraseResource(const wstring& strKey);	
	void				RestoreAll();

	Texture*		CreateTexture(LPCWSTR szFilePath);
	Texture*		CreateTexture(UINT nUniqueNumber=0);
	VertexBuffer*	CreateVertexBuffer(DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT,UINT nUniqueNumber=0);
	IndexBuffer*	CreateIndexBuffer(DWORD bufferSize,D3DPOOL type=D3DPOOL_DEFAULT,UINT nUniqueNumber=0);
	TransformAnm*	CreateTransformAnm(UINT findUniqueNumber=0);	
};

DECLARE_SHAREDCLASS(ResourceMng);


