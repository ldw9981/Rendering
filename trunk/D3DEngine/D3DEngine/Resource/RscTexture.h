#pragma once

#include "../Resource/Resource.h"
#include "../Resource/ResourceMng.h"
#include "../Resource/Restore.h"
#include "../Framework/StaticD3DDevice9.h"

class cRscTexture:
	public cResource,
	public IRestore,
	private cStaticD3DDEVICE9
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

	// IRestore
	virtual void			Restore();

	LPDIRECT3DTEXTURE9		GetD3DTexture() const { return m_pD3DTexture; }
	void					SetTexture(UINT stage);
	static void				SetNullTexture(UINT stage);
};
