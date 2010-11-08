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

	cMaterialEx				m_Matrial;
	cRscIndexBuffer*		m_pRscIndexBuffer;	
	cRscVertexBuffer*		m_pRscVetextBuffer;	
	vector<cMeshNode*>		m_vecSubMesh;
public:
	virtual void			Update(DWORD elapseTime);
	virtual void			Render();
	virtual	void			BuildComposite();

	void					SetStartIndex(WORD val) { m_nStartIndex = val; }
	void					SetPrimitiveCount(WORD val) { m_nPrimitiveCount = val; }	
	void					SetRscIndexBuffer(cRscIndexBuffer* val) { m_pRscIndexBuffer = val; }	
	void					SetRscVertextBuffer(cRscVertexBuffer* val) { m_pRscVetextBuffer = val; }

	BOOL					GetIsBone() const { return m_bIsBone; }
	void					SetIsBone(BOOL val) { m_bIsBone = val; }		

	void					SetMatrial(cMaterialEx& val) { m_Matrial = val; }	


	void					AddMultiSub(cMeshNode* mesh);	
	void					UpdateSubMesh(DWORD elapseTime);
	void					BuildSubMesh();

	void					CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera );
	void					PushSubRender(cRendererQueue* pRendererQueue);



};



