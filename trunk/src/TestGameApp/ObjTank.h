#pragma once
#include "Scene/Entity.h"
#include "Foundation/Interface.h"


class cObjTank:
	public Entity,
	public IControlable
{
public:
	cObjTank(void);
	virtual ~cObjTank(void);

	BOOL		m_bControl;

	//IObject
	virtual	void	Init();
	virtual	void	Uninit();

	virtual void    Control();
	virtual void	Render();

};
