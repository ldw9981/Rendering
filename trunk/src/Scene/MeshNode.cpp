
#include "MeshNode.h"
#include "../ASEParser/ASEParser.h"
#include "../ASEParser/ASELexer.h"

#include "../Foundation/Trace.h"
#include "../Math/Sphere.h"
#include "../Math/CollisionDetector.h"
#include "../D3D9/Texture.h"
#include "../Math/Sphere.h"

#include "../Foundation/Define.h"







cMeshNode::cMeshNode(void)
{	
	SetNodeType(MESH);

	m_TotalBoneRef=0;
	m_bIsBone= FALSE;

	
	m_WorldPosOld=D3DXVECTOR3(0.0f,0.0f,0.0f);		// ���� ��ġ
	m_vecVelocity=D3DXVECTOR3(0.0f,0.0f,0.0f);


	m_pIndexBuffer=NULL;
	m_pVetextBuffer=NULL;
	m_TotalVertex=0;

	CreateCullingSphere();
	CreateBoundingSphere();
}

cMeshNode::~cMeshNode(void)
{

	m_arrayBoneRef.clear();	

	SAFE_RELEASE(m_pVetextBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

}



void cMeshNode::Init()
{	
	if (IsSkinnedMesh())
	{	
		//���� ������ ��� ���� ���������� �����Ŀ� �����ؾ��Ѵ�.
		LinkToBone();
	}
	else
	{
		//���� �������� �ʰ� �̸��� bone�� ���� ���̴�.
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
	// �ڽ��� �׸��� �ڽ��� ������ ȣ���Ѵ�.
	if ((m_pVetextBuffer!=NULL)&&(m_pIndexBuffer!=NULL))
	{
		BOOL bRender=TRUE;

		if(m_bIsBone)
			bRender=FALSE;

		if (bRender)
		{
			if (!IsSkinnedMesh())
			{			
				//IndexBuffer,VertexBuffer����
				m_SharedLPDIRECT3DDEVICE9->SetTransform(D3DTS_WORLD, &m_WorldTM );	
				m_SharedLPDIRECT3DDEVICE9->SetFVF(FVF_NORMALVERTEX);
				m_pVetextBuffer->SetStreamSource(sizeof(NORMALVERTEX));
				m_pIndexBuffer->SetIndices();			

				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//�޽��� ���� ��Ʈ���� ���
					cMaterialEx* pMaterial=&m_Matrial;
					Texture* pTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}
					
					//�ؽ��� ����
					pTexture=pMaterial->GetTexture();
					if (pTexture!=NULL)	
						m_SharedLPDIRECT3DDEVICE9->SetTexture(0,pTexture->GetD3DTexture());	
					else
						m_SharedLPDIRECT3DDEVICE9->SetTexture(0,NULL);
				/*
					//��Ʈ��������
					m_SharedLPDIRECT3DDEVICE9->SetMaterial((D3DMATERIAL9*)pMaterial);
				*/		

					PrimitiveCount=(*it).second;
					m_SharedLPDIRECT3DDEVICE9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
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
				m_SharedLPDIRECT3DDEVICE9->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,TRUE);
				m_SharedLPDIRECT3DDEVICE9->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_3WEIGHTS);				
					
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
					m_SharedLPDIRECT3DDEVICE9->SetTransform(D3DTS_WORLDMATRIX(iBoneRef),&BlendMat);
				}		

				//IndexBuffer,VertexBuffer����			
				m_SharedLPDIRECT3DDEVICE9->SetFVF(FVF_BLENDVERTEX);				
				m_pVetextBuffer->SetStreamSource(sizeof(BLENDVERTEX));
				m_pIndexBuffer->SetIndices();			

				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//�޽��� ���� ��Ʈ���� ���
					cMaterialEx* pMaterial=&m_Matrial;
					Texture* pTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}

					//�ؽ��� ����
					pTexture=pMaterial->GetTexture();
					if (pTexture!=NULL)	
						m_SharedLPDIRECT3DDEVICE9->SetTexture(0,pTexture->GetD3DTexture());
					else
						m_SharedLPDIRECT3DDEVICE9->SetTexture(0,NULL);
					
					/*
					//��Ʈ��������
					m_SharedLPDIRECT3DDEVICE9->SetMaterial((D3DMATERIAL9*)pMaterial);
					*/

					PrimitiveCount=(*it).second;
					m_SharedLPDIRECT3DDEVICE9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
						0,
						0,
						m_TotalVertex,
						StartIndex,
						PrimitiveCount );					
					StartIndex+=PrimitiveCount*3; //cnt
				}

				m_SharedLPDIRECT3DDEVICE9->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);
				m_SharedLPDIRECT3DDEVICE9->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);						
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
		//�̸�,Ÿ�� ������ ĳ���� MESHȮ��
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

