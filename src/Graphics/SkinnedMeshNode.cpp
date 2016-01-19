#include "StdAfx.h"
#include "SkinnedMeshNode.h"
#include "MeshNode.h"


#include "Foundation/Trace.h"
#include "Math/Sphere.h"
#include "Math/CollisionDetector.h"
#include "Graphics/RscTexture.h"
#include "Graphics/Graphics.h"

#include "Math/Sphere.h"
#include "Foundation/Define.h"
#include "Graphics/Entity.h"

#include "Graphics/MatrixTexture.h"
#include "Graphics/InstanceDataBuffer.h"

namespace Sophia
{


#define SKINNEDMESH_LASTEST 1

SkinnedMeshNode::SkinnedMeshNode(void)
{
	m_bIsActiveAnimation = true;
	m_pMatrixPalleteTexture = NULL;
	m_type = TYPE_SKINNEDMESH;
	

}

SkinnedMeshNode::~SkinnedMeshNode(void)
{
	Release();
}

void SkinnedMeshNode::LinkToBone(Entity* pEntity)
{
	assert(!m_vecBoneRef.empty());
	D3DXMATRIX tmBoneWorldReferenceInv;
		
	size_t iBoneRef=0,nBoneRefSize =m_vecBoneRef.size() ;	

	for (iBoneRef=0;iBoneRef<nBoneRefSize;iBoneRef++)
	{
		BONEREFINFO& refItem=m_vecBoneRef[iBoneRef];

		refItem.pNode = pEntity->FindNode(refItem.strNodeName);	
		assert(refItem.pNode!=NULL);	
		// 찾지 못하는경우가 있어서는 안됨 블렌트 버택스에 boneIndex가 들어가있으므로		
		D3DXMatrixInverse(&tmBoneWorldReferenceInv,NULL,&refItem.pNode->GetNodeTM());
		refItem.SkinOffset = GetNodeTM() * tmBoneWorldReferenceInv;	// LocalTM = WorldTM * Parent.WorldTM.Inverse		
	}

}

/*

일반 Object, Bone , Skined Mesh 전부 그리고음.
*/
void SkinnedMeshNode::Render()
{	
	HRESULT hr;
	m_pRscVetextBuffer->SetStreamSource(0,sizeof(BLEND_VERTEX));
	m_pRscIndexBuffer->SetIndices();				
	LPD3DXEFFECT pEffect = Graphics::m_pInstance->GetEffect();
	if (!m_updateMatrixPallete)
	{
		UpdateMatrixPallete();
	}	

	pEffect->SetTexture("Tex_MatrixPallete",m_pMatrixPalleteTexture->GetD3DTexture());
	HR_V(pEffect->CommitChanges());	

	HR_V(Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
		0,  
		0, 
		m_pRscVetextBuffer->GetVertexCount(),
		0,
		m_pRscIndexBuffer->GetTriangleCount() ));
	
}

void SkinnedMeshNode::BuildComposite(Entity* pEntity)
{	
	cSceneNode::BuildComposite(pEntity);
	LinkToBone(pEntity);		
	if (m_bInstancingEnable)
	{
		CreateInstancingResource();
	}
	else
	{
		CreateMatrixPallete();
	}

	GatherRender(pEntity,false);
}

void SkinnedMeshNode::SetBoneRef( std::vector<BONEREFINFO>& vecBoneRef )
{
	m_vecBoneRef = vecBoneRef;
}

void SkinnedMeshNode::GatherRender(Entity* pEntity,bool bTraversal)
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
			pEntity->m_vecSkinned.push_back(this);
		}
		else
		{
			pEntity->m_vecSkinnedAlphaBlend.push_back(this);
		}		
	}
	
	if (!bTraversal)
		return;

	for ( auto it_child=m_vecChildNode.begin() ;it_child!=m_vecChildNode.end();++it_child )
	{
		(*it_child)->GatherRender(pEntity,bTraversal);
	}
}


void SkinnedMeshNode::Release()
{
	cMeshNode::Release();
	DeleteMatrixPallete();
	m_vecBoneRef.clear();	
	ReleaseInstancingResource();
}

