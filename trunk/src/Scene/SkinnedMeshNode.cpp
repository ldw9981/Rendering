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

#define SKINNEDMESH_LASTEST 1

SkinnedMeshNode::SkinnedMeshNode(void)
{
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
		// 찾지 못하는경우가 있어서는 안됨 블렌트 버택스에 boneIndex가 들어가있으므로
		pBoneRefInfo->pNode->SetIsBone(true);		// 스킨드 메쉬가 참조하는 노드는 본으로 설정하고 그리지 않는다.
		D3DXMatrixInverse(&tmBoneWorldReferenceInv,NULL,&pBoneRefInfo->pNode->GetNodeTM());
		pBoneRefInfo->SkinOffset = GetNodeTM() * tmBoneWorldReferenceInv;	// LocalTM = WorldTM * Parent.WorldTM.Inverse
	}			
	m_pArrayMatBoneRef = new D3DXMATRIX[m_vecBoneRef.size()];	
}

/*

일반 Object, Bone , Skined Mesh 전부 그리고음.
*/
void SkinnedMeshNode::Render(unsigned char multiSubIndex)
{
	MultiSub& temp = m_vecMultiSub[multiSubIndex];
	Material& material = m_vecSceneMaterial[m_pRootNode->m_indexMaterial]->m_container[temp.materialIndex];

	Graphics::m_pDevice->SetVertexDeclaration(Graphics::g_pGraphics->m_pVertexDeclationBlend);
	m_pRscVetextBuffer->SetStreamSource(sizeof(BLENDVERTEX));
	m_pRscIndexBuffer->SetIndices();			

	int iBoneRef,nBoneRefSize = (int)m_vecBoneRef.size();
	// 현재메쉬가 참조하는 본개수만큼
	for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];
		m_pArrayMatBoneRef[iBoneRef] = refItem.SkinOffset * refItem.pNode->GetWorldTM();	// WorldTM = LocalTM * Parent.WorldTM
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
	cSceneNode::BuildComposite(pEntity);
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
			Material& material = m_vecSceneMaterial[m_pRootNode->m_indexMaterial]->m_container[temp.materialIndex];

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
			Material& material = m_vecSceneMaterial[m_pRootNode->m_indexMaterial]->m_container[temp.materialIndex];

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
	if (m_pArrayMatBoneRef!=NULL)
	{
		delete m_pArrayMatBoneRef;
	}	
	m_vecBoneRef.clear();	
}

void SkinnedMeshNode::SerializeIn( std::ifstream& stream )
{
	// 이미 앞에서 타입은 읽었다.
	unsigned short ver = 0;
	unsigned char count =0 ;

	//scene
	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	ReadString(stream,m_strParentName);
	ReadMatrix(stream,m_nodeTM);	

	// multisub
	stream.read((char*)&count,sizeof(count));
	for ( unsigned char i = 0; i<count ; i++ )
	{
		MultiSub data;
		stream.read((char*)&data.primitiveCount,sizeof(data.primitiveCount));
		stream.read((char*)&data.startIndex,sizeof(data.startIndex));
		stream.read((char*)&data.materialIndex,sizeof(data.materialIndex));
		m_vecMultiSub.push_back(data);
	}

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
	
	// multi/sub
	count = m_vecMultiSub.size();
	stream.write((char*)&count,sizeof(count));
	for ( auto it = m_vecMultiSub.begin(); it!=m_vecMultiSub.end();++it )
	{
		MultiSub& data = (*it);
		stream.write((char*)&data.primitiveCount,sizeof(data.primitiveCount));
		stream.write((char*)&data.startIndex,sizeof(data.startIndex));
		stream.write((char*)&data.materialIndex,sizeof(data.materialIndex));
	}

	// mesh 
	SerializeOutMesh(stream);

	// child
	count = m_listChildNode.size();
	stream.write((char*)&count,sizeof(count));
	auto it_child = m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
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

	TRIANGLE* pIndices=(TRIANGLE*)m_pRscIndexBuffer->Lock();
	stream.write((char*)pIndices,bufferSize);
	m_pRscIndexBuffer->Unlock();		

	//vertex
	bufferSize = m_pRscVetextBuffer->GetBufferSize();
	stream.write((char*)&bufferSize,sizeof(bufferSize));
	BLENDVERTEX* pVertices=(BLENDVERTEX*)m_pRscVetextBuffer->Lock();
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
		TRIANGLE* pIndices=(TRIANGLE*)pRscIndexBuffer->Lock();
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
		BLENDVERTEX* pVertices=(BLENDVERTEX*)pRscVetextBuffer->Lock();
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

