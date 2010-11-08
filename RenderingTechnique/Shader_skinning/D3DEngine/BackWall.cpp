#include "stdafx.h"
#include "BackWall.h"

#include "./Geometrics/Plane.h"
#include "TestGameApp.h"


// Custom D3D vertex format used by the vertex buffer
struct BACKWALLVERTEX
{
	float		x; // ��ġ����(x, y, z)
	float		y; // ��ġ����(x, y, z)
	float		z; // ��ġ����(x, y, z)
	float		nx; // ��ġ����(x, y, z)
	float		ny; // ��ġ����(x, y, z)
	float		nz; // ��ġ����(x, y, z)
	float		u;  
	float		v;
};
// �� Ÿ���� �����ϴ�, FVF(Flexible Vertex Type) ����
#define FVF_BACKWALLVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)  
 


BACKWALLVERTEX g_BackWallVertices[]=
{
	{ -250.0f, 500.0f , 250.0f, 	0.0f,0.0f,-1.0f, 0.0f,0.0f	},
	{  250.0f, 500.0f , 250.0f, 	0.0f,0.0f,-1.0f, 1.0f,0.0f	},
	{ -250.0f, 0.0f ,   250.0f, 	0.0f,0.0f,-1.0f, 0.0f,1.0f	},
	{  250.0f, 0.0f ,   250.0f, 	0.0f,0.0f,-1.0f, 1.0f,1.0f	}	
};

WORD		g_BackWallIndices[]=
{
	0,1,2,
	2,1,3
};


cBackWall::cBackWall(cWorld* pWorld)
:cGameObject(pWorld)
{
	m_pVB=NULL; 
	m_pIB=NULL;
	m_pTexture=NULL;
	CreatePlane();
	GetPlane()->Make(D3DXVECTOR3(0.0f,0.0f,-1.0f),250);
}

cBackWall::~cBackWall(void)
{
	DestroyPlane();
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
}

void cBackWall::Init()
{	
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
	m_pArVertex=(LPDWORD)&g_BackWallVertices;
	m_pArIndex=(LPWORD)&g_BackWallIndices;
	m_dwByteVertex=sizeof(g_BackWallVertices);
	m_dwByteIndex=sizeof(g_BackWallIndices);

	//�����Ʈ���� �ʱ�ȭ
	D3DXMatrixIdentity(&m_matWorld);	

	// ! VertexBuffer
	// 1) FVF�� �����ؼ� �ʿ���ũ�⸸ŭ VertexBuffer�� �����.	
	m_pD3DDevice9->CreateVertexBuffer( 
		m_dwByteVertex,
		0,
		FVF_BACKWALLVERTEX,
		D3DPOOL_DEFAULT,
		&m_pVB,
		NULL);


	// 2) ������ Vertex Buffer�� Lock()�ؼ� ������ ��ִ´�.
	VOID* pVertices;
	m_pVB->Lock( 0, m_dwByteVertex, (void**)&pVertices, 0 ) ;
	memcpy(pVertices,m_pArVertex,m_dwByteVertex);
	m_pVB->Unlock();

	// ! IndexBuffer
	// 1) �ʿ��� Index���� ��ŭ�� ũ��� IndexBuffer�� �����.
	m_pD3DDevice9->CreateIndexBuffer( 
		m_dwByteIndex,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pIB,
		NULL);


	// 2) ������ Vertex Buffer�� Lock()�ؼ� ������ ��ִ´�.
	VOID* pIndices;
	m_pIB->Lock( 0, m_dwByteIndex, (void**)&pIndices,0);
	memcpy(pIndices,m_pArIndex,m_dwByteIndex);
	m_pIB->Unlock();

	D3DXCreateTextureFromFile(m_pD3DDevice9,_T("wall.bmp"),&m_pTexture);



}

void cBackWall::Uninit()
{

}

void cBackWall::Update(DWORD elapseTime)
{

}

void cBackWall::Render()
{
	static UINT NumVertices=m_dwByteVertex/sizeof(BACKWALLVERTEX);
	static UINT PrimitiveCount=m_dwByteIndex / (sizeof(WORD)*3);

	m_pD3DDevice9->SetTransform (D3DTS_WORLD, &m_matWorld );
	m_pD3DDevice9->SetTexture (0, m_pTexture );
	m_pD3DDevice9->SetStreamSource( 0, m_pVB, 0,  sizeof(BACKWALLVERTEX) );
	m_pD3DDevice9->SetFVF(FVF_BACKWALLVERTEX);

	m_pD3DDevice9->SetIndices(m_pIB); 
	m_pD3DDevice9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,  0, NumVertices,  0, PrimitiveCount);	
}