#include "StdAfx.h"
#include "MeshNode.h"
#include "../ASEParser/ASEParser.h"
#include "../ASEParser/ASELexer.h"

#include "../Log/Trace.h"
#include "../Geometrics/Sphere.h"
#include "../Geometrics/CollisionDetector.h"
#include "../Resource/RscTexture.h"
#include "../Geometrics/Sphere.h"









cMeshNode::cMeshNode(void)
{	
	SetNodeType(MESH);

	m_TotalBoneRef=0;
	m_bIsBone= FALSE;

	
	m_WorldPosOld=D3DXVECTOR3(0.0f,0.0f,0.0f);		// 이전 위치
	m_vecVelocity=D3DXVECTOR3(0.0f,0.0f,0.0f);


	m_pRscIndexBuffer=NULL;
	m_pRscVetextBuffer=NULL;
	m_TotalVertex=0;

	CreateCullingSphere();
	CreateBoundingSphere();
}

cMeshNode::~cMeshNode(void)
{

	m_arrayBoneRef.clear();	

	SAFE_RELEASE(m_pRscVetextBuffer);
	SAFE_RELEASE(m_pRscIndexBuffer);

}



void cMeshNode::Init()
{	
	if (IsSkinnedMesh())
	{	
		//본의 연결은 모든 본을 게층구조에 넣은후에 연결해야한다.
		LinkToBone();
	}
	else
	{
		//본을 참조하지 않고 이름에 bone이 들어가면 본이다.
		if (GetNodeName().find(L"Bone") ==0)
		{
			SetIsBone(TRUE);
		}
	}
	InitChildren();
}
void cMeshNode::Uninit()
{
	UninitChildren();
}

void cMeshNode::Update(DWORD elapseTime)
{
	m_WorldPosOld = GetWorldPos();

	UpdateWorldTM(UpdateTransformAnm(elapseTime),GetParentNode());

	GetBoundingSphere()->SetCenterPos(GetWorldPos());
	*GetCullingSphere() = *GetBoundingSphere();	
	
	m_vecVelocity = GetWorldPos() - m_WorldPosOld;
	
	UpdateChildren(elapseTime);

	UpdateParentCullingSphere(*GetCullingSphere());	
}

void cMeshNode::Render()
{	
	// 자신을 그리고 자식의 렌더를 호출한다.
	if ((m_pRscVetextBuffer!=NULL)&&(m_pRscIndexBuffer!=NULL))
	{
		BOOL bRender=TRUE;

		if(m_bIsBone)
			bRender=FALSE;

		if (bRender)
		{
			if (!IsSkinnedMesh())
			{			
				//IndexBuffer,VertexBuffer셋팅
				m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_WorldTM );	
				m_pD3DDevice->SetFVF(FVF_NORMALVERTEX);
				m_pRscVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
				m_pRscIndexBuffer->SetIndices();			

				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//메쉬에 사용될 매트리얼 얻기
					cMaterialEx* pMaterial=&m_Matrial;
					cRscTexture* pRscTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}
					
					//텍스쳐 적용
					pRscTexture=pMaterial->GetRscTexture();
					if (pRscTexture!=NULL)	
						m_pD3DDevice->SetTexture(0,pRscTexture->GetD3DTexture());	
					else
						m_pD3DDevice->SetTexture(0,NULL);
				/*
					//매트리얼적용
					m_pD3DDevice->SetMaterial((D3DMATERIAL9*)pMaterial);
				*/		

					PrimitiveCount=(*it).second;
					m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
						0,  
						0, 
						m_TotalVertex,
						StartIndex,
						PrimitiveCount );					
					StartIndex+=PrimitiveCount*3; //cnt
				}				

			}	
			else
			{
				m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,TRUE);
				m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_3WEIGHTS);				
					
				int iBoneRef;
				for (iBoneRef=0;iBoneRef<(int)m_arrayBoneRef.size();iBoneRef++)
				{
					BONEREFINFO& refItem=m_arrayBoneRef[iBoneRef];
					//D3DXMATRIX BoneNodeTM,NodeTM_INV,BoneOffsetTM,BoneOffsetTM_INV;
					D3DXMATRIX BlendMat,BoneWorldTM;

					/*
					D3DXMatrixInverse(&NodeTM_INV,NULL,&GetNodeTM());			// NodeTM_INV	
					BoneNodeTM = refItem.pBoneRef->GetNodeTM();					// BoneNodeTM		
					BoneOffsetTM = BoneNodeTM * NodeTM_INV;						// BoneOffsetTM
					D3DXMatrixInverse(&BoneOffsetTM_INV,NULL,&BoneOffsetTM);	// BoneOffsetTM_INV
					*/

					BoneWorldTM = refItem.pBoneRef->GetWorldTM();				// BoneWorldTM		

					BlendMat = refItem.BoneOffSetTM_INV * BoneWorldTM;
					m_pD3DDevice->SetTransform(D3DTS_WORLDMATRIX(iBoneRef),&BlendMat);
				}		

				//IndexBuffer,VertexBuffer셋팅			
				m_pD3DDevice->SetFVF(FVF_BLENDVERTEX);				
				m_pRscVetextBuffer->SetStreamSource(sizeof(BLENDVERTEX));
				m_pRscIndexBuffer->SetIndices();			

				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//메쉬에 사용될 매트리얼 얻기
					cMaterialEx* pMaterial=&m_Matrial;
					cRscTexture* pRscTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}

					//텍스쳐 적용
					pRscTexture=pMaterial->GetRscTexture();
					if (pRscTexture!=NULL)	
						m_pD3DDevice->SetTexture(0,pRscTexture->GetD3DTexture());
					else
						m_pD3DDevice->SetTexture(0,NULL);
					
					/*
					//매트리얼적용
					m_pD3DDevice->SetMaterial((D3DMATERIAL9*)pMaterial);
					*/

					PrimitiveCount=(*it).second;
					m_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
						0,
						0,
						m_TotalVertex,
						StartIndex,
						PrimitiveCount );					
					StartIndex+=PrimitiveCount*3; //cnt
				}

				m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);
				m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);						
			}			
		}
	}
//	RenderChildren();
}




void cMeshNode::InsertBoneRefName(LPCWSTR strNodeName)
{
	BONEREFINFO NewItem;
	NewItem.strNodeName=strNodeName;
	NewItem.pBoneRef=NULL;
	m_arrayBoneRef.push_back(NewItem);
}

void cMeshNode::LinkToBone()
{
	ASSERT(GetRootNode()!=NULL);
	D3DXMATRIX nodeTM_INV,boneOffsetTM;
	D3DXMatrixInverse(&nodeTM_INV,NULL,&GetNodeTM());			// NodeTM_INV	

	vector<BONEREFINFO>::iterator iter;
	for ( iter=m_arrayBoneRef.begin() ; iter!=m_arrayBoneRef.end() ; ++iter)
	{
		BONEREFINFO* p=&(*iter);		
		//이름,타입 같으면 캐스팅 MESH확인
		p->pBoneRef = dynamic_cast<cMeshNode*>(GetRootNode()->FindNode(p->strNodeName,MESH));

		boneOffsetTM = p->pBoneRef->GetNodeTM() * nodeTM_INV;
		D3DXMatrixInverse(&p->BoneOffSetTM_INV,NULL,&boneOffsetTM);	

	}	
}


BOOL cMeshNode::IsSkinnedMesh()
{
	if (m_TotalBoneRef==0)
	{
		return FALSE;
	}
	return TRUE;
}

