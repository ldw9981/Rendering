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


SkinnedMeshNode::SkinnedMeshNode(void)
{
	m_pArrayMatBoneRef = NULL;
}

SkinnedMeshNode::~SkinnedMeshNode(void)
{
	if (m_pArrayMatBoneRef!=NULL)
	{
		delete m_pArrayMatBoneRef;
	}	
	m_vecBoneRef.clear();	
}

void SkinnedMeshNode::LinkToBone()
{
	assert(GetRootNode()!=NULL);
	D3DXMATRIX tmWorldInv,tmBoneOffset;
	D3DXMatrixInverse(&tmWorldInv,NULL,&GetWorldTM());			

	vector<BONEREFINFO>::iterator iter;
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
	//IndexBuffer,VertexBuffer셋팅			
	m_pD3DDevice->SetFVF(FVF_BLENDVERTEX);				
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




	
	//메쉬에 사용될 매트리얼 얻기
	Material* pMaterial=&m_Matrial;
	cRscTexture* pRscTexture=NULL;


	pRscTexture=pMaterial->GetMapDiffuse();
	if (pRscTexture!=NULL)
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",pRscTexture->GetD3DTexture());
	}
	else
	{
		ASSERT(pRscTexture!=NULL);
	}



	D3D9::Server::g_pServer->GetEffect()->CommitChanges();


	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetVerties(),
		m_nStartIndex,
		m_nPrimitiveCount );			
	
}

void SkinnedMeshNode::BuildComposite()
{	
	LinkToBone();
	cMeshNode::BuildComposite();
}

void SkinnedMeshNode::SetBoneRef( vector<BONEREFINFO>& vecBoneRef )
{
	m_vecBoneRef = vecBoneRef;
}

void SkinnedMeshNode::QueueRenderer()
{
	g_pD3DFramework->m_listRenderQueue[1].Insert(this);
}

