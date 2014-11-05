#include "StdAfx.h"
#include "MatrixTexture.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"
#include "Scene/MeshNode.h"
using namespace Sophia;
MatrixTexture::MatrixTexture(void)
{
	m_levels = 1;
	m_pool = D3DPOOL_DEFAULT;
	m_usage = D3DUSAGE_DYNAMIC;
	m_format = D3DFMT_A32B32G32R32F ;

	m_key.pIndexBuffer = NULL;
	m_key.pMaterial = NULL;
	m_key.pVertexBuffer = NULL;
	m_valid = false;
}


MatrixTexture::~MatrixTexture(void)
{
}

void MatrixTexture::Free()
{
	SAFE_RELEASE(m_pD3DTexture);

	if (m_key.pIndexBuffer != NULL && m_key.pMaterial!=NULL && m_key.pVertexBuffer!=NULL)
	{
		cResourceMng::m_pInstance->EraseBoneStreamTexture(m_key);
	}	
	delete this;
}

void Sophia::MatrixTexture::UpdateMatrix( std::list<cMeshNode*>& list )
{
	auto it = list.begin();
	auto it_end = list.end();
	int size = list.size();
	cMeshNode* pMeshNode = NULL;
	D3DXMATRIX* pDst=NULL;
	DWORD offset_bytes = 0;
	DWORD offset_line = 0;
	DWORD bytesMatrix = sizeof(D3DXMATRIX);
	DWORD bytesPerLine= bytesMatrix * (m_size/4); // 1mat= 4pixel

	D3DLOCKED_RECT lock;	
	m_pD3DTexture->LockRect(0,&lock,NULL,D3DLOCK_DISCARD);

	for ( ; it!=it_end ; it++)
	{
		pMeshNode = *it;	
		pDst = (D3DXMATRIX*)((LPBYTE)lock.pBits + offset_line*lock.Pitch + offset_bytes);						
		memcpy_s((void*)pDst,bytesMatrix,(void*)pMeshNode->GetWorldMatrixPtr(),bytesMatrix);	// 64Byte					
		offset_bytes += bytesMatrix;		
		if (offset_bytes >= bytesPerLine)
		{
			offset_line++;			
			offset_bytes=0;
		}		
	}
	m_pD3DTexture->UnlockRect(0);
}

void Sophia::MatrixTexture::SetSize( DWORD val )
{
	m_size = val;
	m_width = val;
	m_height = val;
}
