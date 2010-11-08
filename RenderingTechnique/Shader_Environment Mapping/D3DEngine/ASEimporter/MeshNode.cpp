#include "StdAfx.h"
#include "MeshNode.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "Scene.h"
#include "../Log/Trace.h"
#include "../Geometrics/Sphere.h"
#include "../Geometrics/CollisionDetector.h"
#include "../Renderer/Camera.h"

#include "../Resource/RscTexture.h"
#include "../TestGameApp.h"


#include "../Shader.h"



// 노말리스트를 버텍스 리스트에 합친다. 
// arrVertex [in,out] , arrVNormal [in] ,  arrFaceIndex[in,out]
template <typename T>
void MergeNormalListIntoVertexList(vector<T>& arrVertex,
								   vector<pair<FACEINDEX,SUBMATINDEX>>& arrFaceIndex,
								   const vector<VNORMAL>& arrVNormal)
{
	vector<list<WORD>>						arrVRefFaceList;	// arrVertex를 참조하는 인덱스기준으로 FACE리스트를 만든다.
	vector<list<pair<D3DXVECTOR3,WORD>>>    arrVRefNormalInsert;		// arrVertex를 참조하는 기준으로 뒤쪽 WORD는 이미존재하는 노말을갖는 버텍스의 인덱스

	if (arrFaceIndex.empty())
		return;
	
	arrVRefFaceList.resize(arrVertex.size());
	arrVRefNormalInsert.resize(arrVertex.size());

	//1. arrVertex를 참조하는 인덱스기준으로 FACEINDEX리스트를 만든다.
	// 즉. 인덱스안의 리스트에 담긴 FACEINDEX들은 모두 arrVertex의 특정 인덱스좌표를 사용한다.
	int nFace=(int)arrFaceIndex.size();
	for (int iFace=0;iFace<nFace;iFace++ )
	{
		arrVRefFaceList[arrFaceIndex[iFace].first.index[0]].push_back(iFace);	
		arrVRefFaceList[arrFaceIndex[iFace].first.index[1]].push_back(iFace);
		arrVRefFaceList[arrFaceIndex[iFace].first.index[2]].push_back(iFace);
	}

	//2. arrFaceNormal 을 arrVertex에 넣으며 arrFace내의 버텍스인덱스를 수정한다.
	WORD nVNormal=(WORD)arrVNormal.size();
	for (WORD iVNormal=0;iVNormal<nVNormal;iVNormal++)
	{
		list<pair<D3DXVECTOR3,WORD>>* pListNormalnsert;
		

		WORD iRefVertex=arrVNormal[iVNormal].iRefVertex;
		pListNormalnsert=&arrVRefNormalInsert[iRefVertex];

		if(pListNormalnsert->empty())
		{	// arrVertex 인덱스의 노말들이 없으면 그냥 넣는다.			
			arrVertex[iRefVertex].normal = arrVNormal[iVNormal].normal;			
			arrVRefNormalInsert[iRefVertex].push_back(make_pair(arrVNormal[iVNormal].normal,iRefVertex));
		}
		else
		{					
			const D3DXVECTOR3* pNewNormal=&arrVNormal[iVNormal].normal;

			list<pair<D3DXVECTOR3,WORD>>::iterator it;
			it=pListNormalnsert->begin();
			for ( ; it != pListNormalnsert->end() ; ++it)
			{				
				D3DXVECTOR3* pExistNormal=&(*it).first;				

				if (D3DXVec3Dot(pExistNormal,pNewNormal) > 0.9999f )
				{	//노말이 리스트에 존재하면 인덱스를 수정한다.
					WORD iExist=(*it).second;
					WORD iRefFace=arrVNormal[iVNormal].iRefFace;

					if (arrFaceIndex[iRefFace].first.index[0] == iRefVertex )
						arrFaceIndex[iRefFace].first.index[0] = iExist;

					if (arrFaceIndex[iRefFace].first.index[1] == iRefVertex )
						arrFaceIndex[iRefFace].first.index[1] = iExist;

					if (arrFaceIndex[iRefFace].first.index[2]== iRefVertex )
						arrFaceIndex[iRefFace].first.index[2] = iExist;					

					break;
				}
			}

			// 노말 같은것이 없으면 새로운 버텍스 추가한다.
			if (it == pListNormalnsert->end())
			{
				arrVertex.push_back(arrVertex[iRefVertex]);
				WORD iInsert=(WORD)arrVertex.size()-1;
				arrVertex[iInsert].normal = *pNewNormal;

				// 인덱스에 해당되는 노말리스트에 노말과,실제 넣은 인덱스 추가
				arrVRefNormalInsert[iRefVertex].push_back(make_pair(*pNewNormal,iInsert));
				WORD iRefFace=arrVNormal[iVNormal].iRefFace;

				if (arrFaceIndex[iRefFace].first.index[0] == iRefVertex )
					arrFaceIndex[iRefFace].first.index[0] = iInsert;

				if (arrFaceIndex[iRefFace].first.index[1] == iRefVertex )
					arrFaceIndex[iRefFace].first.index[1] = iInsert;

				if (arrFaceIndex[iRefFace].first.index[2]== iRefVertex )
					arrFaceIndex[iRefFace].first.index[2] = iInsert;					
			}
		}
	}
}


