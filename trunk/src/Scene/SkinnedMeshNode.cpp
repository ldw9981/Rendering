/*
	

*/
#include "StdAfx.h"
#include "SkinnedMeshNode.h"
#include "ASEParser/ASEParser.h"
#include "ASEParser/ASELexer.h"

#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Math/CollisionDetector.h"
#include "D3D9Server/RscTexture.h"
#include "D3D9Server/Server.h"

#include "Math/Sphere.h"
#include "Foundation/Define.h"
#include "Framework/D3DFramework.h"
#include "Scene/View.h"

SkinnedMeshNode::SkinnedMeshNode(void)
{
	m_pArrayMatBoneRef = NULL;
}

SkinnedMeshNode::~SkinnedMeshNode(void)
{
	Release();
}

void SkinnedMeshNode::LinkToBone()
{
	assert(GetRootNode()!=NULL);
	D3DXMATRIX tmWorldInv,tmBoneOffset;
	D3DXMatrixInverse(&tmWorldInv,NULL,&GetWorldTM());			

	std::vector<BONEREFINFO>::iterator iter;
	for ( iter=m_vecBoneRef.begin() ; iter!=m_vecBoneRef.end() ; ++iter)
	{
		BONEREFINFO* pBoneRefInfo=&(*iter);		
		//이름,타입 같으면 캐스팅 MESH확인
		pBoneRefInfo->pRefBoneMesh = dynamic_cast<cMeshNode*>(GetRootNode()->FindNode(pBoneRefInfo->strNodeName));
		// 스킨드 메쉬가 참조하는 노드는 본으로 설정하고 그리지 않는다.
		pBoneRefInfo->pRefBoneMesh->SetIsBone(true);

		tmBoneOffset = pBoneRefInfo->pRefBoneMesh->GetWorldTM() * tmWorldInv;
		D3DXMatrixInverse(&pBoneRefInfo->BoneOffSetTM_INV,NULL,&tmBoneOffset);	
	}	

	
	if(!m_vecBoneRef.empty())
	{
		m_pArrayMatBoneRef = new D3DXMATRIX[m_vecBoneRef.size()];
	}
	
		
}

/*

일반 Object, Bone , Skined Mesh 전부 그리고음.
*/
void SkinnedMeshNode::Render()
{
	//DebugRender();			
	m_pD3DDevice->SetVertexDeclaration(D3D9::Server::g_pServer->m_pVertexDeclationBlend);
	m_pRscVetextBuffer->SetStreamSource(sizeof(BLENDVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	int iBoneRef,nBoneRefSize = (int)m_vecBoneRef.size();
	// 현재메쉬가 참조하는 본개수만큼
	for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];
		
		D3DXMATRIX BlendMat,BoneWorldTM;	

		BoneWorldTM = refItem.pRefBoneMesh->GetWorldTM();				// BoneWorldTM		

		BlendMat = refItem.BoneOffSetTM_INV * BoneWorldTM;

		m_pArrayMatBoneRef[iBoneRef] = BlendMat;

	}

	if (nBoneRefSize>0)
	{
		D3D9::Server::g_pServer->GetEffect()->SetMatrixArray(D3D9::Server::g_pServer->m_hmPalette,m_pArrayMatBoneRef,nBoneRefSize);
	}	
	

	if( m_Matrial.GetMapDiffuse() != NULL )
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",m_Matrial.GetMapDiffuse()->GetD3DTexture());
	}

	if( m_Matrial.GetMapNormal() != NULL )
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex1",m_Matrial.GetMapNormal()->GetD3DTexture());
	}

	if( m_Matrial.GetMapLight() != NULL )
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex3",m_Matrial.GetMapLight()->GetD3DTexture());
	}
	D3D9::Server::g_pServer->GetEffect()->CommitChanges();

	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetCount(),
		m_nStartIndex,
		m_nPrimitiveCount );			
	
}

void SkinnedMeshNode::BuildComposite()
{	
	LinkToBone();

	if (m_vecSubMesh.empty())
	{
		if ((m_pRscIndexBuffer==NULL)||(m_pRscVetextBuffer==NULL))
		{
			m_bRender=false;
		}
	}
	else
	{
		std::vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
		for ( ;it!=m_vecSubMesh.end();++it )
		{
			(*it)->BuildComposite();
		}
	}

	if (m_Matrial.GetMapNormal() != NULL && m_pRscVetextBuffer !=NULL && m_pRscIndexBuffer != NULL)
	{
		long vertexCount = m_pRscVetextBuffer->GetCount();
		long triangleCount = m_pRscIndexBuffer->GetCount();
		BLENDVERTEX* vertex=(BLENDVERTEX*)m_pRscVetextBuffer->Lock();
		TRIANGLE* triangle = (TRIANGLE*)m_pRscIndexBuffer->Lock();

		for (long a = 0; a < triangleCount; a++)
		{
			long i1 = triangle->index[0];
			long i2 = triangle->index[1];
			long i3 = triangle->index[2];

			CalculateVector( vertex[i1].vertex,vertex[i2].vertex,vertex[i3].vertex,
				vertex[i1].uv0,vertex[i2].uv0,vertex[i3].uv0,
				vertex[i1].tangent,vertex[i2].tangent,vertex[i3].tangent,
				vertex[i1].binormal,vertex[i2].binormal,vertex[i3].binormal	);

			triangle++;
		}
		m_pRscIndexBuffer->Unlock();
		m_pRscVetextBuffer->Unlock();
	}

	cSceneNode::BuildComposite();	
}

void SkinnedMeshNode::SetBoneRef( std::vector<BONEREFINFO>& vecBoneRef )
{
	m_vecBoneRef = vecBoneRef;
}

void SkinnedMeshNode::QueueRenderer(cView* pView,bool bTraversal)
{
	if (m_vecSubMesh.empty())
	{
		if (m_bRender)
		{
			int i = m_Matrial.index_renderer_queue();
			pView->m_listRenderQueueSkinned[i].Insert(this);
		}
	}
	else
	{
		std::vector<cMeshNode*>::iterator it_sub=m_vecSubMesh.begin();
		for ( ;it_sub!=m_vecSubMesh.end();++it_sub )
		{
			(*it_sub)->QueueRenderer(pView,bTraversal);
		}
	}
	
	if (!bTraversal)
		return;

	std::list<cSceneNode*>::iterator it_child=m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->QueueRenderer(pView,bTraversal);
	}
}


void SkinnedMeshNode::QueueRendererShadow( cView* pView,bool bTraversal )
{
	if (m_vecSubMesh.empty())
	{
		if (m_bRender)
		{
			pView->m_listShadowBlend.Insert(this);
		}
	}
	else
	{
		std::vector<cMeshNode*>::iterator it_sub=m_vecSubMesh.begin();
		for ( ;it_sub!=m_vecSubMesh.end();++it_sub )
		{
			(*it_sub)->QueueRendererShadow(pView,bTraversal);
		}
	}

	if (!bTraversal)
		return;

	std::list<cSceneNode*>::iterator it_child=m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->QueueRendererShadow(pView,bTraversal);
	}
}

void SkinnedMeshNode::Release()
{
	cMeshNode::Release();
	if ( m_strNodeName == std::string("Bone03"))
	{
		printf("DDD");
	}
	if (m_pArrayMatBoneRef!=NULL)
	{
		delete m_pArrayMatBoneRef;
	}	
	m_vecBoneRef.clear();	
}

