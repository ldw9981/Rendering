#include "StdAfx.h"
#include "MeshNode.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "SceneRoot.h"
#include "../Log/Trace.h"
#include "../Geometrics/Sphere.h"
#include "../Geometrics/CollisionDetector.h"
#include "../Renderer/Camera.h"

#include "../Resource/RscTexture.h"
#include "../TestGameApp.h"
#include "../Shader.h"








BOOL GreatBoneWeight(BONEWEIGHT* a,BONEWEIGHT* b)
{
	if ( a->bone_weight > b->bone_weight)
		return TRUE;

	return FALSE;
}

cMeshNode::cMeshNode(void)
{	
	SetNodeType(MESH);

	m_TotalBoneRef=0;
	m_bIsBone= FALSE;

	m_Position=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_PositionOld=D3DXVECTOR3(0.0f,0.0f,0.0f);		// ���� ��ġ
	m_vecVelocity=D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_pBoundingSphere=NULL;
	m_pRscIndexBuffer=NULL;
	m_pRscVetextBuffer=NULL;
	m_TotalVertex=0;

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
//	CreateBoundingSphere();
	InitChildren();
}
void cMeshNode::Uninit()
{
	UninitChildren();
}

void cMeshNode::Update(DWORD elapseTime)
{
	m_PositionOld=m_Position;
	
	if (GetIsActiveAnimation())
	{
		UpdateTransformAnm( elapseTime);
		m_LocalTM = GetAnimationTM();
	}
	
	if (IsParentObject())	
	{		
		m_WorldTM = m_LocalTM;	
	}
	else
	{			
		m_WorldTM = m_LocalTM * GetParentPtr()->GetWorldTM();
	}	
	m_Position=D3DXVECTOR3(m_WorldTM._41,m_WorldTM._42,m_WorldTM._43);
	m_vecVelocity =  m_Position - m_PositionOld;
	if (m_pBoundingSphere!=NULL)
	{
		m_pBoundingSphere->SetCenter(m_Position);
	}	
	UpdateChildren(elapseTime);
}

