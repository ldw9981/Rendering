/*
	

*/
#include "StdAfx.h"
#include "SkinnedMeshNode.h"
#include "MeshNode.h"


#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Math/CollisionDetector.h"
#include "Graphics/RscTexture.h"
#include "Graphics/Graphics.h"

#include "Math/Sphere.h"
#include "Foundation/Define.h"
#include "Graphics/Entity.h"

namespace Sophia
{


#define SKINNEDMESH_LASTEST 1

SkinnedMeshNode::SkinnedMeshNode(void)
{
	m_bIsActiveAnimation = true;
	m_pArrayMatBoneRef = NULL;
	m_type = TYPE_SKINNEDMESH;
}

SkinnedMeshNode::~SkinnedMeshNode(void)
{
	Release();
}

void SkinnedMeshNode::LinkToBone(Entity* pEntity)
{
	assert(!m_vecBoneRef.empty());
	D3DXMATRIX tmBoneWorldReferenceInv;

	for (auto iter=m_vecBoneRef.begin() ; iter!=m_vecBoneRef.end() ; iter++)
	{
		BONEREFINFO* pBoneRefInfo=&(*iter);				
		pBoneRefInfo->pNode = pEntity->FindNode(pBoneRefInfo->strNodeName);	
		assert(pBoneRefInfo->pNode!=NULL);	
		// ã�� ���ϴ°�찡 �־�� �ȵ� ��Ʈ ���ý��� boneIndex�� �������Ƿ�		
		D3DXMatrixInverse(&tmBoneWorldReferenceInv,NULL,&pBoneRefInfo->pNode->GetNodeTM());
		pBoneRefInfo->SkinOffset = GetNodeTM() * tmBoneWorldReferenceInv;	// LocalTM = WorldTM * Parent.WorldTM.Inverse
	}			
	m_pArrayMatBoneRef = new D3DXMATRIX[m_vecBoneRef.size()];	
}

/*

�Ϲ� Object, Bone , Skined Mesh ���� �׸�����.
*/
void SkinnedMeshNode::Render()
{	
	m_pRscVetextBuffer->SetStreamSource(0,sizeof(BLENDVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	size_t iBoneRef,nBoneRefSize = m_vecBoneRef.size();
	for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];
		m_pArrayMatBoneRef[iBoneRef] = refItem.SkinOffset * refItem.pNode->GetWorldTM();	// WorldTM = LocalTM * Parent.WorldTM
	}	

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	pEffect->SetMatrixArray(Graphics::m_pInstance->m_hmPalette,m_pArrayMatBoneRef,nBoneRefSize);	
	pEffect->CommitChanges();

	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetCount(),
		m_startIndex,
		m_primitiveCount );			
	
}

void SkinnedMeshNode::BuildComposite(Entity* pEntity)
{	
	cSceneNode::BuildComposite(pEntity);
	LinkToBone(pEntity);		

		assert(m_pRscVetextBuffer!=NULL);
		assert(m_pRscIndexBuffer!=NULL);
		long vertexCount = m_pRscVetextBuffer->GetCount();
		long triangleCount = m_pRscIndexBuffer->GetCount();
		BLENDVERTEX* vertex=(BLENDVERTEX*)m_pRscVetextBuffer->Lock(0,m_pRscVetextBuffer->GetBufferSize(),0);
		TRIANGLE* triangle = (TRIANGLE*)m_pRscIndexBuffer->Lock(0,m_pRscIndexBuffer->GetBufferSize(),0);

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
}

void SkinnedMeshNode::SetBoneRef( std::vector<BONEREFINFO>& vecBoneRef )
{
	m_vecBoneRef = vecBoneRef;
}

