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
#include "Scene/RendererQueue.h"

#include "Framework/D3DFramework.h"
#include "Scene/View.h"

#define MESHNODE_LASTEST 1

cMeshNode::cMeshNode(void)
{		
	m_bIsBone= FALSE;
	m_pRscIndexBuffer=NULL;
	m_pRscVetextBuffer=NULL;
}

cMeshNode::~cMeshNode(void)
{
	Release();
}


void cMeshNode::Update(DWORD elapseTime)
{
	cTransformable::Update(elapseTime);
	UpdateWorldMatrix(UpdateTransformAnm(m_animationTime,elapseTime),m_pParentNode);	
	UpdateChildren(elapseTime);
}

/*

	일반 Object, Bone , Skined Mesh 전부 그리고음.
*/

void cMeshNode::Render(unsigned char multiSubIndex)
{
	MultiSub& temp = m_vecMultiSub[multiSubIndex];
	Material& material = m_vecMaterial[temp.materialIndex];

	Graphics::g_pGraphics->GetEffect()->SetMatrix(Graphics::g_pGraphics->m_hmWorld,&m_matWorld);
	Graphics::m_pDevice->SetVertexDeclaration(Graphics::g_pGraphics->m_pVertexDeclationNormal);
	m_pRscVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
	m_pRscIndexBuffer->SetIndices();		

	if( material.GetMapDiffuse() != NULL )	
		Graphics::g_pGraphics->GetEffect()->SetTexture("Tex0",material.GetMapDiffuse()->GetD3DTexture());

	if( material.GetMapNormal() != NULL )	
		Graphics::g_pGraphics->GetEffect()->SetTexture("Tex1",material.GetMapNormal()->GetD3DTexture());

	if( material.GetMapLight() != NULL )
		Graphics::g_pGraphics->GetEffect()->SetTexture("Tex3",material.GetMapLight()->GetD3DTexture());

	Graphics::g_pGraphics->GetEffect()->CommitChanges();
	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetCount(),
		temp.startIndex,
		temp.primitiveCount );
}

void cMeshNode::BuildComposite(Entity* pEntity)
{
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
	cSceneNode::BuildComposite(pEntity);	
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
	if (m_bRender)
	{	
		unsigned char multiSubIndex=0;
		for (auto it_sub=m_vecMultiSub.begin();it_sub!=m_vecMultiSub.end();++it_sub )
		{
			MultiSub& temp = (*it_sub);
			Material& material = m_vecMaterial[temp.materialIndex];		

			int i = material.index_renderer_queue();
			pEntity->m_renderQueueNormal[i].Insert(this,multiSubIndex);
			multiSubIndex++;
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


void cMeshNode::QueueRendererShadow( Entity* pEntity,bool bTraversal )
{
	if (m_bRender)
	{
		unsigned char multiSubIndex=0;
		for (auto it_sub=m_vecMultiSub.begin();it_sub!=m_vecMultiSub.end();++it_sub )
		{
			MultiSub& temp = (*it_sub);
			Material& material = m_vecMaterial[temp.materialIndex];		

			int i = material.index_renderer_queue();
			pEntity->m_renderQueueNormalShadow.Insert(this,multiSubIndex);
			multiSubIndex++;
		}
	}

	if (!bTraversal)
		return;

	auto it_child=m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->QueueRendererShadow(pEntity,bTraversal);
	}
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
	stream >> ver;
	ReadString(stream,m_strNodeName);
	ReadString(stream,m_strParentName);
	ReadMatrix(stream,m_worldReference);	
	
	// child
	unsigned char count =0 ;
	stream >> count;
	for ( int i=0 ; i<count ; i++ )
	{
		SCENETYPE type;
		stream >> type;
		cSceneNode* pNode = CreateNode(type);
		AttachChildNode(pNode);
		pNode->SerializeIn(stream);		
	}
}

void cMeshNode::SerializeOut( std::ofstream& stream )
{
	stream << m_type;
	unsigned short ver = MESHNODE_LASTEST;
	stream << ver;
	WriteString(stream,m_strNodeName);
	WriteString(stream,m_strParentName);
	WriteMatrix(stream,m_worldReference);	

	unsigned char count;
	
	// child
	count = m_listChildNode.size();
	stream << count;
	auto it_child = m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->SerializeOut(stream);
	}	
}

void cMeshNode::SetMaterial( std::vector<Material>& vecMaterial )
{
	m_vecMaterial = vecMaterial;
}
