#pragma once

#include "Resource/Resource.h"
#include "Resource/ResourceMng.h"
#include "Resource/Restore.h"
#include "D3D9Server/StaticD3DDevice9.h"

class cRscTexture:
	public cResource,
	public cIRestore,
	private StaticD3DDEVICE9
{
public:	
	cRscTexture(void);
	~cRscTexture(void);		
private:	
	LPDIRECT3DTEXTURE9		m_pD3DTexture;			
public:		
	// cResource
	virtual void			ProcessMakeUniqueKey();
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();

	LPDIRECT3DTEXTURE9		GetD3DTexture() const { return m_pD3DTexture; }
	void					SetTexture(UINT stage);
	static void				SetNullTexture(UINT stage);
};
