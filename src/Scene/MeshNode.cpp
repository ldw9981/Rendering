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
	vector<cMeshNode*>::iterator it = m_vecSubMesh.begin();
	vector<cMeshNode*>::iterator it_end = m_vecSubMesh.end();

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
		vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
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
	m_pD3DDevice->SetFVF(FVF_NORMALVERTEX);
	m_pRscVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	//메쉬에 사용될 매트리얼 얻기
	Material* pMaterial=&m_Matrial;
	cRscTexture* pRscTexture=NULL;
		
	

	//텍스쳐 적용
	pRscTexture=pMaterial->GetMapDiffuse();
	if (pRscTexture!=NULL)
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",pRscTexture->GetD3DTexture());
	}
	else
	{
				
	}	

	D3D9::Server::g_pServer->GetEffect()->CommitChanges();

	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
			0,  
			0, 
			m_pRscVetextBuffer->GetVerties(),
			m_nStartIndex,
			m_nPrimitiveCount );

}

void cMeshNode::BuildComposite()
{
	if (m_bIsBone)
		m_bRender=false;

	if (m_vecSubMesh.empty())
	{
		if ((m_pRscIndexBuffer==NULL)||(m_pRscVetextBuffer==NULL))
		{
			m_bRender=false;
		}
	}
	else
	{
		vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
		for ( ;it!=m_vecSubMesh.end();++it )
		{
			(*it)->BuildComposite();
		}
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
		vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
		for ( ;it!=m_vecSubMesh.end();++it )
		{
			(*it)->CullRendererIntoRendererQueue(pActiveCamera);
		}			
	}		


children:
	list<cSceneNode*>::iterator it=m_listChildNode.begin();
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
	g_pD3DFramework->m_listRenderQueue[0].Insert(this);
}