void cMeshNode::Render()
{	
	// �ڽ��� �׸��� �ڽ��� ������ ȣ���Ѵ�.
	if ((m_pRscVetextBuffer!=NULL)&&(m_pRscIndexBuffer!=NULL))
	{

		BOOL bRender=TRUE;
//		bRender = m_pGraphics->GetDefaultTopCamera()->CullWorldFrustum(*GetBoundingSphere());

		if(m_bIsBone)
			bRender=FALSE;

		if (bRender)
		{

			if (!IsSkinnedMesh())
			{								
				m_pD3DDevice9->SetVertexShader(NULL);

				//IndexBuffer,VertexBuffer����
				m_pD3DDevice9->SetTransform(D3DTS_WORLD, &m_WorldTM );	
				m_pD3DDevice9->SetFVF(FVF_NORMALVERTEX);
				m_pD3DDevice9->SetStreamSource( 0, m_pRscVetextBuffer->GetD3DVertexBuffer(), 0,  sizeof(NORMALVERTEX) );				
				m_pD3DDevice9->SetIndices(m_pRscIndexBuffer->GetD3DIndexBuffer()); 

				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//�޽��� ���� ��Ʈ���� ���
					cMaterialNode* pMaterial=&m_Matrial;
					cRscTexture* pRscTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}
					
					//�ؽ��� ����
					pRscTexture=pMaterial->GetRscTexture();
					if (pRscTexture!=NULL)	
						m_pD3DDevice9->SetTexture(0,pRscTexture->GetD3DTexture());	
					else
						m_pD3DDevice9->SetTexture(0,NULL);
				/*
					//��Ʈ��������
					m_pD3DDevice9->SetMaterial((D3DMATERIAL9*)pMaterial);
				*/		

					PrimitiveCount=(*it).second;
					m_pD3DDevice9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
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
				m_pD3DDevice9->SetVertexShader(g_pVertexShader);

//				m_pD3DDevice9->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,TRUE);
//				m_pD3DDevice9->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_3WEIGHTS);				
				D3DXVECTOR4	tempSetInfo[256*3+1];	// �ִ�Bone��256����.	

				UINT iBoneRef;
				for (iBoneRef=0;iBoneRef<m_arrayBoneRef.size();iBoneRef++)
				{
					BONEREFINFO& refItem=m_arrayBoneRef[iBoneRef];			
					//D3DXMATRIX BlendMat,BoneWorldTM;					
					//BoneWorldTM = refItem.pBoneRef->GetWorldTM();				// BoneWorldTM		
					//BlendMat = refItem.BoneOffSetTM_INV * BoneWorldTM;					
					/*
						�������� ���� ��� ���� ���� Matrix�� �����ϰ�
						���ؽ��� ���˿� ������ 4�� ���� ��Ʈ���� ������ ��ġ�� �����Ѵ�.
					*/	
					//m_pD3DDevice9->SetTransform(D3DTS_WORLDMATRIX(iBoneRef),&BlendMat);
					D3DXMatrixMultiplyTranspose((LPD3DXMATRIX)(tempSetInfo+iBoneRef*3),
						&m_arrayBoneRef[iBoneRef].BoneOffSetTM_INV,
						&m_arrayBoneRef[iBoneRef].pBoneRef->GetWorldTM());
				}	
				m_pD3DDevice9->SetVertexShaderConstantF(26, (float*)tempSetInfo, m_arrayBoneRef.size()*3);
			

				//IndexBuffer,VertexBuffer����
				m_pD3DDevice9->SetStreamSource( 0, m_pRscVetextBuffer->GetD3DVertexBuffer(), 0,  sizeof(BLENDVERTEX) );	
				m_pD3DDevice9->SetFVF(FVF_BLENDVERTEX);
				m_pD3DDevice9->SetIndices(m_pRscIndexBuffer->GetD3DIndexBuffer()); 

				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//�޽��� ���� ��Ʈ���� ���
					cMaterialNode* pMaterial=&m_Matrial;
					cRscTexture* pRscTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}

					//�ؽ��� ����
					pRscTexture=pMaterial->GetRscTexture();
					if (pRscTexture!=NULL)	
						m_pD3DDevice9->SetTexture(0,pRscTexture->GetD3DTexture());
					else
						m_pD3DDevice9->SetTexture(0,NULL);
					
					/*
					//��Ʈ��������
					m_pD3DDevice9->SetMaterial((D3DMATERIAL9*)pMaterial);
					*/

					PrimitiveCount=(*it).second;
					m_pD3DDevice9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
						0,
						0,
						m_TotalVertex,
						StartIndex,
						PrimitiveCount );					
					StartIndex+=PrimitiveCount*3; //cnt
				}

//				m_pD3DDevice9->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);
//				m_pD3DDevice9->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);		
				
			}			
		}
	}
	RenderChildren();
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
	ASSERT(GetRoot()!=NULL);
	vector<BONEREFINFO>::iterator iter;
	for ( iter=m_arrayBoneRef.begin() ; iter!=m_arrayBoneRef.end() ; ++iter)
	{
		BONEREFINFO* p=&(*iter);		
		//�̸�,Ÿ�� ������ ĳ���� MESHȮ��
		p->pBoneRef = dynamic_cast<cMeshNode*>(GetRoot()->FindChildObject(p->strNodeName,MESH));
/*
		D3DXMatrixInverse(&NodeTM_INV,NULL,&GetNodeTM());			// NodeTM_INV	
		BoneNodeTM = refItem.pBoneRef->GetNodeTM();					// BoneNodeTM		
		BoneOffsetTM = BoneNodeTM * NodeTM_INV;						// BoneOffsetTM
		D3DXMatrixInverse(&BoneOffsetTM_INV,NULL,&BoneOffsetTM);	// BoneOffsetTM_INV
*/
		D3DXMATRIX nodeTM_INV,boneOffsetTM;
		D3DXMatrixInverse(&nodeTM_INV,NULL,&GetNodeTM());			// NodeTM_INV	
		boneOffsetTM = p->pBoneRef->GetNodeTM() * nodeTM_INV;
		D3DXMatrixInverse(&p->BoneOffSetTM_INV,NULL,&boneOffsetTM);	

	}	
}

