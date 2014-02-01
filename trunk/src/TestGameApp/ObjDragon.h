#pragma once
#include "Graphics/Entity.h"
#include "Foundation/Interface.h"

class cObjDragon :
	public Entity,
	public IControlable
{
public:
	cObjDragon();
	virtual ~cObjDragon(void);
	BOOL		m_bControl;
private:
	cSceneNode* m_pDummy;		//¿ëÀÇ HelperObj
	
public:
	//IObject
	virtual	void	Init();
	virtual	void	Uninit();

	virtual void    Control();
	virtual void	Render();

};
