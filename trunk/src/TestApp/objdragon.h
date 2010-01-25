#pragma once

#include "../Scene/SceneGraphNode.h"
#include "../Framework/Interface.h"
#include "../Framework/Input/WinInput.h"
#include "../Framework/Interface.h"
#include "../Foundation/Define.h"

class cObjDragon :
	public SceneGraphNode,
	public SharedWinInput,
	public Framework::IControlable
{
public:
	cObjDragon();
	virtual ~cObjDragon(void);

private:
	SceneGraphNode* m_pDummy;		//¿ëÀÇ HelperObj

public:
	//IObject
	virtual	void	Init();
	virtual	void	Uninit();
	virtual	void	Update(DWORD elapseTime);

	virtual void    Control();


};
