#pragma once

#include "Resource/Resource.h"
#include "Resource/ResourceMng.h"
#include "Resource/Restore.h"

namespace Sophia
{

class cRscTexture:
	public cResource,
	public cIRestore
{
public:	
	cRscTexture(void);
	~cRscTexture(void);		
protected:	
	LPDIRECT3DTEXTURE9		m_pD3DTexture;			
	std::string				m_filePath;
	
public:		
	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DTEXTURE9		GetD3DTexture() const { return m_pD3DTexture; }
	void					SetTexture(UINT stage);
	static void				SetNullTexture(UINT stage);
	void SetFilePath(const char* filePath) { m_filePath = filePath; }
	const char* GetFilePath() const { return m_filePath.c_str(); }
};

}