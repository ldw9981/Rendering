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
	virtual ~cRscTexture(void);		
protected:	
	LPDIRECT3DTEXTURE9		m_pD3DTexture;			
	std::string				m_filePath;
	UINT					m_width;
	UINT					m_height;
	UINT					m_levels;
	DWORD					m_usage;
	D3DFORMAT				m_format;
	D3DPOOL					m_pool;

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

	UINT GetWidth() const { return m_width; }
	void SetWidth(UINT val) { m_width = val; }
	UINT GetHeight() const { return m_height; }
	void SetHeight(UINT val) { m_height = val; }
	D3DPOOL GetPool() const { return m_pool; }
	void SetPool(D3DPOOL val) { m_pool = val; }
	DWORD GetUsage() const { return m_usage; }
	void SetUsage(DWORD val) { m_usage = val; }
};

}