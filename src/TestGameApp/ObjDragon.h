#pragma once
#include "Graphics/Entity.h"
#include "Foundation/Interface.h"

class cObjDragon :
	public Sophia::Entity,
	public Sophia::IControlable
{
public:
	cObjDragon();
	virtual ~cObjDragon(void);
	BOOL		m_bControl;
private:
	Sophia::cSceneNode* m_pDummy;		//¿ëÀÇ HelperObj
	
public:
	//IObject
	virtual	void	Init();
	virtual	void	Uninit();

	virtual void    Control(DWORD elapseTime);
	virtual void	Render(DWORD elapseTime);

};
