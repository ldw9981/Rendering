#include "stdafx.h"
#include "Ground.h"

#include "Math/Plane.h"
#include "TestGameApp.h"


// Custom D3D vertex format used by the vertex buffer
struct GROUNDVERTEX
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
#define FVF_GROUNDVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)  


GROUNDVERTEX g_GroundVertices[]=
{
	{ -250.0f, 0.0f , 250.0f, 	0.0f,1.0f,0.0f, 0.0f,0.0f	},
	{  250.0f, 0.0f , 250.0f, 	0.0f,1.0f,0.0f, 1.0f,0.0f	},
	{ -250.0f, 0.0f ,-250.0f, 	0.0f,1.0f,0.0f, 0.0f,1.0f	},
	{  250.0f, 0.0f ,-250.0f, 	0.0f,1.0f,0.0f, 1.0f,1.0f	}	
};

WORD		g_GroundIndices[]=
{
	0,1,2,
	2,1,3
};


cGround::cGround()
{
	m_pVB=NULL; 
	m_pIB=NULL;
	m_pTexture=NULL;
//	CreatePlane();
//	GetPlane()->Make(D3DXVECTOR3(0.0f,1.0f,0.0f),0);
}

cGround::~cGround(void)
{
//	DestroyPlane();
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
}

void cGround::Init(void* arg/*=NULL*/)
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
	m_pArVertex=(LPDWORD)&g_GroundVertices;
	m_pArIndex=(LPWORD)&g_GroundIndices;
	m_dwByteVertex=sizeof(g_GroundVertices);
	m_dwByteIndex=sizeof(g_GroundIndices);

	int size=sizeof(GROUNDVERTEX);
	//�����Ʈ���� �ʱ�ȭ
	D3DXMatrixIdentity(&m_matWorld);	

	// ! VertexBuffer
	// 1) FVF�� �����ؼ� �ʿ���ũ�⸸ŭ VertexBuffer�� �����.	
	m_pD3DDevice->CreateVertexBuffer( 
		m_dwByteVertex,
		0,
		FVF_GROUNDVERTEX,
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
	m_pD3DDevice->CreateIndexBuffer( 
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

	D3DXCreateTextureFromFile(m_pD3DDevice,_T("ground.bmp"),&m_pTexture);
}

void cGround::Uninit()
{

}

void cGround::Update(DWORD elapseTime)
{

}

void cGround::Render()
{	
	static UINT NumVertices=m_dwByteVertex/sizeof(GROUNDVERTEX);
	static UINT PrimitiveCount=m_dwByteIndex / (sizeof(WORD)*3);

	m_pD3DDevice->SetTransform (D3DTS_WORLD, &m_matWorld );
	m_pD3DDevice->SetTexture (0, m_pTexture );
	m_pD3DDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(GROUNDVERTEX) );
	m_pD3DDevice->SetFVF(FVF_GROUNDVERTEX);
	
	m_pD3DDevice->SetIndices(m_pIB); 
	m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,  0, NumVertices,  0, PrimitiveCount);	
}