void	OptimizeTexCoordAndFace(vector<TEXCOORD>& arrTexCoordOut,
								const vector<TEXCOORD>& arrTexCoordIn,
								vector<FACEINDEX>& arrTFaceIndexInOut)
{
	map<float,map<float,WORD>>	mapUVINDEX;
	vector<list<int>>			arrTVIndexedFaceIndexList;	// 해당버텍스를 사용하는 페이스인덱스의 리스트를 버텍스인덱스로 인덱스화

	//랜덤 액세스를 위한 초기할당
	if (arrTexCoordIn.empty())
		return;
	
	arrTVIndexedFaceIndexList.resize(arrTexCoordIn.size());
	// arrTexCoordIn를 참조하는 인덱스기준으로 FACE리스트를 만든다.
	// 즉. 인덱스안의 리스트에 담긴 FACE들은 모두 arrTexCoordIn의 특정 인덱스좌표를 사용한다.
	int nTFace=(int)arrTFaceIndexInOut.size();
	for (int iTFace=0;iTFace<nTFace;iTFace++ )
	{
		arrTVIndexedFaceIndexList[arrTFaceIndexInOut[iTFace].index[0]].push_back(iTFace);	
		arrTVIndexedFaceIndexList[arrTFaceIndexInOut[iTFace].index[1]].push_back(iTFace);
		arrTVIndexedFaceIndexList[arrTFaceIndexInOut[iTFace].index[2]].push_back(iTFace);
	}

	// arrTexCoordIn 의 전체를 돌며 동시에 UVINDEX 맵을 만들며 
	// 최적의 텍스쳐좌표(arrTexCoordOpt) 를 구성하며 페이스리스트(arrTFaceIndex)의 텍스쳐좌표 인덱스를 수정한다.
	WORD nNumTexCoord=(WORD)arrTexCoordIn.size();
	for (WORD iTexCoordIn=0;iTexCoordIn<nNumTexCoord;iTexCoordIn++)
	{
		pair<map<float,map<float,WORD>>::iterator	,bool> retItInsertU;
		pair<map<float,WORD>::iterator				,bool> retItInsertV;

		map<float,WORD> mapVINDEX;
		map<float,WORD>* pmapVINDEX;

		// U좌표와 빈 mapVINDEX 삽입
		retItInsertU= mapUVINDEX.insert(make_pair(arrTexCoordIn[iTexCoordIn].first,mapVINDEX));
		pmapVINDEX=&(retItInsertU.first->second);		
		retItInsertV=pmapVINDEX->insert(make_pair(arrTexCoordIn[iTexCoordIn].second,0));

		WORD iTexCoordOut;
		if (retItInsertV.second)	// V좌표 삽입 성공 (기존값이 없음)
		{			
			arrTexCoordOut.push_back(arrTexCoordIn[iTexCoordIn]);
			retItInsertV.first->second=(WORD)arrTexCoordOut.size()-1;		
			iTexCoordOut=retItInsertV.first->second;
		}
		else						// V좌표 삽입 실패 (기존값이 있음)
		{
			iTexCoordOut=retItInsertV.first->second;		
		}

		// iTexCoordIn,iTexCoordOut 를 이용한  FACE내의 인덱스 수정
		list<int>::iterator it2=arrTVIndexedFaceIndexList[iTexCoordIn].begin();		
		for ( ;it2!=arrTVIndexedFaceIndexList[iTexCoordIn].end();++it2)
		{
			int iFace=*it2;
			if(arrTFaceIndexInOut[iFace].index[0] == iTexCoordIn ) 
				arrTFaceIndexInOut[iFace].index[0]=iTexCoordOut;

			if(arrTFaceIndexInOut[iFace].index[1] == iTexCoordIn ) 
				arrTFaceIndexInOut[iFace].index[1]=iTexCoordOut;

			if(arrTFaceIndexInOut[iFace].index[2] == iTexCoordIn ) 
				arrTFaceIndexInOut[iFace].index[2]=iTexCoordOut;
		}
	}
}

