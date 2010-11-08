#pragma once

#include "../Resource/Resource.h"
#include "../Resource/ResourceMng.h"
#include "../Renderer/D3DGraphics.h"
#include "../Resource/Restore.h"

class cRscTexture:
	public cResource,
	public cIRestore,
	private cStaticD3DGraphics,
	private cStaticD3DDEVICE9
{
public:	
	cRscTexture(void);
	~cRscTexture(void);		
private:	
	LPDIRECT3DTEXTURE9		m_pD3DTexture;			
public:		
	LPDIRECT3DTEXTURE9		GetD3DTexture() const { return m_pD3DTexture; }


	void	ApplyStage(UINT stage);

	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();
private:
};
