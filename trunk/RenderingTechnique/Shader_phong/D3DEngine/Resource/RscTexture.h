#pragma once

#include "../Resource/Resource.h"
#include "../Resource/ResourceMng.h"
#include "../Renderer/D3DGraphics.h"
#include "../Resource/Restore.h"

class cRscTexture:
	public cResource,
	public cIRestore,
	public cStaticD3DGraphics,
	public cStaticD3DDEVICE9
{
public:	
	cRscTexture(void);
	~cRscTexture(void);		
private:	
	LPDIRECT3DTEXTURE9		m_pD3DTexture;			
public:		
	LPDIRECT3DTEXTURE9		GetD3DTexture() const { return m_pD3DTexture; }

	// cResource
	virtual	BOOL			Create();
	virtual	BOOL			Destroy();
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();
private:
};
