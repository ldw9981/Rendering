#include "StdAfx.h"
#include "MeshNode.h"
#include "ASEParser/ASEParser.h"
#include "ASEParser/ASELexer.h"

#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Graphics/RscTexture.h"
#include "Graphics/Graphics.h"

#include "Math/Sphere.h"
#include "Foundation/Define.h"

#include "Scene/CameraNode.h"
#include "Graphics/RendererQueue.h"
#include "Graphics/Entity.h"

namespace Sophia
{

#define MESHNODE_LASTEST 1



cMeshNode::cMeshNode(void)
{	
	m_pRscIndexBuffer=NULL;
	m_pRscVetextBuffer=NULL;
	m_type = TYPE_MESH;
	m_nMaterialRefIndex = 0;
}

cMeshNode::~cMeshNode(void)
{
	Release();
}

/*

	일반 Object, Bone , Skined Mesh 전부 그리고음.
*/

void cMeshNode::Render(MultiSub* pMultiSub,Material* pMaterial)
{
	MultiSub& multiSub = *pMultiSub;
	Material& material = *pMaterial;
		
	
	m_pRscVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
	m_pRscIndexBuffer->SetIndices();		

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	pEffect->SetMatrix(Graphics::m_pInstance->m_hmWorld,&m_matWorld);

	pEffect->CommitChanges();
	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetCount(),
		multiSub.startIndex,
		multiSub.primitiveCount );
}

void cMeshNode::BuildComposite(Entity* pEntity)
{
	cSceneNode::BuildComposite(pEntity);	
	assert(m_pRscVetextBuffer!=NULL);
	assert(m_pRscIndexBuffer!=NULL);
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

/*
	겹치면 자식까지 그냥 다 그린다. 밖이면 자식검사
	* 논리적오류! 자식이라고해서 원 안에 항상있을까..
*/

void cMeshNode::AddMultiSub( WORD startIndex,WORD primitiveCount,unsigned char materialIndex )
{
	MultiSub temp;
	temp.startIndex = startIndex;
	temp.primitiveCount = primitiveCount;
	temp.materialIndex = materialIndex;
	m_vecMultiSub.push_back(temp);
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

void cMeshNode::QueueRenderer(Entity* pEntity,bool bTraversal)
{
	if (m_bShow)
	{	
		unsigned char multiSubIndex=0;
		for (auto it_sub=m_vecMultiSub.begin();it_sub!=m_vecMultiSub.end();++it_sub )
		{
			MultiSub* pMultiSub = & (*it_sub);
			Material* pMaterial = GetMaterial(pMultiSub);

			int i = pMaterial->index_renderer_queue();

			if (pMaterial->AlphaBlend == false)
			{
				pEntity->m_renderQueueNormal.Insert(this,pMultiSub,pMaterial);				
			}
			else
			{
				pEntity->m_renderQueueNormalAlphaBlend.Insert(this,pMultiSub,pMaterial);
			}			
		}
	}

	if (!bTraversal)
		return;

	auto it_child=m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->QueueRenderer(pEntity,bTraversal);
	}
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


void cMeshNode::Release()
{
	cSceneNode::Release();

	SAFE_RELEASE(m_pRscVetextBuffer);	
	SAFE_RELEASE(m_pRscIndexBuffer);	
}

void cMeshNode::SerializeIn( std::ifstream& stream )
{
	// 이미 앞에서 타입은 읽었다.
	unsigned short ver = 0;
	unsigned char count =0 ;

	//scene
	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	ReadString(stream,m_strParentName);
	ReadMatrix(stream,m_nodeTM);	
	stream.read((char*)&m_nMaterialRefIndex,sizeof(m_nMaterialRefIndex));

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

void cMeshNode::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = MESHNODE_LASTEST;
	unsigned char count =0 ;

	//scene
	stream.write((char*)&m_type,sizeof(m_type));	
	stream.write((char*)&ver,sizeof(ver));	
	WriteString(stream,m_strNodeName);
	WriteString(stream,m_strParentName);
	WriteMatrix(stream,m_nodeTM);	
	stream.write((char*)&m_nMaterialRefIndex,sizeof(m_nMaterialRefIndex));

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

void cMeshNode::SerializeOutMesh( std::ofstream& stream )
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
	NORMALVERTEX* pVertices=(NORMALVERTEX*)m_pRscVetextBuffer->Lock();
	stream.write((char*)pVertices,bufferSize);
	m_pRscVetextBuffer->Unlock();	

}

void cMeshNode::SerializeInMesh( std::ifstream& stream )
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
		NORMALVERTEX* pVertices=(NORMALVERTEX*)pRscVetextBuffer->Lock();
		stream.read((char*)pVertices,bufferSize);
		pRscVetextBuffer->Unlock();		
		pRscVetextBuffer->SetCount(bufferSize/sizeof(NORMALVERTEX));
	}
	else
	{
		stream.seekg( bufferSize,std::ios_base::cur);
	}
	SetRscVertextBuffer(pRscVetextBuffer);
}

Material* cMeshNode::GetMaterial( MultiSub* pMultiSub )
{
	assert(m_nMaterialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	std::vector<Material*>& sub = m_pRootNode->m_pEntityMaterial->m_ref[m_nMaterialRefIndex];
	assert(pMultiSub->materialIndex < sub.size());
	return sub[pMultiSub->materialIndex];
}

Material* cMeshNode::GetMaterial( unsigned char subIndex )
{
	assert(m_nMaterialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	std::vector<Material*>& sub = m_pRootNode->m_pEntityMaterial->m_ref[m_nMaterialRefIndex];
	assert(subIndex < sub.size());
	return sub[subIndex];
}



const std::vector<Material*>& cMeshNode::GetMaterials()
{
	assert(m_nMaterialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	return m_pRootNode->m_pEntityMaterial->m_ref[m_nMaterialRefIndex];
}

}