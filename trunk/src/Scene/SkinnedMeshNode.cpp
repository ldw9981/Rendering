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


SkinnedMeshNode::SkinnedMeshNode(void)
{

}

SkinnedMeshNode::~SkinnedMeshNode(void)
{
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
		pBoneRefInfo->pBoneRef = dynamic_cast<cMeshNode*>(GetRootNode()->FindNode(pBoneRefInfo->strNodeName));
		// 스킨드 메쉬가 참조하는 노드는 본으로 설정하고 그리지 않는다.
		pBoneRefInfo->pBoneRef->SetIsBone(true);

		tmBoneOffset = pBoneRefInfo->pBoneRef->GetWorldTM() * tmWorldInv;
		D3DXMatrixInverse(&pBoneRefInfo->BoneOffSetTM_INV,NULL,&tmBoneOffset);	

	}	
}

/*

일반 Object, Bone , Skined Mesh 전부 그리고음.
*/
void SkinnedMeshNode::Render()
{
	m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_3WEIGHTS);				

	int iBoneRef;
	for (iBoneRef=0;iBoneRef<(int)m_vecBoneRef.size();iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];
		
		D3DXMATRIX BlendMat,BoneWorldTM;	

		BoneWorldTM = refItem.pBoneRef->GetWorldTM();				// BoneWorldTM		

		BlendMat = refItem.BoneOffSetTM_INV * BoneWorldTM;
//		m_pD3DDevice->SetTransform(D3DTS_WORLDMATRIX(iBoneRef),&BlendMat);
		D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmWorld,&BlendMat);
	}		

	//IndexBuffer,VertexBuffer셋팅			
	m_pD3DDevice->SetFVF(FVF_BLENDVERTEX);				
	m_pRscVetextBuffer->SetStreamSource(sizeof(BLENDVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	
	//메쉬에 사용될 매트리얼 얻기
	Material* pMaterial=&m_Matrial;
	cRscTexture* pRscTexture=NULL;

/*
	//텍스쳐 적용
	pRscTexture=pMaterial->GetMapDiffuse();
	if (pRscTexture!=NULL)	
		m_pD3DDevice->SetTexture(0,pRscTexture->GetD3DTexture());	
	else
		m_pD3DDevice->SetTexture(0,NULL);
*/
	//텍스쳐 적용
	pRscTexture=pMaterial->GetMapDiffuse();
	if (pRscTexture!=NULL)
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",pRscTexture->GetD3DTexture());
	}



	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetVerties(),
		m_nStartIndex,
		m_nPrimitiveCount );

	m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);
	m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);						
				
	
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