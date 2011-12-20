#pragma once
#include "Scene/SceneNode.h"
#include "Foundation/Interface.h"
#include "WinInput/WinInput.h"


class cObjTank:
	public cSceneNode,
	public cStaticWinInput,
	public IControlable
{
public:
	cObjTank(void);
	virtual ~cObjTank(void);

	BOOL		m_bControl;

	//IObject
	virtual	void	Init();
	virtual	void	Uninit();
	virtual	void	Update(DWORD elapseTime);

	virtual void    Control();
	virtual void	Render();

};
