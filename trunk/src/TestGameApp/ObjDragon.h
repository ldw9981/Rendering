#pragma once
#include "Scene/SceneNode.h"
#include "Foundation/Interface.h"
#include "WinInput/WinInput.h"

class cObjDragon :
	public cSceneNode,
	public cStaticWinInput,
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
	virtual	void	Update(DWORD elapseTime);

	virtual void    Control();
	virtual void	Render();

};
