#pragma once
#include "./GameObject.h"
#include "./SceneGraph/SceneGraphNode.h"
#include "./Common/Interface.h"
#include "./Input/WinInput.h"


class cObjTank:
	public cSceneGraphNode,
	public cStaticWinInput,
	public cIControlable
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
