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
	UpdateWorldTM(UpdateTransformAnm(elapseTime),m_pParentNode);

	m_pBoundingSphere->SetCenterPos(D3DXVECTOR3(m_WorldTM._41,m_WorldTM._42,m_WorldTM._43));
	*m_pCullingSphere = *m_pBoundingSphere;	
	
	UpdateSubMesh(elapseTime);
	UpdateChildren(elapseTime);
	UpdateParentCullingSphere(*m_pCullingSphere);	
}

/*

	�Ϲ� Object, Bone , Skined Mesh ���� �׸�����.
*/
void cMeshNode::Render()
{		
	//IndexBuffer,VertexBuffer����

#ifdef USE_EFFECT
	D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmWorld,&m_WorldTM);
#else
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_WorldTM );	
#endif	

	string strPick("BOPED_R_foot");
	if ( m_strNodeName == strPick )
	{
		char temp[1024]={0,};		
		sprintf_s(temp,1024,"%f %f %f\n",m_WorldTM._41,m_WorldTM._42,m_WorldTM._43);
		OutputDebugStr(temp);
	}

	m_pD3DDevice->SetFVF(FVF_NORMALVERTEX);
	m_pRscVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	//�޽��� ���� ��Ʈ���� ���
	Material* pMaterial=&m_Matrial;
	cRscTexture* pRscTexture=NULL;
		
	
#ifdef USE_EFFECT
	//�ؽ��� ����
	pRscTexture=pMaterial->GetMapDiffuse();
	if (pRscTexture!=NULL)
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",pRscTexture->GetD3DTexture());
	}

#else
	if (pRscTexture!=NULL)	

		m_pD3DDevice->SetTexture(0,pRscTexture->GetD3DTexture());	
	else
		m_pD3DDevice->SetTexture(0,NULL);
#endif
	
	
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


	BuildSubMesh();

	cSceneNode::BuildComposite();
}

void cMeshNode::CullRendererTraversal( cRendererQueue* pRendererQueue,cCameraNode* pActiveCamera )
{	
	if (!m_bRender)
		goto children;
	

	if (!m_vecSubMesh.empty())
	{
		PushSubRender(pRendererQueue);
		goto children;
	}
	
	// �ڽĳ�忡���� ���ŵǴ� �ø����� Ȱ��ȭ�� ī�޶� ����ü�� ���������Ȯ�� Ȯ��
	cCollision::STATE retCS=pActiveCamera->CheckWorldFrustum(m_pCullingSphere);
	if( retCS == cCollision::OUTSIDE)
	{	//  �ۿ� �ִ°��̸� ����ȸ ����
		return;
	}
	else if (retCS == cCollision::INSIDE)
	{	// ������ ���θ� �ڽ��� ��� ť�� �ְ� ��ȸ����			
		PushTraversal(pRendererQueue,pActiveCamera);
		return;
	}
	

		// cCollision::INTERSECT ��ġ�� �ڽ��� �ٿ�� ���Ǿ�� �˻�. 
	if (m_pBoundingSphere!=NULL)
	{
		cCollision::STATE retBS=pActiveCamera->CheckWorldFrustum(m_pBoundingSphere);
		if( retBS != cCollision::OUTSIDE)	// INTERSECT or INSIDE�� ť�� �ִ´�.
		{				
			pRendererQueue->Insert(this);						
		}	
	}
	else
	{	//��� ������ �ٿ�� ���Ǿ������ �׳� �׸���.	
		pRendererQueue->Insert(this);
	}
		
children:
	list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->CullRendererTraversal(pRendererQueue,pActiveCamera);
	}
}

void cMeshNode::AddMultiSub( cMeshNode* mesh )
{
	m_vecSubMesh.push_back(mesh);
}

void cMeshNode::PushSubRender( cRendererQueue* pRendererQueue )
{
	vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
	for ( ;it!=m_vecSubMesh.end();++it )
	{
		pRendererQueue->Insert(*it);
	}
}

void cMeshNode::UpdateSubMesh( DWORD elapseTime )
{
	vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
	for ( ;it!=m_vecSubMesh.end();++it )
	{
		(*it)->Update(elapseTime);
	}
}

void cMeshNode::BuildSubMesh()
{
	vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
	for ( ;it!=m_vecSubMesh.end();++it )
	{
		(*it)->BuildComposite();
	}
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