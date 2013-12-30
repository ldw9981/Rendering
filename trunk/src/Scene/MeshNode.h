#pragma once
#include "Foundation/interface.h"
#include "Math/Sphere.h"
#include "D3D9Server/RscVertexBuffer.h"
#include "D3D9Server/RscIndexBuffer.h"
#include "SceneNode.h"
#include "D3D9Server/MaterialEx.h"
#include "D3D9Server/Vertex.h"

class cMeshNode;
class cView;

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
	std::vector<cMeshNode*>		m_vecSubMesh;
public:
	virtual void			Update(DWORD elapseTime);
	virtual void			Render();
	virtual void			RenderShadow();
	virtual	void			BuildComposite();

	void					SetStartIndex(WORD val) { m_nStartIndex = val; }
	void					SetPrimitiveCount(WORD val) { m_nPrimitiveCount = val; }	
	
	void					SetRscIndexBuffer(cRscIndexBuffer* val);

	void					SetRscVertextBuffer(cRscVertexBuffer* val);

	BOOL					GetIsBone() const { return m_bIsBone; }
	void					SetIsBone(BOOL val) { m_bIsBone = val; }		

	void					SetMatrial(Material& val) { m_Matrial = val; }	


	void					AddMultiSub(cMeshNode* mesh);	


	void					CullRendererIntoRendererQueue(cView* pView,Frustum* pFrustum );
	virtual void			QueueRenderer(cView* pView,bool bTraversal);

	void					CalculateTangentBinormal();
	void                    CalculateVector(const D3DXVECTOR3& vertex1,const D3DXVECTOR3& vertex2,const D3DXVECTOR3& vertex3,
		const TEXCOORD& t1,const TEXCOORD& t2,const TEXCOORD& t3,
		D3DXVECTOR3& tangent1,D3DXVECTOR3& tangent2,D3DXVECTOR3& tangent3,
		D3DXVECTOR3& binormal1,D3DXVECTOR3& binormal2,D3DXVECTOR3& binormal3);
	void					DebugRender(); 
};