// arrVertexInOut    에존재하는 버텍스는 텍스쳐좌표를 제외한 다른 정보는 유효한값 존재한다는 가정을한다. (버텍스,노말,가중치정보등,,)
// arrFaceIndexInOut 페이스에 쓰이는 위치정보 인덱스,	
// arrTexCoordIn 의 텍스쳐 좌표들은 
// arrTFaceIndexIn   페이스에 쓰이는 텍스쳐 좌표인덱스 
template <typename T>
void MergeTexCoordListIntoVertexList(vector<T>& arrVertexInOut,
									 vector<pair<FACEINDEX,SUBMATINDEX>>& arrVFaceIndexInOut,
									 const vector<TEXCOORD>& arrTexCoordIn,
									 const vector<FACEINDEX>& arrTFaceIndexIn)
{	
	if (arrVertexInOut.empty())
		return;
	if (arrVertexInOut.empty())
		return;
	if (arrTexCoordIn.empty())
		return;
	if (arrTFaceIndexIn.empty())
		return;

	vector<list<pair<TEXCOORD,WORD>>> arrVIndexedTCList;
	//랜덤액세스를 위한 할당
	arrVIndexedTCList.resize(arrVertexInOut.size());

	int nFace=(int)arrVFaceIndexInOut.size();
	for (int iFace=0;iFace<nFace;iFace++)
	{
		FACEINDEX*		 pVFace=&arrVFaceIndexInOut[iFace].first;
		const FACEINDEX* pTFace=&arrTFaceIndexIn[iFace];

		for (int i=0;i<3;i++)
		{
			int iRefVertex=pVFace->index[i];					// 버텍스 인덱스
			int iRefTexCoord=arrTFaceIndexIn[iFace].index[i];	//텍스좌표 인덱스
			const TEXCOORD* pTexcoord=&arrTexCoordIn[iRefTexCoord]; //텍스좌표 얻기


			if(arrVIndexedTCList[iRefVertex].empty())			// 버텍스에 텍스좌표 리스트들이 없으면
			{
				arrVertexInOut[iRefVertex].tex = *pTexcoord;	// 버텍스리스트에 버테스의 텍스좌표 추가
				arrVIndexedTCList[iRefVertex].push_back(make_pair(*pTexcoord,iRefVertex)); //리스트에 텍스좌표추가
			}
			else // 버텍스인덱스별 텍스좌표 리스트에 하나이상 들어있다.
			{				
				list<pair<TEXCOORD,WORD>>* pListTexcoordInsert=&arrVIndexedTCList[iRefVertex];
				list<pair<TEXCOORD,WORD>>::iterator it=pListTexcoordInsert->begin();
				for ( ; it!=pListTexcoordInsert->end();++it)
				{					
					//텍스좌표가 같은것이면 이미 넣은것인덱스로 변경
					if( (*it).first == *pTexcoord )
					{						
						pVFace->index[i]=(*it).second;							
						break;
					}
				}
				//같은게 없으면
				if (it == pListTexcoordInsert->end())
				{
					//버텍스 하나더 만들고 넣은 위치저장 후 인덱스화된 리스트에텍스추가 
					T vertex;
					memcpy(&vertex,&arrVertexInOut[iRefVertex],sizeof(vertex));
					vertex.tex=*pTexcoord;
					arrVertexInOut.push_back(vertex);					

					//새로운 인덱스
					iRefVertex=(int)arrVertexInOut.size()-1;

					//페이스 인덱스 수정
					pVFace->index[i]=iRefVertex;					

					//리스트 생성,텍스추가
					list<pair<TEXCOORD,WORD>> temp;
					arrVIndexedTCList.push_back(temp);
					arrVIndexedTCList[iRefVertex].push_back(make_pair(*pTexcoord,iRefVertex));
				}
			}
		}	
	}
}

BOOL LessFaceIndex(pair<FACEINDEX,SUBMATINDEX> a,pair<FACEINDEX,SUBMATINDEX> b)
{
	if( a.second < b.second)
		return TRUE;

	return FALSE;
}

BOOL GreatBoneWeight(BONEWEIGHT* a,BONEWEIGHT* b)
{
	if ( a->bone_weight > b->bone_weight)
		return TRUE;

	return FALSE;
}

cMeshNode::cMeshNode(void)
{
	m_pArVertex=NULL;
	m_pArIndex=NULL;
	m_dwByteVertex=0;
	m_dwByteIndex=0;

	m_pVB=NULL; 
	m_pIB=NULL;
	
	SetNodeType(MESH);
	m_NUMFACES=0;
	m_NUMVERTEX=0;
	m_NUMBONEREF=0;
	m_bIsBone= FALSE;

	m_Position=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_PositionOld=D3DXVECTOR3(0.0f,0.0f,0.0f);		// 이전 위치
	m_vecVelocity=D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_pBoundingSphere=NULL;

}

cMeshNode::~cMeshNode(void)
{


	if (m_pArIndex!=NULL)
	{
		delete []m_pArIndex;
	}

	vector<BONEREFINFO>::iterator it_arrayBoneRef=m_arrayBoneRef.begin();	
	m_arrayBoneRef.clear();
	
	vector<NORMALVERTEX>::iterator it_arrayNormalVertex=m_arrayNormalVertex.begin();
	m_arrayNormalVertex.clear();
	
	vector<BLENDVERTEX>::iterator it_arrayBLENDVERTEX=m_arrayBlendVertex.begin();
	m_arrayBlendVertex.clear();
	
	SAFE_DELETE(m_pBoundingSphere);
	
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
	

}



