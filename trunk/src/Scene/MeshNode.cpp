#include "StdAfx.h"
#include "MeshNode.h"
#include "ASEParser/ASEParser.h"
#include "ASEParser/ASELexer.h"

#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Math/CollisionDetector.h"
#include "D3D9Server/RscTexture.h"
#include "D3D9Server/Server.h"

#include "Math/Sphere.h"
#include "Foundation/Define.h"

#include "Scene/CameraNode.h"
#include "Scene/RendererQueue.h"

#include "Framework/D3DFramework.h"
#include "Scene/View.h"


cMeshNode::cMeshNode(void)
{		
	m_bIsBone= FALSE;


	m_pRscIndexBuffer=NULL;
	m_pRscVetextBuffer=NULL;

	m_nStartIndex=0;
	m_nPrimitiveCount=0;
}

cMeshNode::~cMeshNode(void)
{
	std::vector<cMeshNode*>::iterator it = m_vecSubMesh.begin();
	std::vector<cMeshNode*>::iterator it_end = m_vecSubMesh.end();

	for ( ;it!=it_end ; it++ )
	{
		delete *it;
	}	
	m_vecSubMesh.clear();

	if (m_pRscVetextBuffer)
	{
		m_pRscVetextBuffer->Release();
		m_pRscVetextBuffer=NULL;
	}
	if (m_pRscIndexBuffer)
	{
		m_pRscIndexBuffer->Release();
		m_pRscIndexBuffer=NULL;
	}
}


void cMeshNode::Update(DWORD elapseTime)
{
	cTransformable::Update(elapseTime);
	UpdateWorldMatrix(UpdateTransformAnm(elapseTime),m_pParentNode);
	m_BoundingSphere.SetCenterPos(D3DXVECTOR3(m_matWorld._41,m_matWorld._42,m_matWorld._43));

	if (!m_vecSubMesh.empty())
	{
		std::vector<cMeshNode*>::iterator it=m_vecSubMesh.begin();
		for ( ;it!=m_vecSubMesh.end();++it )
		{
			(*it)->Update(elapseTime);
		}
	}	

	UpdateChildren(elapseTime);
}

/*

	일반 Object, Bone , Skined Mesh 전부 그리고음.
*/
void cMeshNode::Render()
{			
	//DebugRender();
	D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmWorld,&m_matWorld);
	m_pD3DDevice->SetVertexDeclaration(D3D9::Server::g_pServer->m_pVertexDeclationNormal);
	m_pRscVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
	m_pRscIndexBuffer->SetIndices();		

	if( m_Matrial.GetMapDiffuse() != NULL )	
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex0",m_Matrial.GetMapDiffuse()->GetD3DTexture());

	if( m_Matrial.GetMapNormal() != NULL )	
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex1",m_Matrial.GetMapNormal()->GetD3DTexture());
	
	if( m_Matrial.GetMapLight() != NULL )
		D3D9::Server::g_pServer->GetEffect()->SetTexture("Tex3",m_Matrial.GetMapLight()->GetD3DTexture());
	
	D3D9::Server::g_pServer->GetEffect()->CommitChanges();
	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
			0,  
			0, 
			m_pRscVetextBuffer->GetCount(),
			m_nStartIndex,
			m_nPrimitiveCount );	
}

void cMeshNode::BuildComposite()
{
	if (m_bIsBone)
	{
		m_bRender=false;
	}

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
	}

	cSceneNode::BuildComposite();	
}

/*
	겹치면 자식까지 그냥 다 그린다. 밖이면 자식검사
*/
void cMeshNode::CullRendererIntoRendererQueue(cView* pView,cCameraNode* pActiveCamera )
{
	cCollision::STATE retCS=pActiveCamera->CheckWorldFrustum(m_BoundingSphere);
	if( retCS != cCollision::OUTSIDE)
	{			
		if (m_bRender)
		{
			QueueRenderer(pView,true);
			return;
		}
	}	

	std::list<cSceneNode*>::iterator it_child=m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->CullRendererIntoRendererQueue(pView,pActiveCamera);
	}
}

void cMeshNode::AddMultiSub( cMeshNode* mesh )
{
	m_vecSubMesh.push_back(mesh);
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

void cMeshNode::QueueRenderer(cView* pView,bool bTraversal)
{
	int i = m_Matrial.index_renderer_queue();
	
	pView->m_listRenderQueue[i].Insert(this);

	if (!bTraversal)
		return;

	std::vector<cMeshNode*>::iterator it_sub=m_vecSubMesh.begin();
	for ( ;it_sub!=m_vecSubMesh.end();++it_sub )
	{
		(*it_sub)->QueueRenderer(pView,bTraversal);
	}

	std::list<cSceneNode*>::iterator it_child=m_listChildNode.begin();
	for ( ;it_child!=m_listChildNode.end();++it_child )
	{
		(*it_child)->QueueRenderer(pView,bTraversal);
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

void cMeshNode::DebugRender() 
{ 
	int LineCount=16;
	float fScale=m_BoundingSphere.GetRadius();
	D3DCOLOR color=D3DCOLOR_RGBA(255,0,255,0);	

	// 최소한의 다각형을 그리기위한 
	if( LineCount < 6 ) 
		LineCount = 6; 

	float fRotationAngleAverage = ( D3DX_PI * 2) / LineCount; 

	struct sDrawCircle 
	{ 
		D3DXVECTOR3 pos; 
		D3DCOLOR color; 
	}; 

	sDrawCircle CircleLine[2]; 
	D3DXVECTOR3 NewPos, Pos; 
	D3DXMATRIXA16 matRot; 
	
	//mat.m[3][0] = GetWorldTM().[3][0];
	D3D9::Server::g_pServer->GetEffect()->SetMatrix(D3D9::Server::g_pServer->m_hmWorld,&GetWorldTM());
	D3D9::Server::g_pServer->GetEffect()->CommitChanges();

	// m_vtCircleCenter : Circle의 중심점 
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE ); 
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); 
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); 

	Pos = D3DXVECTOR3( 1.f, 0.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos;// + m_pBoundingSphere->GetCenterPos(); 
	CircleLine[1].color = CircleLine[0].color = color; 
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 

		// 회전하고자 하는 축에 따라 마음대로 
		D3DXMatrixRotationY( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 

		CircleLine[1].pos = (NewPos);// + m_pBoundingSphere->GetCenterPos() ); 

		m_pD3DDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE ); 
		
		m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( sDrawCircle ) ); 
	} 
	
	
	Pos = D3DXVECTOR3( 0.f, 1.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos; 
	CircleLine[1].color = CircleLine[0].color = color; 
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 

		// 회전하고자 하는 축에 따라 마음대로 
		D3DXMatrixRotationX( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 

		CircleLine[1].pos = (NewPos); 

		m_pD3DDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE ); 
		m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( sDrawCircle ) ); 
	} 

	/*
	Pos = D3DXVECTOR3( 0.f, 1.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos + m_pBoundingSphere->GetCenterPos(); 
	CircleLine[1].color = CircleLine[0].color = color; 
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 

		// 회전하고자 하는 축에 따라 마음대로 
		D3DXMatrixRotationZ( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 

		CircleLine[1].pos = (NewPos + m_pBoundingSphere->GetCenterPos() ); 

		m_pD3DDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE ); 
		m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( sDrawCircle ) ); 
	} 
	*/

	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE ); 
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE ); 
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE ); 
	

}