void SkinnedMeshNode::QueueRenderer(Entity* pEntity,bool bTraversal)
{
	assert(m_materialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	std::vector<Material*>& sub = m_pRootNode->m_pEntityMaterial->m_ref[m_materialRefIndex];
	assert(m_materialSubIndex < sub.size());
	m_pMaterial = sub[m_materialSubIndex];  

	if (m_bShow)
	{	
		int i = m_pMaterial->index_renderer_queue();
		if (m_pMaterial->AlphaBlend == false)
		{
			pEntity->m_renderQueueSkinned.InsertIntoMeshList(this);
		}
		else
		{
			pEntity->m_renderQueueSkinnedAlphaBlend.InsertIntoMeshList(this);
		}		
	}
	
	if (!bTraversal)
		return;

	for ( auto it_child=m_vecChildNode.begin() ;it_child!=m_vecChildNode.end();++it_child )
	{
		(*it_child)->QueueRenderer(pEntity,bTraversal);
	}
}


void SkinnedMeshNode::Release()
{
	cMeshNode::Release();
	if (m_pArrayMatBoneRef!=NULL)
	{
		delete m_pArrayMatBoneRef;
	}	
	m_vecBoneRef.clear();	
}

void SkinnedMeshNode::SerializeIn( std::ifstream& stream )
{
	// �̹� �տ��� Ÿ���� �о���.
	unsigned short ver = 0;
	unsigned char count =0 ;

	//scene
	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	ReadString(stream,m_strParentName);
	ReadMatrix(stream,m_nodeTM);	
	stream.read((char*)&m_materialRefIndex,sizeof(m_materialRefIndex));
	stream.read((char*)&m_materialSubIndex,sizeof(m_materialSubIndex));
	stream.read((char*)&m_primitiveCount,sizeof(m_primitiveCount));
	stream.read((char*)&m_startIndex,sizeof(m_startIndex));

	// mesh
	SerializeInMesh(stream);

	// child	
	stream.read((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		SCENETYPE type;
		stream.read((char*)&type,sizeof(type));
		cSceneNode* pNode = CreateNode(type);
		pNode->SetRootNode(m_pRootNode);
		pNode->SetParentNode(this);
		pNode->SetParentName(m_strNodeName.c_str());
		AttachChildNode(pNode);
		pNode->SerializeIn(stream);		
	}
}

void SkinnedMeshNode::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = SKINNEDMESH_LASTEST;
	unsigned char count =0 ;

	//scene
	stream.write((char*)&m_type,sizeof(m_type));
	stream.write((char*)&ver,sizeof(ver));
	WriteString(stream,m_strNodeName);
	WriteString(stream,m_strParentName);
	WriteMatrix(stream,m_nodeTM);	
	stream.write((char*)&m_materialRefIndex,sizeof(m_materialRefIndex));
	stream.write((char*)&m_materialSubIndex,sizeof(m_materialSubIndex));	
	stream.write((char*)&m_primitiveCount,sizeof(m_primitiveCount));	
	stream.write((char*)&m_startIndex,sizeof(m_startIndex));	


	// mesh 
	SerializeOutMesh(stream);

	// child
	count = m_vecChildNode.size();
	stream.write((char*)&count,sizeof(count));
	auto it_child = m_vecChildNode.begin();
	for ( ;it_child!=m_vecChildNode.end();++it_child )
	{
		(*it_child)->SerializeOut(stream);
	}	
}

void SkinnedMeshNode::SerializeOutMesh( std::ofstream& stream )
{
	// index
	DWORD bufferSize =0;
	bufferSize = m_pRscIndexBuffer->GetBufferSize();
	stream.write((char*)&bufferSize,sizeof(bufferSize));

	TRIANGLE* pIndices=(TRIANGLE*)m_pRscIndexBuffer->Lock(0,m_pRscIndexBuffer->GetBufferSize(),0);
	stream.write((char*)pIndices,bufferSize);
	m_pRscIndexBuffer->Unlock();		

	//vertex
	bufferSize = m_pRscVetextBuffer->GetBufferSize();
	stream.write((char*)&bufferSize,sizeof(bufferSize));
	BLENDVERTEX* pVertices=(BLENDVERTEX*)m_pRscVetextBuffer->Lock(0,m_pRscVetextBuffer->GetBufferSize(),0);
	stream.write((char*)pVertices,bufferSize);
	m_pRscVetextBuffer->Unlock();	

	// bone info
	unsigned char count;
	count = m_vecBoneRef.size();
	stream.write((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		BONEREFINFO& info = m_vecBoneRef[i];
		WriteString(stream,info.strNodeName); 
	}
}

void SkinnedMeshNode::SerializeInMesh( std::ifstream& stream )
{
	// index
	DWORD bufferSize =0;
	stream.read((char*)&bufferSize,sizeof(bufferSize));
	cRscIndexBuffer* pRscIndexBuffer = cResourceMng::m_pInstance->CreateRscIndexBuffer(m_pRootNode->GetNodeName().c_str(),m_strNodeName.c_str(),bufferSize);
	if(pRscIndexBuffer->GetRefCounter() == 0)
	{
		TRIANGLE* pIndices=(TRIANGLE*)pRscIndexBuffer->Lock(0,pRscIndexBuffer->GetBufferSize(),0);
		stream.read((char*)pIndices,bufferSize);
		pRscIndexBuffer->Unlock();		
		pRscIndexBuffer->SetCount(bufferSize/sizeof(TRIANGLE));
	}
	else
	{
		stream.seekg( bufferSize,std::ios_base::cur);
	}
	SetRscIndexBuffer(pRscIndexBuffer);

	// vertex
	stream.read((char*)&bufferSize,sizeof(bufferSize));
	cRscVertexBuffer* pRscVetextBuffer = cResourceMng::m_pInstance->CreateRscVertexBuffer(m_pRootNode->GetNodeName().c_str(),m_strNodeName.c_str(),bufferSize);
	if(pRscVetextBuffer->GetRefCounter() == 0)
	{
		BLENDVERTEX* pVertices=(BLENDVERTEX*)pRscVetextBuffer->Lock(0,pRscVetextBuffer->GetBufferSize(),0);
		stream.read((char*)pVertices,bufferSize);
		pRscVetextBuffer->Unlock();		
		pRscVetextBuffer->SetCount(bufferSize/sizeof(BLENDVERTEX));
	}
	else
	{
		stream.seekg( bufferSize,std::ios_base::cur);
	}
	SetRscVertextBuffer(pRscVetextBuffer);

	// bone info
	unsigned char count = 0;
	stream.read((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		BONEREFINFO info;
		ReadString(stream,info.strNodeName); 
		m_vecBoneRef.push_back(info);
	}
}

}