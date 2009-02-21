#pragma once
#include "./GameObject.h"
#include "./SceneGraph/SceneGraphNode.h"
#include "./Common/Interface.h"
#include "./Input/WinInput.h"

class cObjDragon :
	public cSceneGraphNode,
	public cStaticWinInput,
	public cIControlable
{
public:
	cObjDragon();
	virtual ~cObjDragon(void);

private:
	cSceneGraphNode* m_pDummy;		//¿ëÀÇ HelperObj

public:
	//IObject
	virtual	void	Init();
	virtual	void	Uninit();
	virtual	void	Update(DWORD elapseTime);

	virtual void    Control();


};
