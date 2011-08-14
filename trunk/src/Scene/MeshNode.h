#pragma once
#include "Foundation/interface.h"
#include "Math/Sphere.h"
#include "D3D9Server/RscVertexBuffer.h"
#include "D3D9Server/RscIndexBuffer.h"
#include "SceneNode.h"
#include "D3D9Server/MaterialEx.h"

class cMeshNode;


class cMeshNode:
	public cSceneNode,
	protected StaticD3DDEVICE9
{
public:
	cMeshNode(void);
	virtual ~cMeshNode(void);

protected:	

	BOOL					m_bIsBone;
	int						m_nStartIndex;
	int						m_nPrimitiveCount;

	Material				m_Matrial;
	cRscIndexBuffer*		m_pRscIndexBuffer;	
	cRscVertexBuffer*		m_pRscVetextBuffer;	
	vector<cMeshNode*>		m_vecSubMesh;
public:
	virtual void			Update(DWORD elapseTime);
	virtual void			Render();
	virtual	void			BuildComposite();

	void					SetStartIndex(WORD val) { m_nStartIndex = val; }
	void					SetPrimitiveCount(WORD val) { m_nPrimitiveCount = val; }	
	
	void					SetRscIndexBuffer(cRscIndexBuffer* val);

	void					SetRscVertextBuffer(cRscVertexBuffer* val);

	BOOL					GetIsBone() const { return m_bIsBone; }
	void					SetIsBone(BOOL val) { m_bIsBone = val; }		

	void					SetMatrial(Material& val) { m_Matrial = val; }	


	void					AddMultiSub(cMeshNode* mesh);	
	void					UpdateSubMesh(DWORD elapseTime);
	void					BuildSubMesh();

	void					CullRendererTraversal(cCameraNode* pActiveCamera );
	void					PushSubRender();


	virtual void			SendQueue();
};