void cMeshNode::Init()
{
	HRESULT hResult;
	
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
	m_dwByteVertex=0;
	m_dwByteIndex=0;

	

	if (m_NUMVERTEX>0)
	{
		if (!IsSkinnedMesh())
		{
			m_dwByteVertex=sizeof(NORMALVERTEX)*m_NUMVERTEX;
			// ! VertexBuffer
			// 1) FVF를 설정해서 필요한크기만큼 VertexBuffer를 만든다.	^
			hResult=m_pD3DDevice9->CreateVertexBuffer( 
				m_dwByteVertex,
				0,
				FVF_NORMALVERTEX,
				D3DPOOL_DEFAULT,
				&m_pVB,
				NULL);

			ASSERT(hResult==S_OK);

			// 2) 생성된 Vertex Buffer를 Lock()해서 내용을 써넣는다.
			NORMALVERTEX* pVertices;
			m_pVB->Lock( 0, m_dwByteVertex, (void**)&pVertices, 0 ) ;
			
			for (int i=0;i< m_NUMVERTEX;i++)
			{			
				memcpy(&pVertices[i],&m_arrayNormalVertex[i],sizeof(NORMALVERTEX));
			}
			m_pVB->Unlock();
		}
		else
		{
			m_dwByteVertex=sizeof(BLENDVERTEX)*m_NUMVERTEX;
			// ! VertexBuffer
			// 1) FVF를 설정해서 필요한크기만큼 VertexBuffer를 만든다.	^
			hResult=m_pD3DDevice9->CreateVertexBuffer( 
				m_dwByteVertex,
				0,
				FVF_BLENDVERTEX,
				D3DPOOL_DEFAULT,
				&m_pVB,
				NULL);

			ASSERT(hResult==S_OK);

			// 2) 생성된 Vertex Buffer를 Lock()해서 내용을 써넣는다.
			BLENDVERTEX* pVertices;
			m_pVB->Lock( 0, m_dwByteVertex, (void**)&pVertices, 0 ) ;
			for (int i=0;i< m_NUMVERTEX;i++)
			{
				memcpy(&pVertices[i],&m_arrayBlendVertex[i],sizeof(BLENDVERTEX));
			}
			m_pVB->Unlock();
		}	
	
	}


	if (m_NUMFACES>0)
	{
		m_dwByteIndex=sizeof(WORD)*m_NUMFACES*3;
		// ! IndexBuffer
		// 1) 필요한 Index갯수 만큼의 크기로 IndexBuffer를 만든다.
		hResult=m_pD3DDevice9->CreateIndexBuffer( 
			m_dwByteIndex,
			0,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&m_pIB,
			NULL);

		ASSERT(hResult==S_OK);

		// 2) 생성된 Vertex Buffer를 Lock()해서 내용을 써넣는다.
		FACEINDEX* pIndices;
		m_pIB->Lock( 0, m_dwByteIndex, (void**)&pIndices,0);		
		for (int i=0;i< m_NUMFACES;i++)
		{
			memcpy(&pIndices[i],&m_arrFaceIndex[i].first,sizeof(FACEINDEX));			
		}
		m_pIB->Unlock();	
	}
	
	if (IsSkinnedMesh())
	{	
		//
		//본의 연결은 모든 본을 게층구조에 넣은후에 연결한다.
		LinkToBone();
	}
	else
	{
		//본을 참조하지 않고 이름에 bone이 들어가면 본이다.
		if (GetNodeName().find("Bone") ==0)
		{
			SetIsBone(TRUE);
		}
	}
	
	//로컬 매트릭스 초기화	
	if (IsParentObject())
	{
		m_LocalTM = m_WorldTM;
	}
	else
	{	// m_LocalTM 을 부모기준의 상대 변환으로 설정한다. 
		m_LocalTM =	m_WorldTM * GetParentPtr()->GetWorldInverseTM();	
	}

	CalculateANMKEY(INTERPOLATE_TIME);
	CreateBoundingSphere();

	

	InitChildren();
}
void cMeshNode::Uninit()
{

}

