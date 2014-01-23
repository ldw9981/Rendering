#include "StdAfx.h"
#include "Entity.h"
#include "Graphics/Graphics.h"
#include "Graphics/Vertex.h"
#include "Scene/View.h"
#include "Math/CollisionDetector.h"

#define ENTITY_LASTEST 1


Entity::Entity(void)
{
	m_strNodeName="Entity";
}


Entity::~Entity(void)
{

}
void Entity::SetBoundingSphere( cSphere& Sphere )
{
	m_BoundingSphere = Sphere;	
}

void Entity::RenderBound()
{
	int LineCount=32;
	float fScale=m_BoundingSphere.GetRadius();
	D3DCOLOR color=D3DCOLOR_RGBA(255,0,0,0);	

	// 최소한의 다각형을 그리기위한 
	if( LineCount < 6 ) 
		LineCount = 6; 

	float fRotationAngleAverage = ( D3DX_PI * 2) / LineCount; 

	LINEVERTEX CircleLine[2]; 
	D3DXVECTOR3 NewPos, Pos; 
	D3DXMATRIX matRot; 
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	Graphics::g_pGraphics->GetEffect()->SetMatrix(Graphics::g_pGraphics->m_hmWorld,&GetWorldTM());
	Graphics::g_pGraphics->GetEffect()->CommitChanges();
	Graphics::m_pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE ); 

	Pos = D3DXVECTOR3( 1.f, 0.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos;
	CircleLine[1].color = CircleLine[0].color = color; 
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 
		D3DXMatrixRotationY( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 
		CircleLine[1].pos = (NewPos);
		Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( LINEVERTEX ) ); 
	} 

	Pos = D3DXVECTOR3( 0.f, 1.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos; 
	CircleLine[1].color = CircleLine[0].color = D3DCOLOR_RGBA(0,255,0,0);	
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 
		D3DXMatrixRotationX( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 
		CircleLine[1].pos = (NewPos); 
		Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( LINEVERTEX ) ); 
	} 	

	Pos = D3DXVECTOR3( 0.f, 1.f, 0.f )* fScale; 
	CircleLine[1].pos = Pos;
	CircleLine[1].color = CircleLine[0].color = D3DCOLOR_RGBA(0,0,255,0);	
	for ( int i = 1; i < LineCount + 1; i++ ) 
	{ 
		CircleLine[0].pos = CircleLine[1].pos; 
		D3DXMatrixRotationZ( &matRot, i * fRotationAngleAverage ); 
		D3DXVec3TransformCoord( &NewPos, &Pos, &matRot ); 
		CircleLine[1].pos = (NewPos); 		
		Graphics::m_pDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, CircleLine, sizeof( LINEVERTEX ) ); 
	} 
}

void Entity::Update( DWORD elapseTime )
{
	cSceneNode::Update(elapseTime);
	m_BoundingSphere.SetCenterPos(D3DXVECTOR3(m_matWorld._41,m_matWorld._42,m_matWorld._43));
}

bool Entity::CullRendererIntoRendererQueue( Frustum* pFrustum )
{
	if (!m_bRender)
		return false;

	cCollision::STATE retCS=cCollision::CheckWorldFrustum(*pFrustum,m_BoundingSphere);
	if( retCS == cCollision::OUTSIDE)
		return false;

	return true;	
}

void Entity::Build()
{
	cSceneNode::BuildComposite(this);
}

void Entity::SerializeIn( std::ifstream& stream )
{
	unsigned short ver = 0;
	stream >> ver;
	ReadString(stream,m_strNodeName);
	float radius = 0.0f;
	stream >> radius;
	m_BoundingSphere.SetRadius(radius);
	unsigned char count = 0;
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

void Entity::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = ENTITY_LASTEST;
	stream << ver;
	WriteString(stream,m_strNodeName);
	float radius = m_BoundingSphere.GetRadius(); 
	stream << radius;
	unsigned char count = m_listChildNode.size();
	stream << count;
	std::list<cSceneNode*>::iterator it=m_listChildNode.begin();
	for ( ;it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeOut(stream);
	}	
}

bool Entity::Save( const char* fileName )
{
	std::ofstream stream;
	stream.open(fileName, std::ios::out | std::ios::binary);
	SerializeOut(stream);
	return true;
}

bool Entity::Load( const char* fileName )
{
	return true;
}


