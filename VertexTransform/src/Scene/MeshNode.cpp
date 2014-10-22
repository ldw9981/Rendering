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
#include "Graphics/MatrixStreamVertexBuffer.h"

namespace Sophia
{

#define MESHNODE_LASTEST 1



cMeshNode::cMeshNode(void)
{	
	m_pRscIndexBuffer=NULL;
	m_pRscVetextBuffer=NULL;
	m_type = TYPE_MESH;
	m_primitiveCount=0;
	m_startIndex=0;
	m_materialRefIndex=0;
	m_materialSubIndex=0;
	m_pMaterial = NULL;
	m_bInstancingEnable = false;
	m_pMatrixStreamVertexBuffer=NULL;
}

cMeshNode::~cMeshNode(void)
{
	Release();
}

/*

	일반 Object, Bone , Skined Mesh 전부 그리고음.
*/

void cMeshNode::Render()
{	
	m_pRscVetextBuffer->SetStreamSource(0,sizeof(NORMALVERTEX));
	m_pRscIndexBuffer->SetIndices();		

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	pEffect->SetMatrix(Graphics::m_pInstance->m_hmWorld,&m_matWorld);

	pEffect->CommitChanges();
	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetCount(),
		m_startIndex,
		m_primitiveCount );
}

void cMeshNode::BuildComposite(Entity* pEntity)
{
	cSceneNode::BuildComposite(pEntity);	

	if (m_bInstancingEnable)
	{
		CreateInstancingResource();
	}

	QueueRenderer(pEntity,false);
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
	assert(m_materialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	std::vector<Material*>& sub = m_pRootNode->m_pEntityMaterial->m_ref[m_materialRefIndex];
	assert(m_materialSubIndex < sub.size());
	m_pMaterial = sub[m_materialSubIndex];  
	
	if (m_bShow)
	{	
		int i = m_pMaterial->index_renderer_queue();
		if (m_pMaterial->AlphaBlend == false)
		{
			pEntity->m_renderQueueNormal.InsertIntoMeshList(this);				
		}
		else
		{
			pEntity->m_renderQueueNormalAlphaBlend.InsertIntoMeshList(this);
		}					
	}

	if (!bTraversal)
		return;

	auto it_child=m_vecChildNode.begin();
	for ( ;it_child!=m_vecChildNode.end();++it_child )
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

	ReleaseInstancingResource();
	SAFE_RELEASE(m_pRscVetextBuffer);	
	SAFE_RELEASE(m_pRscIndexBuffer);		
}

void cMeshNode::SerializeIn( std::ifstream& stream )
{
	// 이미 앞에서 타입은 읽었다.
	unsigned short ver = 0;
	unsigned char count =0 ;

	std::string temp;
	//scene
	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	ReadString(stream,m_strParentName);
	ReadMatrix(stream,m_nodeTM);	
	stream.read((char*)&m_materialRefIndex,sizeof(m_materialRefIndex));
	stream.read((char*)&m_materialSubIndex,sizeof(m_materialSubIndex));
	stream.read((char*)&m_primitiveCount,sizeof(m_primitiveCount));
	stream.read((char*)&m_startIndex,sizeof(m_startIndex));
	ReadBool(stream,m_bInstancingEnable);


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
	stream.write((char*)&m_materialRefIndex,sizeof(m_materialRefIndex));
	stream.write((char*)&m_materialSubIndex,sizeof(m_materialSubIndex));	
	stream.write((char*)&m_primitiveCount,sizeof(m_primitiveCount));	
	stream.write((char*)&m_startIndex,sizeof(m_startIndex));	
	WriteBool(stream,m_bInstancingEnable);

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

void cMeshNode::SerializeOutMesh( std::ofstream& stream )
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
	NORMALVERTEX* pVertices=(NORMALVERTEX*)m_pRscVetextBuffer->Lock(m_pRscVetextBuffer->GetBufferSize(),0);
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
		NORMALVERTEX* pVertices=(NORMALVERTEX*)pRscVetextBuffer->Lock(pRscVetextBuffer->GetBufferSize(),0);
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

Material* cMeshNode::GetMaterial( unsigned char subIndex )
{
	assert(m_materialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	std::vector<Material*>& sub = m_pRootNode->m_pEntityMaterial->m_ref[m_materialRefIndex];
	assert(subIndex < sub.size());
	return sub[subIndex];
}



const std::vector<Material*>& cMeshNode::GetMaterials()
{
	assert(m_materialRefIndex < m_pRootNode->m_pEntityMaterial->m_ref.size());
	return m_pRootNode->m_pEntityMaterial->m_ref[m_materialRefIndex];
}

void cMeshNode::RenderIsntancing()
{
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	pEffect->CommitChanges();
	
	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0, 
		m_pRscVetextBuffer->GetCount(),
		m_startIndex,
		m_primitiveCount );
}

void cMeshNode::SetInstancingEnable( bool val )
{
	m_bInstancingEnable = val;
	if (m_bInstancingEnable)
	{	
		CreateInstancingResource();		
	}
	else
	{
		ReleaseInstancingResource();
	}
}

void cMeshNode::CreateInstancingResource()
{
	if (m_pMatrixStreamVertexBuffer==NULL)
	{
		m_pMatrixStreamVertexBuffer = cResourceMng::m_pInstance->CreateMatrixStreamVertexBuffer(SCENE_KEY(m_pRscVetextBuffer,m_pMaterial,m_pRscIndexBuffer));
		m_pMatrixStreamVertexBuffer->AddRef();
	}
}

void cMeshNode::ReleaseInstancingResource()
{
	SAFE_RELEASE(m_pMatrixStreamVertexBuffer);
}

}