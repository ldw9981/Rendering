#pragma once

#include "../Foundation/Resource.h"
#include "../Foundation/ResourceMng.h"
#include "../Foundation/Restore.h"
#include "SharedDevice.h"

class Texture:
	public cResource,
	public IRestore,
	public SharedLPDIRECT3DDEVICE9
{
public:	
	Texture(void);
	~Texture(void);		
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
