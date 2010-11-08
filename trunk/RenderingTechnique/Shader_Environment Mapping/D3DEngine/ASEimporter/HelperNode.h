#pragma once
#include "SceneNode.h"
#include "../Geometrics/Sphere.h"

class cHelperNode :
	public cSceneNode
{
public:
	cHelperNode(void);
	virtual ~cHelperNode(void);

private:
	string			m_strClassName;
	D3DXVECTOR3		m_BoundBoxMin;
	

	D3DXVECTOR3		m_BoundBoxMax;
	cSphere*		m_pBoundSphere;


public:
	cSphere*		GetBoundSphere()  { return m_pBoundSphere; }
	D3DXVECTOR3&	GetBoundBoxMin()  { return m_BoundBoxMin; }	
	D3DXVECTOR3&	GetBoundBoxMax()  { return m_BoundBoxMax; }	

			void	SetClassName( LPCSTR name );
			string&	GetClassName();

	// cSceneNode
	
	virtual BOOL	LoadObjInfo(cASEParser* pParser);
	

	// cObject
	virtual void Init();
	virtual void	Uninit();
	
	virtual void	Update();
	virtual void	Render();


};
