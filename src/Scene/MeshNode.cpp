#include "StdAfx.h"
#include "MeshNode.h"
#include "ASEParser/ASEParser.h"
#include "ASEParser/ASELexer.h"

#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Math/CollisionDetector.h"
#include "D3D9Server/RscTexture.h"
#include "D3D9Server/Server.h"

#include "Math/Sphere.h"
#include "Foundation/Define.h"

#include "Scene/CameraNode.h"
#include "Scene/RendererQueue.h"

#include "Framework/D3DFramework.h"

cMeshNode::cMeshNode(void)
{		
	m_bIsBone= FALSE;


	m_pRscIndexBuffer=NULL;
	m_pRscVetextBuffer=NULL;

	m_nStartIndex=0;
	m_nPrimitiveCount=0;
	

	CreateCullingSphere();
	CreateBoundingSphere();
}

cMeshNode::~cMeshNode(void)
{
	std::vector<cMeshNode*>::iterator it = m_vecSubMesh.begin();
	std::vector<cMeshNode*>::iterator it_end = m_vecSubMesh.end();

	for ( ;it!=it_end ; it++ )
	{
		delete *it;
	}	
	m_vecSubMesh.clear();

//	for_each(m_vecSubMesh.begin(),m_vecSubMesh.end(),FuncDeleteType<cMeshNode*>);
	
//	m_vecSubMesh.clear();

	//SAFE_RELEASE(m_pRscVetextBuffer);
	if (m_pRscVetextBuffer)
	{
		m_pRscVetextBuffer->Release();
		m_pRscVetextBuffer=NULL;
	}
	if (m_pRscIndexBuffer)
	{
		m_pRscIndexBuffer->Release();
		m_pRscIndexBuffer=NULL;
	}
}


void cMeshNode::Update(DWORD elapseTime)
{
	cTransformable::Update(elapseTime);
	UpdateWorldMatrix(UpdateTransformAnm(elapseTime),m_pParentNode);

	// 바운딩스피어의 위치는 로컬좌표 기준이므로 월드로 바로 스지는 못한다.
	m_pBoundingSphere->SetCenterPos(D3DXVECTOR3(m_matWorld._41,m_matWorld._42,m_matWorld._43));
	*m_pCullingSphere = *m_pBoundingSphere;	

	if (!m_vecSubMesh.empty())
	{
		std::vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
		for ( ;it!=m_vecSubMesh.end();++it )
		{
			(*it)->Update(elapseTime);
		}
	}	

	UpdateChildren(elapseTime);
	UpdateParentCullingSphere(*m_pCullingSphere);	
}

/*

	일반 Object, Bone , Skined Mesh 전부 그리고음.
*/
void cMeshNode::Render()
{			
	D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmWorld,&m_matWorld);
	
	//IndexBuffer,VertexBuffer셋팅
	m_pD3DDevice->SetFVF(FVF_NORMAL);
	m_pRscVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
	m_pRscIndexBuffer->SetIndices();			


	assert( m_Matrial.GetMapDiffuse()->GetD3DTexture() != NULL);

	D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",m_Matrial.GetMapDiffuse()->GetD3DTexture());

	if( m_Matrial.GetMapNormal() != NULL )
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex1",m_Matrial.GetMapNormal()->GetD3DTexture());
	}

	D3D9::Server::g_pServer->GetEffect()->CommitChanges();
	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
			0,  
			0, 
			m_pRscVetextBuffer->GetCount(),
			m_nStartIndex,
			m_nPrimitiveCount );

}