void cMeshNode::Update()
{
	m_PositionOld=m_Position;
	if (GetIsActiveAnimation())
	{
		SetAnimationTM();
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
	UpdateChildren();
}

void cMeshNode::Render()
{	
	int size=sizeof(NORMALVERTEX);
	// 자신을 그리고 자식의 렌더를 호출한다.
	if ((m_pVB!=NULL)&&(m_pIB!=NULL))
	{

		BOOL bRender=TRUE;
		bRender = m_pGraphics->GetDefaultCamera()->CullWorldFrustum(*GetBoundingSphere());

		if(m_bIsBone)
			bRender=FALSE;

		if (bRender)
		{
			if (!IsSkinnedMesh())
			{	
				m_pD3DDevice9->SetTransform(D3DTS_WORLD, &m_WorldTM );	
				D3DXMATRIX temp;
				D3DXMatrixTranspose(&temp,&m_WorldTM);			
				g_pD3DDevice9->SetVertexShaderConstantF( 4, (float*) &temp,4);	

				m_pD3DDevice9->SetFVF(FVF_NORMALVERTEX);
				m_pD3DDevice9->SetStreamSource( 0, m_pVB, 0,  sizeof(NORMALVERTEX) );				
				m_pD3DDevice9->SetIndices(m_pIB); 

				//				m_pD3DDevice9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,  0, m_NUMVERTEX,  0,m_NUMFACES );	


				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//메쉬에 사용될 매트리얼 얻기
					cMaterialNode* pMaterial=&m_Matrial;
					cRscTexture* pRscTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}
// 
// 					//텍스쳐 적용
// 					pRscTexture=pMaterial->GetRscTexture();
// 					if (pRscTexture!=NULL)	
// 						m_pD3DDevice9->SetTexture(0,pRscTexture->GetD3DTexture());	
// 					else
// 						m_pD3DDevice9->SetTexture(0,NULL);
// 
// 					//매트리얼적용
// 					//m_pD3DDevice9->SetMaterial((D3DMATERIAL9*)pMaterial);

					PrimitiveCount=(*it).second;
					m_pD3DDevice9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,  0, m_NUMVERTEX,  StartIndex,PrimitiveCount );					
					StartIndex+=PrimitiveCount*3; //cnt
				}				

			}	
			else
			{
				m_pD3DDevice9->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,TRUE);
				m_pD3DDevice9->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_3WEIGHTS);


				int iBoneRef;
				for (iBoneRef=0;iBoneRef<(int)m_arrayBoneRef.size();iBoneRef++)
				{
					BONEREFINFO Item=m_arrayBoneRef[iBoneRef];
					D3DXMATRIX BlendMat,BoneNodeTM,BoneWorldTM,NodeTM_INV,BoneOffsetTM,BoneOffsetTM_INV;


					BoneWorldTM=Item.pBoneRef->GetWorldTM();					// BoneWorldTM				
					D3DXMatrixInverse(&NodeTM_INV,NULL,&GetNodeTM());			// NodeTM_INV	
					BoneNodeTM=Item.pBoneRef->GetNodeTM();						// BoneNodeTM		
					BoneOffsetTM = BoneNodeTM * NodeTM_INV;						// BoneOffsetTM
					D3DXMatrixInverse(&BoneOffsetTM_INV,NULL,&BoneOffsetTM);	// BoneOffsetTM_INV

					BlendMat = BoneOffsetTM_INV * BoneWorldTM;
					m_pD3DDevice9->SetTransform(D3DTS_WORLDMATRIX(iBoneRef),&BlendMat);
				}		


				m_pD3DDevice9->SetStreamSource( 0, m_pVB, 0,  sizeof(BLENDVERTEX) );
				m_pD3DDevice9->SetFVF(FVF_BLENDVERTEX);
				m_pD3DDevice9->SetIndices(m_pIB); 

				WORD StartIndex=0;WORD PrimitiveCount=0;
				map<SUBMATINDEX,WORD>::iterator it;
				it=m_mapSubIndexCount.begin();
				for ( ; it!=m_mapSubIndexCount.end(); ++it )
				{
					//메쉬에 사용될 매트리얼 얻기
					cMaterialNode* pMaterial=&m_Matrial;
					cRscTexture* pRscTexture=NULL;
					if (!m_Matrial.IsEmptySubMaterial())
					{
						int index=(*it).first;	
						pMaterial=m_Matrial.GetSubMaterial(index);										
					}

// 					//텍스쳐 적용
// 					pRscTexture=pMaterial->GetRscTexture();
// 					if (pRscTexture!=NULL)	
// 						m_pD3DDevice9->SetTexture(0,pRscTexture->GetD3DTexture());
// 					else
// 						m_pD3DDevice9->SetTexture(0,NULL);

					//매트리얼적용
//					m_pD3DDevice9->SetMaterial((D3DMATERIAL9*)pMaterial);

					PrimitiveCount=(*it).second;
					m_pD3DDevice9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,  0, m_NUMVERTEX,  StartIndex,PrimitiveCount );					
					StartIndex+=PrimitiveCount*3; //cnt
				}

				//				m_pD3DDevice9->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,  0, m_NUMVERTEX,  0, m_NUMFACES);	

				m_pD3DDevice9->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);
				m_pD3DDevice9->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);		

			}			
		}
	}
	RenderChildren();
}





