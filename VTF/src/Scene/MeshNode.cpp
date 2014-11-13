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

#include "Graphics/VertexTexture.h"
#include "Graphics/VertexInstancingBuffer.h"
#include "Graphics/MatrixTexture.h"
#include "Graphics/IndexInstancingBuffer.h"

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
	m_pMatrixTexture=NULL;
	m_pVertexInstancingBuffer = NULL;
	m_pIndexInstancingBuffer=NULL;
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
	m_pRscVetextBuffer->SetStreamSource(0,sizeof(NORMAL_VERTEX));
	m_pRscIndexBuffer->SetIndices();		

	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	pEffect->SetMatrix(Graphics::m_pInstance->m_hmWorld,&m_matWorld);

	pEffect->CommitChanges();
	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetVertexCount(),
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
	WriteString(stream,m_pRscIndexBuffer->GetUniqueKey());
	DWORD bufferSize =0;
	bufferSize = m_pRscIndexBuffer->GetBufferSize();
	stream.write((char*)&bufferSize,sizeof(bufferSize));
	TRIANGLE_INDEX16* pIndices=(TRIANGLE_INDEX16*)m_pRscIndexBuffer->Lock(m_pRscIndexBuffer->GetBufferSize(),0);
	stream.write((char*)pIndices,bufferSize);
	m_pRscIndexBuffer->Unlock();		

	//vertex
	WriteString(stream,m_pRscVetextBuffer->GetUniqueKey());
	bufferSize = m_pRscVetextBuffer->GetBufferSize();
	stream.write((char*)&bufferSize,sizeof(bufferSize));
	NORMAL_VERTEX* pVertices=(NORMAL_VERTEX*)m_pRscVetextBuffer->Lock(m_pRscVetextBuffer->GetBufferSize(),0);
	stream.write((char*)pVertices,bufferSize);
	m_pRscVetextBuffer->Unlock();	

}

