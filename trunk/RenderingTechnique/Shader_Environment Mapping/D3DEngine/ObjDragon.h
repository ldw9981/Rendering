#pragma once
#include "./GameObject.h"
#include "./Common/Interface.h"
#include "./Input/WinInput.h"
#include "./Renderer/StaticD3DDevice9.h"

class cSceneNode;
class cObjDragon :
	public cGameObject,
	public cStaticWinInput,
	public cIControlable,
	public cStaticD3DDEVICE9
{
public:
	cObjDragon(cWorld* pWorld);
	virtual ~cObjDragon(void);

private:
	cSceneNode* m_pDummy;		//¿ëÀÇ HelperObj
	LPDIRECT3DCUBETEXTURE9		m_pD3DCubeTexture;

public:
	//IObject
	virtual	void	Init();
	virtual	void	Uninit();
	virtual	void	Update();
	virtual	void	Render();
	virtual void    Control();

	virtual	void	OnCollide(cGameObject* pObj,cMeshNode* pMyMesh);	
};