void cMeshNode::TransformVertexOnLocalCoord()
{
	D3DXMATRIX InverseNodeTM;
	D3DXMatrixInverse(&InverseNodeTM,NULL,&GetNodeTM());

	if (!IsSkinnedMesh())
	{
		vector<NORMALVERTEX>::iterator iter;
		int index=0;
		NORMALVERTEX* pItem;
		for ( iter=m_arrayNormalVertex.begin() ; iter!=m_arrayNormalVertex.end() ; ++iter,index++)
		{
			pItem=&(*iter);
			D3DXVec3TransformCoord(&pItem->vertex,&pItem->vertex,&InverseNodeTM);
		}	
	}
	else
	{
		vector<BLENDVERTEX>::iterator iter;
		int index=0;
		BLENDVERTEX* pItem;
		for ( iter=m_arrayBlendVertex.begin() ; iter!=m_arrayBlendVertex.end() ; ++iter,index++)
		{
			pItem=&(*iter);
			D3DXVec3TransformCoord(&pItem->vertex,&pItem->vertex,&InverseNodeTM);
		}	
	}
}

void cMeshNode::InsertBoneRefName(LPCSTR strNodeName)
{
	BONEREFINFO NewItem;
	NewItem.strNodeName=strNodeName;
	NewItem.pBoneRef=NULL;
	m_arrayBoneRef.push_back(NewItem);
}

void cMeshNode::LinkToBone()
{
	ASSERT(GetRootPtr()!=NULL);
	vector<BONEREFINFO>::iterator iter;
	for ( iter=m_arrayBoneRef.begin() ; iter!=m_arrayBoneRef.end() ; ++iter)
	{
		BONEREFINFO* p=&(*iter);		
		//이름,타입 같으면 캐스팅 MESH확인
		p->pBoneRef = dynamic_cast<cMeshNode*>(GetRootPtr()->FindChildObject(p->strNodeName,MESH));
	}	
}



