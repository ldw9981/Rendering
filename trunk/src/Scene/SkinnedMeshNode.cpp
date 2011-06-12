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
		//�̸�,Ÿ�� ������ ĳ���� MESHȮ��
		pBoneRefInfo->pBoneRef = dynamic_cast<cMeshNode*>(GetRootNode()->FindNode(pBoneRefInfo->strNodeName));
		// ��Ų�� �޽��� �����ϴ� ���� ������ �����ϰ� �׸��� �ʴ´�.
		pBoneRefInfo->pBoneRef->SetIsBone(true);

		tmBoneOffset = pBoneRefInfo->pBoneRef->GetWorldTM() * tmWorldInv;
		D3DXMatrixInverse(&pBoneRefInfo->BoneOffSetTM_INV,NULL,&tmBoneOffset);	
	}	

	
	if(!m_vecBoneRef.empty())
	{
		m_pArrayMatBoneRef = new D3DXMATRIX[m_vecBoneRef.size()];
	}
	
		
}

/*

�Ϲ� Object, Bone , Skined Mesh ���� �׸�����.
*/
void SkinnedMeshNode::Render()
{
#if USE_EFFECT
	D3D9::Server::g_pServer->GetEffect()->SetTechnique(D3D9::Server::g_pServer->m_hTSkinning);
#else
	m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_3WEIGHTS);	
#endif
			

	//IndexBuffer,VertexBuffer����			
	m_pD3DDevice->SetFVF(FVF_BLENDVERTEX);				
	m_pRscVetextBuffer->SetStreamSource(sizeof(BLENDVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	int iBoneRef,nBoneRefSize = (int)m_vecBoneRef.size();
	// ����޽��� �����ϴ� ��������ŭ
	for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];
		
		D3DXMATRIX BlendMat,BoneWorldTM;	

		BoneWorldTM = refItem.pBoneRef->GetWorldTM();				// BoneWorldTM		

		BlendMat = refItem.BoneOffSetTM_INV * BoneWorldTM;
#if USE_EFFECT
		m_pArrayMatBoneRef[iBoneRef] = BlendMat;
#else
		m_pD3DDevice->SetTransform(D3DTS_WORLDMATRIX(iBoneRef),&BlendMat);
#endif
	}

#if USE_EFFECT
	if (nBoneRefSize>0)
	{
		D3D9::Server::g_pServer->GetEffect()->SetMatrixArray(D3D9::Server::g_pServer->m_hmPalette,m_pArrayMatBoneRef,nBoneRefSize);
	}	
#endif



	
	//�޽��� ���� ��Ʈ���� ���
	Material* pMaterial=&m_Matrial;
	cRscTexture* pRscTexture=NULL;

#if USE_EFFECT
	pRscTexture=pMaterial->GetMapDiffuse();
	if (pRscTexture!=NULL)
	{
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",pRscTexture->GetD3DTexture());
	}
#else
	pRscTexture=pMaterial->GetMapDiffuse();
	if (pRscTexture!=NULL)	
		m_pD3DDevice->SetTexture(0,pRscTexture->GetD3DTexture());	
	else
		m_pD3DDevice->SetTexture(0,NULL);
#endif


#if USE_EFFECT
	D3D9::Server::g_pServer->GetEffect()->CommitChanges();
#endif

	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetVerties(),
		m_nStartIndex,
		m_nPrimitiveCount );

#if USE_EFFECT

#else
	m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);
	m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);		
#endif

				
				
	
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