void SkinnedMeshNode::SerializeIn( std::ifstream& stream )
{
	// 이미 앞에서 타입은 읽었다.
	unsigned short ver = 0;
	unsigned char count =0 ;

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

void SkinnedMeshNode::SerializeOut( std::ofstream& stream )
{
	unsigned short ver = SKINNEDMESH_LASTEST;
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

void SkinnedMeshNode::SerializeOutMesh( std::ofstream& stream )
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
	BLEND_VERTEX* pVertices=(BLEND_VERTEX*)m_pRscVetextBuffer->Lock(m_pRscVetextBuffer->GetBufferSize(),0);
	stream.write((char*)pVertices,bufferSize);
	m_pRscVetextBuffer->Unlock();	

	// bone info
	unsigned char count;
	count = m_vecBoneRef.size();
	stream.write((char*)&count,sizeof(count));
	for ( int i=0 ; i<count ; i++ )
	{
		BONEREFINFO& info = m_vecBoneRef[i];
		WriteString(stream,info.strNodeName); 
	}
}

void SkinnedMeshNode::SerializeInMesh( std::ifstream& stream )
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
		BLEND_VERTEX* pVertices=(BLEND_VERTEX*)pRscVetextBuffer->Lock(pRscVetextBuffer->GetBufferSize(),0);
		stream.read((char*)pVertices,bufferSize);
		pRscVetextBuffer->Unlock();		
		pRscVetextBuffer->SetVertexCount(bufferSize/sizeof(BLEND_VERTEX));
	}
	else
	{
		stream.seekg( bufferSize,std::ios_base::cur);
	}
	SetRscVertextBuffer(pRscVetextBuffer);

	// bone info
	unsigned char count = 0;
	stream.read((char*)&count,sizeof(count));
	if(count<512)
		m_vecBoneRef.reserve(count);

	for ( int i=0 ; i<count ; i++ )
	{
		BONEREFINFO info;
		ReadString(stream,info.strNodeName); 
		m_vecBoneRef.push_back(info);
	}
}

void SkinnedMeshNode::Update( DWORD elapseTime )
{
	cMeshNode::Update(elapseTime);
	m_updateMatrixPallete = false;
}

void SkinnedMeshNode::UpdateMatrixPallete()
{
	D3DXMATRIX* pDst=NULL;
	DWORD offset_bytes = 0;
	DWORD offset_line = 0;
	DWORD bytesMatrix = sizeof(D3DXMATRIX);
	
	D3DLOCKED_RECT lock;	
	m_pMatrixPalleteTexture->GetD3DTexture()->LockRect(0,&lock,NULL,D3DLOCK_DISCARD);

	DWORD boneSize = m_vecBoneRef.size();
	for (DWORD boneIndex=0;boneIndex<boneSize;boneIndex++)
	{			
		pDst = (D3DXMATRIX*)((LPBYTE)lock.pBits + offset_line*lock.Pitch + offset_bytes);						
		BONEREFINFO& refItem=m_vecBoneRef[boneIndex];
		// = refItem.SkinOffset * refItem.pNode->GetWorldTM();	// WorldTM = LocalTM * Parent.WorldTM
		D3DXMatrixMultiply(pDst,&refItem.SkinOffset,&refItem.pNode->m_matWorld);	
		offset_bytes += bytesMatrix;		
	}	

	m_pMatrixPalleteTexture->GetD3DTexture()->UnlockRect(0);
	m_updateMatrixPallete = true;
}

void SkinnedMeshNode::CreateInstancingResource()
{
	HRESULT hr=0;
	int instancingMax = Graphics::m_pInstance->GetEntityInstancingMax(m_pRootNode->GetNodeName().c_str());
	if (m_pInstanceDataBuffer == NULL)
	{
		DWORD size = sizeof(INSTANCEDATA) * instancingMax;
		m_pInstanceDataBuffer = cResourceMng::m_pInstance->CreateInstanceDataBuffer(m_pRscVetextBuffer,size,instancingMax);
		m_pInstanceDataBuffer->AddRef();
		if (m_pInstanceDataBuffer->GetRefCounter()==1)
		{
			INSTANCEDATA* pDstLockPos = (INSTANCEDATA*)m_pInstanceDataBuffer->Lock(0,0);
			for( int instanceIndex = 0 ; instanceIndex < instancingMax; instanceIndex++ )
			{
				pDstLockPos->instanceIndex = (float)instanceIndex;	
				pDstLockPos->boneSize = (float)m_vecBoneRef.size();
				pDstLockPos++;				
			}
			m_pInstanceDataBuffer->Unlock();
		}		
	}

	if (m_pMatrixInstancingTexture==NULL)
	{
		size_t nBoneRefSize = m_vecBoneRef.size();
		DWORD size = (DWORD) pow(2.0f,ceil(log(sqrt((float) 4*nBoneRefSize *instancingMax ))/log(2.0f)));
		m_pMatrixInstancingTexture = cResourceMng::m_pInstance->CreateMatrixTexture(SCENE_KEY(m_pRscVetextBuffer,m_pMaterial,m_pRscIndexBuffer),size);
		m_pMatrixInstancingTexture->AddRef();
	}
}