BOOL cMeshNode::LoadObjInfo( cASEParser* pParser )
{
	LONG token;
	CHAR tokenString[256];

	int nNUMTVERTEX;

	vector<TEXCOORD>	  arrTVertex;
	vector<TEXCOORD>	  arrTVertexOut;
	vector<FACEINDEX>	  arrTFaceIndex;	
	vector<VNORMAL>		  arrVertexNormal;
	

	if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)
		return FALSE;


	while (token=pParser->GetToken(tokenString),token!=TOKEND_BLOCK_END)
	{
		switch(token)
		{
		case TOKENR_TIMEVALUE:	
			pParser->GetInt();
			break;
		case TOKENR_MESH_NUMVERTEX:
			m_NUMVERTEX=pParser->GetInt();			
			m_arrayBlendVertex.reserve(m_NUMVERTEX);
			break;
		case TOKENR_MESH_NUMFACES:
			m_NUMFACES=pParser->GetInt();
		
			m_arrFaceIndex.reserve(m_NUMFACES);
			break;
		case TOKENR_MESH_VERTEX_LIST:
			{
				if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)
					return FALSE;
				while (token=pParser->GetToken(tokenString),token!=TOKEND_BLOCK_END)
				{
					switch(token)
					{
					case TOKENR_MESH_VERTEX:
						if (m_NUMBONEREF==0)
						{
							NORMALVERTEX Item;
							memset(&Item,0,sizeof(Item));
								
							int index=pParser->GetInt();
							pParser->GetVector3(&Item.vertex);										
							m_arrayNormalVertex.push_back(Item);	
						}
						else
						{
							BLENDVERTEX Item;
							memset(&Item,0,sizeof(Item));
											
							int index=pParser->GetInt();
							pParser->GetVector3(&Item.vertex);									
							m_arrayBlendVertex.push_back(Item);	
						}						
						break;
					}
				}

			}			
			break;
		case TOKENR_MESH_FACE_LIST:
			{
				//pParser->SkipBlock();
				if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while (token=pParser->GetToken(tokenString),token!=TOKEND_BLOCK_END)
				{
					switch(token)
					{
					case TOKENR_MESH_FACE:						
						int iFace,iMat;
						FACEINDEX tFaceIndex;
						iFace=pParser->GetInt();			// FaceIndex

						token=pParser->GetToken(tokenString);		// A:
						tFaceIndex.index[0]=pParser->GetInt();			// 0

						token=pParser->GetToken(tokenString);		// B:
						tFaceIndex.index[2]=pParser->GetInt();			// 2

						token=pParser->GetToken(tokenString);		// C:
						tFaceIndex.index[1]=pParser->GetInt();			// 3						
					
						pParser->FindToken(TOKENR_MESH_MTLID);
						iMat=pParser->GetInt();

						pair<FACEINDEX,WORD> temp;		
						temp.first = tFaceIndex;						
						temp.second=iMat;
						m_mapSubIndexCount.insert(make_pair(iMat,0));
						m_arrFaceIndex.push_back(temp);
						break;
					}					
				}
			}	
			break;
		case TOKENR_MESH_FACEMAPLIST:
			{
				pParser->SkipBlock();
			}	
			break;
		case TOKENR_MESH_NUMTVERTEX:
			nNUMTVERTEX=pParser->GetInt();
			arrTVertex.reserve(nNUMTVERTEX);

			break;
		case TOKENR_MESH_TVERTLIST:
			{				
				if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while (token=pParser->GetToken(tokenString),token!=TOKEND_BLOCK_END)
				{
					switch(token)
					{
					case TOKENR_MESH_TVERT:	
						int index=pParser->GetInt();

						D3DXVECTOR3 tvertex;
						pParser->GetVector3(&tvertex);
						tvertex.z = 1.0f-tvertex.z;
						arrTVertex.push_back(TEXCOORD(tvertex.x,tvertex.z));
						break;
					}					
				}
			}	
			break;
		case TOKENR_MESH_NUMTVFACES:
			pParser->SkipBlock();
			break;
		case TOKENR_MESH_TFACELIST:
			{				
				if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while (token=pParser->GetToken(tokenString),token!=TOKEND_BLOCK_END)
				{
					switch(token)
					{
					case TOKENR_MESH_TFACE:						
						int iTFace;
						FACEINDEX tFaceIndex;
						iTFace=pParser->GetInt();			// FaceIndex						
						tFaceIndex.index[0]=pParser->GetInt();			// 0						
						tFaceIndex.index[2]=pParser->GetInt();			// 1						
						tFaceIndex.index[1]=pParser->GetInt();			// 2						
						arrTFaceIndex.push_back(tFaceIndex);
						break;
					}					
				}
			}	
			break;

		case TOKENR_MESH_NUMCVERTEX:
			pParser->GetInt();
			break;
		case TOKENR_MESH_NORMALS:
			{
				// {
				if(pParser->GetToken(tokenString) != TOKEND_BLOCK_START)
					return FALSE;

				for (int iNUMFACES=0;iNUMFACES<m_NUMFACES;iNUMFACES++)
				{
					// *MESH_FACENORMAL
					if(pParser->GetToken(tokenString) != TOKENR_MESH_FACENORMAL)
						return FALSE;

					int iRefFace=pParser->GetInt();
					pParser->GetVector3(NULL);

					for (int triangle=0;triangle<3;triangle++)
					{
						// *MESH_VERTEXNORMAL
						if(pParser->GetToken(tokenString) != TOKENR_MESH_VERTEXNORMAL)
							return FALSE;

						
						VNORMAL temp;
						temp.iRefFace= iRefFace;
						temp.iRefVertex= pParser->GetInt();		// vertex index						
						pParser->GetVector3(&temp.normal);		// vertex normal			
						arrVertexNormal.push_back(temp);					
					}
				}
				// }
				if(pParser->GetToken(tokenString) != TOKEND_BLOCK_END)
					return FALSE;				
			}
			break;	
		case TOKENR_MESH_NUMBONE:
			m_NUMBONEREF=pParser->GetInt();
			break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			pParser->GetInt();
			break;	
		case TOKENR_SKIN_INITTM:
			pParser->SkipBlock();
			break;
		case TOKENR_BONE_LIST:
			{
				if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				for (int i=0;i<m_NUMBONEREF;i++)
				{
					if(pParser->GetToken(tokenString)!=TOKENR_BONE)
						return FALSE;
					pParser->GetInt();				
					if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)
						return FALSE;

					if(pParser->GetToken(tokenString)!=TOKENR_BONE_NAME)
						return FALSE;				

					InsertBoneRefName(pParser->GetString().c_str());

					if (!pParser->FindToken(TOKEND_BLOCK_END))
						return FALSE;
				}

				if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_END)
					return FALSE;
			}
			break;
		case TOKENR_MESH_WVERTEXS:
			{			
				if(pParser->GetToken(tokenString) != TOKEND_BLOCK_START)	
					return FALSE;			
				
				for (int iVertex=0;iVertex<(int)m_arrayBlendVertex.size();iVertex++)
				{						
					vector<BONEWEIGHT*>	  arrBoneWeight;
					
					if(pParser->GetToken(tokenString) != TOKENR_MESH_WEIGHT)	return FALSE;
					int index=pParser->GetInt();

					if (pParser->GetToken(tokenString)!=TOKEND_BLOCK_START)	return FALSE;
					while (token=pParser->GetToken(tokenString),token!=TOKEND_BLOCK_END)
					{
						switch(token)
						{
						case TOKENR_BONE_BLENGING_WEIGHT:
							int BoneIndex=pParser->GetInt();
							float BoneWeight=pParser->GetFloat();
													
							BONEWEIGHT* temp=new BONEWEIGHT;
							temp->bone_index=BoneIndex;
							temp->bone_weight=BoneWeight;
							arrBoneWeight.push_back(temp);
							break;					
						}					
					}
					
					sort(arrBoneWeight.begin(),arrBoneWeight.end(),GreatBoneWeight);
					BYTE bindex[4]= { 0,0,0,0};
					float bweight[4]= { 0.0f,0.0f,0.0f,0.0f };	
					for (int iTop=0; (iTop<4)&&(iTop<(int)arrBoneWeight.size());iTop++)
					{
						bindex[iTop]=arrBoneWeight[iTop]->bone_index;
						bweight[iTop]=arrBoneWeight[iTop]->bone_weight;
					}					
					for_each(arrBoneWeight.begin(),arrBoneWeight.end(),FuncDeleteType<BONEWEIGHT*>);

					m_arrayBlendVertex[iVertex].index3 = bindex[3]; 
					m_arrayBlendVertex[iVertex].index2 = bindex[2];
					m_arrayBlendVertex[iVertex].index1 = bindex[1];
					m_arrayBlendVertex[iVertex].index0 = bindex[0];

					m_arrayBlendVertex[iVertex].weight0 = bweight[0]; 
					m_arrayBlendVertex[iVertex].weight1 = bweight[1];
					m_arrayBlendVertex[iVertex].weight2 = bweight[2];
				}
				if(pParser->GetToken(tokenString) != TOKEND_BLOCK_END)		return FALSE;
			}
			break;
		default:
			{	
				string test="test";
				TCHAR strToken[256]={0,};
				TCHAR strNodeName[256]={0,};	
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,tokenString,strlen(tokenString),strToken,256);				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,GetNodeName().c_str(),GetNodeName().length(),strNodeName,256);			
				TRACE4(_T("정의되지 않은 Command입니다. ( %s ,NodeName= %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__), strNodeName,token,strToken );						
			}		
			return	FALSE;
		}		
	}	

	if (m_NUMBONEREF==0)
	{
		MergeNormalListIntoVertexList(m_arrayNormalVertex,m_arrFaceIndex,arrVertexNormal);
	}
	else
	{
		MergeNormalListIntoVertexList(m_arrayBlendVertex,m_arrFaceIndex,arrVertexNormal);
	}
	
	if (!arrTVertex.empty())
	{
		OptimizeTexCoordAndFace(arrTVertexOut,arrTVertex,arrTFaceIndex);
		if (m_NUMBONEREF==0)
		{
			MergeTexCoordListIntoVertexList(m_arrayNormalVertex,m_arrFaceIndex,arrTVertexOut,arrTFaceIndex);
		}
		else
		{
			MergeTexCoordListIntoVertexList(m_arrayBlendVertex,m_arrFaceIndex,arrTVertexOut,arrTFaceIndex);
		}
	}

	sort(m_arrFaceIndex.begin(),m_arrFaceIndex.end(),LessFaceIndex);

	map<SUBMATINDEX,WORD>::iterator it_mapSubIndexCount;
	it_mapSubIndexCount=m_mapSubIndexCount.begin();
	for ( ; it_mapSubIndexCount!=m_mapSubIndexCount.end() ; ++it_mapSubIndexCount )
	{			
		vector<pair<FACEINDEX,SUBMATINDEX>>::iterator it_arrFaceIndex;
		it_arrFaceIndex = m_arrFaceIndex.begin();
		for ( ; it_arrFaceIndex!=m_arrFaceIndex.end() ; ++it_arrFaceIndex )
		{
			if( (*it_arrFaceIndex).second == (*it_mapSubIndexCount).first )
			{
				(*it_mapSubIndexCount).second++;
			}
		}		
	}


	m_NUMFACES = m_arrFaceIndex.size();
	if (m_NUMBONEREF==0)
	{
		m_NUMVERTEX = m_arrayNormalVertex.size();
	}
	else
	{
		m_NUMVERTEX = m_arrayBlendVertex.size();
	}

	// 모든 버텍스를 로컬좌표계로 이동시킨다.
	TransformVertexOnLocalCoord();		
	return TRUE;
}

// 버텍스 배열에 버텍스들이 존재해야한다.
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
	


	//버텍스가 어쨋든 x,y,z축의 최대 최소값을 구한다.
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
	// 최대 최소의 중간값으로 Center pos을 구한다.
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


	// 모델의 BoundBoxMin,Max를 업데이트한다.	
	//애니메이션이되면 전체 구는 소용이 없다.
	//GetScenePtr()->SetBoundBoxMax(AxisMax);
	//GetScenePtr()->SetBoundBoxMin(AxisMin);
	
}

cSphere* cMeshNode::GetBoundingSphere()
{
	if (m_pBoundingSphere==NULL)
	{
		return NULL;
	}
	return m_pBoundingSphere;
}

BOOL cMeshNode::IsSkinnedMesh()
{
	if (m_NUMBONEREF==0)
	{
		return FALSE;
	}
	return TRUE;
}



