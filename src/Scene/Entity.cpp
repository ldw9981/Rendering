#include "StdAfx.h"
#include "Entity.h"
#include "Graphics/Graphics.h"
#include "Graphics/Animation.h"
#include "Graphics/Vertex.h"
#include "Scene/View.h"
#include "Math/CollisionDetector.h"
#include "Resource/ResourceMng.h"

#define ENTITY_LASTEST 1


Entity::Entity(void)
{
	m_strNodeName="Entity";
}


Entity::~Entity(void)
{
	for (auto it=m_vecAnimation.begin();it!=m_vecAnimation.end();++it )
	{
		Animation* pAnimation = *it;
		pAnimation->Release();
	}	
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

	// �ּ����� �ٰ����� �׸������� 
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

bool Entity::Cull( Frustum* pFrustum ,float loose)
{
	if (!m_bRender)
		return false;

	cCollision::STATE retCS=cCollision::CheckWorldFrustum(*pFrustum,m_BoundingSphere,loose);
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
	unsigned char count = 0;
	stream.read((char*)&ver,sizeof(ver));
	ReadString(stream,m_strNodeName);
	float radius = 0.0f;
	stream.read((char*)&radius,sizeof(radius));
	m_BoundingSphere.SetRadius(radius);
	
	stream.read((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		SCENETYPE type;
		stream.read((char*)&type,sizeof(type));
		cSceneNode* pNode = CreateNode(type);
		if (pNode==NULL)
		{
			return;
		}
		pNode->SetRootNode(this);
		pNode->SetParentNode(this);
		pNode->SetParentName(m_strNodeName.c_str());
		AttachChildNode(pNode);
		pNode->SerializeIn(stream);		
	}	
}

void Entity::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = ENTITY_LASTEST;
	unsigned char count = 0;
	stream.write((char*)&ver,sizeof(ver));
	WriteString(stream,m_strNodeName);
	float radius = m_BoundingSphere.GetRadius(); 
	stream.write((char*)&radius,sizeof(radius));

	//child
	count = m_listChildNode.size();
	stream.write((char*)&count,sizeof(count));
	
	for (auto it=m_listChildNode.begin();it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeOut(stream);
	}	
}

bool Entity::SaveScene( const char* fileName )
{
	std::ofstream stream;
	stream.open(fileName, std::ios::out | std::ios::binary);
	SerializeOut(stream);
	return true;
}

bool Entity::LoadScene( const char* fileName )
{
	std::ifstream stream;
	stream.open(fileName, std::ios::in | std::ios::binary);
	SerializeIn(stream);
	return true;
}

void Entity::PushAnimation( Animation* pAnimation )
{
	pAnimation->AddRef();
	m_vecAnimation.push_back(pAnimation);

	for (auto it=m_listChildNode.begin();it!=m_listChildNode.end();++it )
	{
		(*it)->PushAnimation(pAnimation);
	}	
}

void Entity::PopAnimation()
{
	if (m_vecAnimation.empty())
		return;

	Animation* pAnimation = m_vecAnimation[m_vecAnimation.size()-1];
	m_vecAnimation.pop_back();
	pAnimation->Release();

	for (auto it=m_listChildNode.begin();it!=m_listChildNode.end();++it )
	{
		(*it)->PopAnimation();
	}	
}

bool Entity::SaveAnimation( const char* fileName ,int index)
{
	if (index >= (int)m_vecAnimation.size())
		return false;

	std::ofstream stream;
	stream.open(fileName, std::ios::out | std::ios::binary);	

	Animation* pAnimation = m_vecAnimation[index];
	pAnimation->SerializeOut(stream);
	
	return true;
}

bool Entity::LoadAnimation( const char* fileName )
{
	Animation* pAnimation = cResourceMng::m_pInstance->CreateAnimation(fileName);
	if (pAnimation->GetRefCounter()==0)
	{
		std::ifstream stream;
		stream.open(fileName, std::ios::in | std::ios::binary);
		pAnimation->SerializeIn(stream);
	}
	
	PushAnimation(pAnimation);
	return true;
}

bool Entity::SaveMaterial( const char* fileName )
{
	std::ofstream stream;
	stream.open(fileName, std::ios::out | std::ios::binary);	
	for (auto it=m_listChildNode.begin();it!=m_listChildNode.end();++it )
	{
		(*it)->SerializeOut(stream);
	}	
}

bool Entity::LoadMaterial( const char* fileName )
{

}