void SkinnedMeshNode::RenderInstancing( int instanceSize )
{
	m_pRscVetextBuffer->SetStreamSource(0,D3DXGetDeclVertexSize(declBlendInstance,0));
	m_pRscVetextBuffer->SetStreamSourceFreq(0,D3DSTREAMSOURCE_INDEXEDDATA | instanceSize);		
	m_pInstanceDataBuffer->SetStreamSource(1,D3DXGetDeclVertexSize(declBlendInstance,1));
	m_pInstanceDataBuffer->SetStreamSourceFreq(1,D3DSTREAMSOURCE_INSTANCEDATA|1);
	m_pRscIndexBuffer->SetIndices();
	Graphics::m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,m_pRscVetextBuffer->GetVertexCount(),m_startIndex,m_primitiveCount );
}



void SkinnedMeshNode::UpdateMatrixInstancing( std::list<cMeshNode*>& list )
{
	auto it = list.begin();
	auto it_end = list.end();
	int size = list.size();
	SkinnedMeshNode* pMeshNode = NULL;
	D3DXMATRIX* pDst=NULL;
	DWORD offset_bytes = 0;
	DWORD offset_line = 0;
	DWORD bytesMatrix = sizeof(D3DXMATRIX);
	DWORD bytesPerLine= bytesMatrix * (m_pMatrixInstancingTexture->GetSize()/4); // 1mat= 4pixel

	D3DLOCKED_RECT lock;	
	m_pMatrixInstancingTexture->GetD3DTexture()->LockRect(0,&lock,NULL,D3DLOCK_DISCARD);

	for ( ; it!=it_end ; it++)
	{		
		pMeshNode =  static_cast<SkinnedMeshNode*>(*it);	
		auto& refArrBone = pMeshNode->GetArrayBoneRef();
		DWORD boneSize = refArrBone.size();
		for (DWORD boneIndex=0;boneIndex<boneSize;boneIndex++)
		{			
			pDst = (D3DXMATRIX*)((LPBYTE)lock.pBits + offset_line*lock.Pitch + offset_bytes);						
			BONEREFINFO& refItem=refArrBone[boneIndex];
			// = refItem.SkinOffset * refItem.pNode->GetWorldTM();	// WorldTM = LocalTM * Parent.WorldTM
			D3DXMatrixMultiply(pDst,&refItem.SkinOffset,&refItem.pNode->m_matWorld);		
			
			offset_bytes += bytesMatrix;		
			if (offset_bytes >= bytesPerLine)
			{
				offset_line++;			
				offset_bytes=0;
			}	
		}	
	}
	m_pMatrixInstancingTexture->GetD3DTexture()->UnlockRect(0);
}

void SkinnedMeshNode::CreateMatrixPallete()
{
	assert(m_pMatrixPalleteTexture==NULL);
	size_t nBoneRefSize = m_vecBoneRef.size();
	size_t nBoneMax = 256;
	assert(nBoneRefSize<nBoneMax);
	m_pMatrixPalleteTexture = new cRscTexture;
	m_pMatrixPalleteTexture->SetWidth(nBoneMax*4);
	m_pMatrixPalleteTexture->SetHeight(1);
	m_pMatrixPalleteTexture->SetPool(D3DPOOL_DEFAULT);
	m_pMatrixPalleteTexture->SetUsage(D3DUSAGE_DYNAMIC);
	m_pMatrixPalleteTexture->SetFormat(D3DFMT_A32B32G32R32F);	
	m_pMatrixPalleteTexture->Create();
}

void SkinnedMeshNode::DeleteMatrixPallete()
{
	m_pMatrixPalleteTexture->Free();
	m_pMatrixPalleteTexture=NULL;
}

void SkinnedMeshNode::ChangeInstancingEnable( bool val )
{
	m_bInstancingEnable = val;
	if (m_bInstancingEnable)
	{	
		CreateInstancingResource();		
		DeleteMatrixPallete();
	}
	else
	{
		ReleaseInstancingResource();
		CreateMatrixPallete();
	}
}


}