void cMeshNode::BuildComposite()
{
	std::vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
	for ( ;it!=m_vecSubMesh.end();++it )
	{
		(*it)->BuildComposite();
	}

	if (m_bIsBone || m_pRscIndexBuffer==NULL || m_pRscVetextBuffer==NULL || m_Matrial.GetMapDiffuse() == NULL)
	{
		m_bRender=false;
	}	

	if (m_Matrial.GetMapNormal() != NULL && m_pRscVetextBuffer !=NULL && m_pRscIndexBuffer != NULL)
	{
		long vertexCount = m_pRscVetextBuffer->GetCount();
		long triangleCount = m_pRscIndexBuffer->GetCount();
		NORMALVERTEX* vertex=(NORMALVERTEX*)m_pRscVetextBuffer->Lock();
		TRIANGLE* triangle = (TRIANGLE*)m_pRscIndexBuffer->Lock();

		for (long a = 0; a < triangleCount; a++)
		{
			long i1 = triangle->index[0];
			long i2 = triangle->index[1];
			long i3 = triangle->index[2];

			CalculateVector( vertex[i1].vertex,vertex[i2].vertex,vertex[i3].vertex,
				vertex[i1].tex,vertex[i2].tex,vertex[i3].tex,
				vertex[i1].tangent,vertex[i2].tangent,vertex[i3].tangent,
				vertex[i1].binormal,vertex[i2].binormal,vertex[i3].binormal	);

			triangle++;
		}
		m_pRscIndexBuffer->Unlock();
		m_pRscVetextBuffer->Unlock();
	}

	cSceneNode::BuildComposite();	
}

void cMeshNode::CullRendererIntoRendererQueue(cCameraNode* pActiveCamera )
{	
	if (!m_bRender)
		goto children;
	
	
	if (m_vecSubMesh.empty())
	{
		// 자식노드에의해 갱신되는 컬링구가 활성화된 카메라 절두체와 어떤상태인지확인 확인
		cCollision::STATE retCS=pActiveCamera->CheckWorldFrustum(m_pCullingSphere);
		if( retCS == cCollision::OUTSIDE)
		{	//  밖에 있는것이면 노드순회 없음
			return;
		}
		else if (retCS == cCollision::INSIDE)
		{	// 자기넣고 순회
			QueueRenderer();					
		}
		else if (m_pBoundingSphere!=NULL)	// cCollision::INTERSECT 겹치면 자신의 바운딩 스피어랑 검사. 
		{
			cCollision::STATE retBS=pActiveCamera->CheckWorldFrustum(m_pBoundingSphere);
			if ( retBS == cCollision::INTERSECT || retBS == cCollision::INTERSECT)	
			{				
				QueueRenderer();						
			}	
		}	
	}
	else	// 멀티서브일때
	{
		std::vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
		for ( ;it!=m_vecSubMesh.end();++it )
		{
			(*it)->CullRendererIntoRendererQueue(pActiveCamera);
		}			
	}		


children:
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererIntoRendererQueue(pActiveCamera);
	}
}

void cMeshNode::AddMultiSub( cMeshNode* mesh )
{
	m_vecSubMesh.push_back(mesh);
}


void cMeshNode::SetRscIndexBuffer( cRscIndexBuffer* val )
{
	m_pRscIndexBuffer = val;
	if (val)
	{
		val->AddRef();
	}
}

void cMeshNode::SetRscVertextBuffer( cRscVertexBuffer* val )
{
	m_pRscVetextBuffer = val;
	if (val)
	{
		val->AddRef();
	}
}

void cMeshNode::QueueRenderer()
{
	int i = m_Matrial.index_renderer_queue();
	g_pD3DFramework->m_listRenderQueue[i].Insert(this);
}

void cMeshNode::CalculateVector(const D3DXVECTOR3& vertex1,const D3DXVECTOR3& vertex2,const D3DXVECTOR3& vertex3,
	const TEXCOORD& t1,const TEXCOORD& t2,const TEXCOORD& t3,
	D3DXVECTOR3& tangent1,D3DXVECTOR3& tangent2,D3DXVECTOR3& tangent3,
	D3DXVECTOR3& binormal1,D3DXVECTOR3& binormal2,D3DXVECTOR3& binormal3)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];

	float den;
	float length;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = t2.u - t1.u;
	tvVector[0] = t2.v - t1.v;

	tuVector[1] = t3.u - t1.u;
	tvVector[1] = t3.v - t1.v;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	tangent1 = tangent;
	tangent2 = tangent;
	tangent3 = tangent;

	binormal1 = binormal;
	binormal2 = binormal;
	binormal3 = binormal;

}