/*
// ���ؽ� �迭�� ���ؽ����� �����ؾ��Ѵ�.
void cMeshNode::CreateBoundingSphere()
{
	if ( m_arrayNormalVertex.empty() && m_arrayBlendVertex.empty())
	{
		return;
	}
	
	m_pBoundingSphere = new cSphere;

	D3DXVECTOR3 AxisMin=D3DXVECTOR3(0.0f,0.0f,0.0f),AxisMax=D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 CenterPos;
	float		Radius;
	


	//���ؽ��� ��¶�� x,y,z���� �ִ� �ּҰ��� ���Ѵ�.
	if (!IsSkinnedMesh())
	{
		vector<NORMALVERTEX>::iterator it;
		for (it=m_arrayNormalVertex.begin();it!=m_arrayNormalVertex.end();++it)
		{
			NORMALVERTEX* pNormalVertexItem=&(*it);
			AxisMin.x= min(pNormalVertexItem->vertex.x,AxisMin.x);
			AxisMin.y= min(pNormalVertexItem->vertex.y,AxisMin.y);
			AxisMin.z= min(pNormalVertexItem->vertex.z,AxisMin.z);

			AxisMax.x= max(pNormalVertexItem->vertex.x,AxisMax.x);
			AxisMax.y= max(pNormalVertexItem->vertex.y,AxisMax.y);
			AxisMax.z= max(pNormalVertexItem->vertex.z,AxisMax.z);		
		}
	}
	else
	{
		vector<BLENDVERTEX>::iterator it;
		for (it=m_arrayBlendVertex.begin();it!=m_arrayBlendVertex.end();++it)
		{
			BLENDVERTEX* pBLENDVERTEXItem=&(*it);
			AxisMin.x= min(pBLENDVERTEXItem->vertex.x,AxisMin.x);
			AxisMin.y= min(pBLENDVERTEXItem->vertex.y,AxisMin.y);
			AxisMin.z= min(pBLENDVERTEXItem->vertex.z,AxisMin.z);

			AxisMax.x= max(pBLENDVERTEXItem->vertex.x,AxisMax.x);
			AxisMax.y= max(pBLENDVERTEXItem->vertex.y,AxisMax.y);
			AxisMax.z= max(pBLENDVERTEXItem->vertex.z,AxisMax.z);		
		}
	}
	// �ִ� �ּ��� �߰������� Center pos�� ���Ѵ�.
	CenterPos.x= AxisMin.x + (AxisMax.x - AxisMin.x)/2.0f;
	CenterPos.y= AxisMin.y + (AxisMax.y - AxisMin.y)/2.0f;
	CenterPos.z= AxisMin.z + (AxisMax.z - AxisMin.z)/2.0f;

	float MaxLengthSq=0.0f,LengthSq;
	if (!IsSkinnedMesh())
	{
		vector<NORMALVERTEX>::iterator it;
		for (it=m_arrayNormalVertex.begin();it!=m_arrayNormalVertex.end();++it)
		{
			NORMALVERTEX* pNormalVertexItem=&(*it);
			LengthSq=D3DXVec3LengthSq(&D3DXVECTOR3(CenterPos - pNormalVertexItem->vertex));
			MaxLengthSq=max(MaxLengthSq,LengthSq);
		}
	}
	else
	{
		vector<BLENDVERTEX>::iterator it;
		for (it=m_arrayBlendVertex.begin();it!=m_arrayBlendVertex.end();++it)
		{
			BLENDVERTEX* pBLENDVERTEXItem=&(*it);
			LengthSq=D3DXVec3LengthSq(&D3DXVECTOR3(CenterPos - pBLENDVERTEXItem->vertex));
			MaxLengthSq=max(MaxLengthSq,LengthSq);		
		}
	}
	Radius=sqrt(MaxLengthSq);
	m_pBoundingSphere->Make(CenterPos,Radius);	
}

cSphere* cMeshNode::GetBoundingSphere()
{
	if (m_pBoundingSphere==NULL)
	{
		return NULL;
	}
	return m_pBoundingSphere;
}
*/
BOOL cMeshNode::IsSkinnedMesh()
{
	if (m_TotalBoneRef==0)
	{
		return FALSE;
	}
	return TRUE;
}