void cMeshNode::SerializeInMesh( std::ifstream& stream )
{
	// index
	std::string strKey;
	ReadString(stream,strKey);
	DWORD bufferSize =0;
	stream.read((char*)&bufferSize,sizeof(bufferSize));
	cRscIndexBuffer* pRscIndexBuffer = cResourceMng::m_pInstance->CreateRscIndexBuffer(strKey,bufferSize);
	if(pRscIndexBuffer->GetRefCounter() == 0)
	{
		TRIANGLE_INDEX16* pIndices=(TRIANGLE_INDEX16*)pRscIndexBuffer->Lock(pRscIndexBuffer->GetBufferSize(),0);
		stream.read((char*)pIndices,bufferSize);
		pRscIndexBuffer->Unlock();		
		pRscIndexBuffer->SetTriangleCount(bufferSize/sizeof(TRIANGLE_INDEX16));
	}
	else
	{
		stream.seekg( bufferSize,std::ios_base::cur);
	}
	SetRscIndexBuffer(pRscIndexBuffer);

	// vertex
	ReadString(stream,strKey);
	stream.read((char*)&bufferSize,sizeof(bufferSize));
	cRscVertexBuffer* pRscVetextBuffer = cResourceMng::m_pInstance->CreateRscVertexBuffer(strKey,bufferSize);
	if(pRscVetextBuffer->GetRefCounter() == 0)
	{
		NORMAL_VERTEX* pVertices=(NORMAL_VERTEX*)pRscVetextBuffer->Lock(pRscVetextBuffer->GetBufferSize(),0);
		stream.read((char*)pVertices,bufferSize);
		pRscVetextBuffer->Unlock();		
		pRscVetextBuffer->SetVertexCount(bufferSize/sizeof(NORMAL_VERTEX));
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


void cMeshNode::RenderInstancing( int vertexCount,int triangleCount )
{
	HRESULT hr;
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	V(Graphics::m_pDevice->SetStreamSource(0,m_pVertexInstancingBuffer->GetD3DVertexBuffer(),0, sizeof(NORMAL_VERTEX_INSTANCEDATA)));		
	V(Graphics::m_pDevice->SetIndices(m_pIndexInstancingBuffer->GetD3DIndexBuffer())); 
	UINT passes;
	V(pEffect->Begin(&passes, 0));			
	V(pEffect->BeginPass(0));
	pEffect->CommitChanges();
	V(Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0, vertexCount,0, triangleCount) );
	V(pEffect->EndPass());		
	V(pEffect->End());		
}

void cMeshNode::ChangeInstancingEnable( bool val )
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
	HRESULT hr=0;
	if (m_pVertexInstancingBuffer == NULL)
	{
		DWORD size = sizeof(NORMAL_VERTEX_INSTANCEDATA) * m_pRscVetextBuffer->GetVertexCount() * INSTANCING_MAX;
		m_pVertexInstancingBuffer = cResourceMng::m_pInstance->CreateVertexInstancingBuffer(m_pRscVetextBuffer,size,m_pRscVetextBuffer->GetVertexCount()*INSTANCING_MAX);
		m_pVertexInstancingBuffer->AddRef();
		if (m_pVertexInstancingBuffer->GetRefCounter()==1)
		{
			NORMAL_VERTEX_INSTANCEDATA* pDstLockPos = (NORMAL_VERTEX_INSTANCEDATA*)m_pVertexInstancingBuffer->Lock(0,0);
			NORMAL_VERTEX* pSrcLockPos = (NORMAL_VERTEX*)m_pRscVetextBuffer->Lock(0,0);

			int vertexSize = m_pRscVetextBuffer->GetVertexCount();

			for( int instanceIndex = 0 ; instanceIndex < INSTANCING_MAX; instanceIndex++ )
			{
				NORMAL_VERTEX* pSrcPos = pSrcLockPos;			
				for (int vertexIndex = 0 ;vertexIndex<vertexSize;vertexIndex++)
				{
					pDstLockPos->vertex = *pSrcPos;

					pDstLockPos->vertexIndex = (float)vertexIndex;
					pDstLockPos->vertexSize =  (float)vertexSize;
					pDstLockPos->instanceIndex = (float)instanceIndex;
	
					pSrcPos++;
					pDstLockPos++;
				}	 
			}
			m_pRscVetextBuffer->Unlock();
			m_pVertexInstancingBuffer->Unlock();
		}		
	}
	if (m_pIndexInstancingBuffer==NULL)
	{
		DWORD bufferSize = sizeof(TRIANGLE_INDEX32) * m_pRscIndexBuffer->GetTriangleCount() * INSTANCING_MAX;
		DWORD triangleCount = m_pRscIndexBuffer->GetTriangleCount()*INSTANCING_MAX;
		m_pIndexInstancingBuffer = cResourceMng::m_pInstance->CreateIndexInstancingBuffer(m_pRscIndexBuffer,bufferSize,triangleCount);
		m_pIndexInstancingBuffer->AddRef();				
		if (m_pIndexInstancingBuffer->GetRefCounter()==1)
		{
			TRIANGLE_INDEX16* pSrcLockPos = (TRIANGLE_INDEX16*)m_pRscIndexBuffer->Lock(0,0);
			TRIANGLE_INDEX32* pDstLockPos = (TRIANGLE_INDEX32*)m_pIndexInstancingBuffer->Lock(0,0);

			for( int instanceIndex = 0 ; instanceIndex < INSTANCING_MAX; instanceIndex++ )
			{
				for (int i=0 ; i< m_pRscIndexBuffer->GetTriangleCount() ;i++ )
				{
					pDstLockPos[instanceIndex*m_pRscIndexBuffer->GetTriangleCount()+i].index[0] = pSrcLockPos[i].index[0] + instanceIndex* m_pRscVetextBuffer->GetVertexCount();	// 
					pDstLockPos[instanceIndex*m_pRscIndexBuffer->GetTriangleCount()+i].index[1] = pSrcLockPos[i].index[1] + instanceIndex* m_pRscVetextBuffer->GetVertexCount();	// 
					pDstLockPos[instanceIndex*m_pRscIndexBuffer->GetTriangleCount()+i].index[2] = pSrcLockPos[i].index[2] + instanceIndex* m_pRscVetextBuffer->GetVertexCount();	//
				}	 
			}	
			m_pIndexInstancingBuffer->Unlock();
			m_pRscIndexBuffer->Unlock();
		}
	}


// 	if (m_pVertexTexture == NULL)
// 	{
// 		DWORD size =(DWORD) pow(2.0f,ceil(log(sqrt((float) m_pRscVetextBuffer->GetVertexCount() * INSTANCING_MAX))/log(2.0f)));
// 		m_pVertexTexture = cResourceMng::m_pInstance->CreateVertexTexture(SCENE_KEY(m_pRscVetextBuffer,m_pMaterial,m_pRscIndexBuffer),size);
// 		m_pVertexTexture->AddRef();
// 	}

	if (m_pMatrixTexture==NULL)
	{
		DWORD size = (DWORD) pow(2.0f,ceil(log(sqrt((float) INSTANCING_MAX*4 ))/log(2.0f)));
		m_pMatrixTexture = cResourceMng::m_pInstance->CreateMatrixTexture(SCENE_KEY(m_pRscVetextBuffer,m_pMaterial,m_pRscIndexBuffer),size);
		m_pMatrixTexture->AddRef();
	}
}

void cMeshNode::ReleaseInstancingResource()
{
	SAFE_RELEASE(m_pVertexInstancingBuffer);
	SAFE_RELEASE(m_pMatrixTexture);
	SAFE_RELEASE(m_pIndexInstancingBuffer);
}



void cMeshNode::UpdateMatrixTexture( std::list<cMeshNode*>& list )
{
	auto it = list.begin();
	auto it_end = list.end();
	int size = list.size();
	cMeshNode* pMeshNode = NULL;
	D3DXMATRIX* pDst=NULL;
	DWORD offset_bytes = 0;
	DWORD offset_line = 0;
	DWORD bytesMatrix = sizeof(D3DXMATRIX);
	DWORD bytesPerLine= bytesMatrix * (m_pMatrixTexture->GetSize()/4); // 1mat= 4pixel

	D3DLOCKED_RECT lock;	
	m_pMatrixTexture->GetD3DTexture()->LockRect(0,&lock,NULL,D3DLOCK_DISCARD);

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
	m_pMatrixTexture->GetD3DTexture()->UnlockRect(0);
}

}