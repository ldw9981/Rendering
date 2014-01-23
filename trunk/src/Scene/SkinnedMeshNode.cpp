/*
	

*/
#include "StdAfx.h"
#include "SkinnedMeshNode.h"
#include "ASEParser/ASEParser.h"
#include "ASEParser/ASELexer.h"

#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Math/CollisionDetector.h"
#include "Graphics/RscTexture.h"
#include "Graphics/Graphics.h"

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

void SkinnedMeshNode::LinkToBone(Entity* pEntity)
{
	assert(!m_vecBoneRef.empty());
	D3DXMATRIX tmBoneWorldReferenceInv;

	std::vector<BONEREFINFO>::iterator iter;
	for ( iter=m_vecBoneRef.begin() ; iter!=m_vecBoneRef.end() ; ++iter)
	{
		BONEREFINFO* pBoneRefInfo=&(*iter);				
		pBoneRefInfo->pMesh = pEntity->FindNode(pBoneRefInfo->strNodeName);	
		//pBoneRefInfo->pMesh->SetIsBone(true);		// ��Ų�� �޽��� �����ϴ� ���� ������ �����ϰ� �׸��� �ʴ´�.
	
		D3DXMatrixInverse(&tmBoneWorldReferenceInv,NULL,&pBoneRefInfo->pMesh->GetWorldReference());
		pBoneRefInfo->SkinOffset = GetWorldReference() * tmBoneWorldReferenceInv;	// LocalTM = WorldTM * Parent.WorldTM.Inverse
	}	
	m_pArrayMatBoneRef = new D3DXMATRIX[m_vecBoneRef.size()];	
}

/*

�Ϲ� Object, Bone , Skined Mesh ���� �׸�����.
*/
void SkinnedMeshNode::Render(unsigned char multiSubIndex)
{
	MultiSub& temp = m_vecMultiSub[multiSubIndex];
	Material& material = m_vecMaterial[temp.materialIndex];

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::g_pGraphics->m_pVertexDeclationBlend);
	m_pRscVetextBuffer->SetStreamSource(sizeof(BLENDVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	int iBoneRef,nBoneRefSize = (int)m_vecBoneRef.size();
	// ����޽��� �����ϴ� ��������ŭ
	for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];
		m_pArrayMatBoneRef[iBoneRef] = refItem.SkinOffset * refItem.pMesh->GetWorldTM();	// WorldTM = LocalTM * Parent.WorldTM
	}	

	Graphics::g_pGraphics->GetEffect()->SetMatrixArray(Graphics::g_pGraphics->m_hmPalette,m_pArrayMatBoneRef,nBoneRefSize);	

	if( material.GetMapDiffuse() != NULL )
	{
		Graphics::g_pGraphics->GetEffect()->SetTexture("Tex0",material.GetMapDiffuse()->GetD3DTexture());
	}

	if( material.GetMapNormal() != NULL )
	{
		Graphics::g_pGraphics->GetEffect()->SetTexture("Tex1",material.GetMapNormal()->GetD3DTexture());
	}

	if( material.GetMapLight() != NULL )
	{
		Graphics::g_pGraphics->GetEffect()->SetTexture("Tex3",material.GetMapLight()->GetD3DTexture());
	}
	Graphics::g_pGraphics->GetEffect()->CommitChanges();

	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetCount(),
		temp.startIndex,
		temp.primitiveCount );			
	
}

void SkinnedMeshNode::BuildComposite(Entity* pEntity)
{	
	LinkToBone(pEntity);		

		assert(m_pRscVetextBuffer!=NULL);
		assert(m_pRscIndexBuffer!=NULL);
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


	QueueRenderer(pEntity,false);
	QueueRendererShadow(pEntity,false);
	cSceneNode::BuildComposite(pEntity);
}

void SkinnedMeshNode::SetBoneRef( std::vector<BONEREFINFO>& vecBoneRef )
{
	m_vecBoneRef = vecBoneRef;
}

void SkinnedMeshNode::QueueRenderer(Entity* pEntity,bool bTraversal)
{
	if (m_bRender)
	{	
		unsigned char multiSubIndex=0;
		for (auto it_sub=m_vecMultiSub.begin();it_sub!=m_vecMultiSub.end();++it_sub )
		{
			MultiSub& temp = (*it_sub);
			Material& material = m_vecMaterial[temp.materialIndex];		

			int i = material.index_renderer_queue();
			pEntity->m_renderQueueBlend[i].Insert(this,multiSubIndex);
			multiSubIndex++;
		}
	}
	
	if (!bTraversal)
		return;

	for ( auto it_child=m_listChildNode.begin() ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->QueueRenderer(pEntity,bTraversal);
	}
}


void SkinnedMeshNode::QueueRendererShadow(Entity* pEntity,bool bTraversal )
{
	if (m_bRender)
	{	
		unsigned char multiSubIndex=0;
		for (auto it_sub=m_vecMultiSub.begin();it_sub!=m_vecMultiSub.end();++it_sub )
		{
			MultiSub& temp = (*it_sub);
			Material& material = m_vecMaterial[temp.materialIndex];		

			int i = material.index_renderer_queue();
			pEntity->m_renderQueueBlendShadow.Insert(this,multiSubIndex);
			multiSubIndex++;
		}
	}

	if (!bTraversal)
		return;
	
	for (auto it_child=m_listChildNode.begin() ; it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->QueueRendererShadow(pEntity,bTraversal);
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

