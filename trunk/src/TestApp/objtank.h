#pragma once

#include "../Scene/SceneGraphNode.h"
#include "../Framework/Interface.h"
#include "../Framework/Input/WinInput.h"


class cObjTank:
	public SceneGraphNode,
	public SharedWinInput,
	public Framework::IControlable
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


};
