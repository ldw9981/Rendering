#include "stdafx.h"
#include "../Log/Trace.h"

#include "ASEParser.h"
#include "ASELexer.h"
#include "Lexer.h"



#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/HelperNode.h"
#include "../SceneGraph/ShapeNode.h"
#include "../SceneGraph/CameraNode.h"
#include "../SceneGraph/LightNode.h"
#include "../Framework/D3DFramework.h"
#include "../Resource/RscTransformAnm.h"

cASEParser::cASEParser()
{
	m_CNTOBJECT=0;	
}
cASEParser::~cASEParser(void)
{
}

float cASEParser::GetFloat()
{
	float			tNumber;
	m_Token			 = GetToken(m_TokenString);	ASSERT(m_Token == TOKEND_NUMBER);
	tNumber		 = (float)atof(m_TokenString);
	return			tNumber;
}

int cASEParser::GetInt()
{
	int				tNumber;
	m_Token			 = GetToken(m_TokenString);	ASSERT(m_Token == TOKEND_NUMBER);
	tNumber		 = atoi(m_TokenString);
	return			tNumber;	
}

long cASEParser::GetLong()
{
	LONG			tNumber;
	m_Token			 = GetToken(m_TokenString);	ASSERT(m_Token == TOKEND_NUMBER);
	tNumber		 = strtoul(m_TokenString, NULL, 10);
	return			tNumber;
}

BOOL cASEParser::GetVector3(D3DXVECTOR3* pOutput)
{	
	m_Token			 = GetToken(m_TokenString);		ASSERT(m_Token == TOKEND_NUMBER);
	if(pOutput!=NULL)	pOutput->x	 = (float)atof(m_TokenString);
	m_Token			 = GetToken(m_TokenString);		ASSERT(m_Token == TOKEND_NUMBER);
	if(pOutput!=NULL)	pOutput->z	 = (float)atof(m_TokenString);
	m_Token			 = GetToken(m_TokenString);		ASSERT(m_Token == TOKEND_NUMBER);
	if(pOutput!=NULL)	pOutput->y	 = (float)atof(m_TokenString);
	return			TRUE;
}


string& cASEParser::GetString()
{	
	static string temp;
	// string
	m_Token=GetToken(m_TokenString);
	if (m_Token!=TOKEND_STRING)
	{
		ASSERT(0&&_T("m_Token!=TOKEND_STRING"));	
	}	
	temp=m_TokenString;
	return temp;
}

wstring& cASEParser::GetWString()
{
	static wstring temp;
	m_Token=GetToken(m_TokenString);
	if (m_Token!=TOKEND_STRING)
	{
		ASSERT(0&&_T("m_Token!=TOKEND_STRING"));	
	}	

	WCHAR WideBuffer[256]={0,};
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),WideBuffer,256);
	temp=WideBuffer;
	return temp;
}

// Skip to the end of this block.
BOOL cASEParser::SkipBlock()
{
	int level = 0;
	
	do 
	{
		m_Token =  GetToken(m_TokenString);

		if (m_Token==TOKEND_BLOCK_START)
			level++;
		else if (m_Token==TOKEND_BLOCK_END)
			level--;

		if (level>100)
		{
			return FALSE;
		}
	} while (level > 0);

	return TRUE;
}



BOOL cASEParser::FindToken( LONG token)
{
	BOOL bFind=FALSE;
	do 
	{
		m_Token =  GetToken(m_TokenString);
		
		if(m_Token==token)
		{
			bFind=TRUE;
			return bFind;
		}
		
	} while (m_Token!=TOKEND_END );
	assert(bFind==TRUE);
	return bFind;
}

LONG cASEParser::GetNextASEToken()
{
	do 
	{
		m_Token =  GetToken(m_TokenString);

		if(m_Token < TOKENR_MAX )
		{
			return m_Token;
		}

	} while (m_Token!=TOKEND_END );
	return TOKEND_END;
}

D3DXMATRIX& cASEParser::GetNodeTM()
{
	static D3DXMATRIX nodeTM;
	D3DXMatrixIdentity(&nodeTM);	
	FindToken(TOKEND_BLOCK_START);

	int row;
	for (int i=0;i<4;i++)
	{			
		FindToken(TOKENR_TM_ROW0+i);

		D3DXVECTOR3 tVector3;
		GetVector3(&tVector3);


		if (i==1) row=2;
		else if (i==2) row=1;
		else row=i;				
		nodeTM.m[row][0]=tVector3.x;
		nodeTM.m[row][1]=tVector3.y;
		nodeTM.m[row][2]=tVector3.z;
	}		

	FindToken(TOKEND_BLOCK_END);
	return nodeTM;
}

D3DXMATRIX& cASEParser::GetMatrix()
{
	static D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);

	int row;
	for (int i=0;i<4;i++)
	{			
		FindToken(TOKENR_TM_ROW0+i);

		D3DXVECTOR3 tVector3;
		GetVector3(&tVector3);

		if (i==1) row=2;
		else if (i==2) row=1;
		else row=i;				
		temp.m[row][0]=tVector3.x;
		temp.m[row][1]=tVector3.y;
		temp.m[row][2]=tVector3.z;
	}		
	return temp;
}

BOOL cASEParser::GetIdentifier( LPSTR pOutput )
{
	m_Token=GetToken(m_TokenString);
	if (m_Token != TOKEND_IDENTIFIER)
	{
		ASSERT(0&&_T("m_Token!=TOKEND_IDENTIFIER"));
		return FALSE;
	}
	if (pOutput!=NULL)
	{
		memcpy(pOutput,m_TokenString,strlen(m_TokenString));
	}
	
	return TRUE;
}

BOOL cASEParser::Load( LPCTSTR strFileName ,cSceneGraphNode* pOutput)
{	
	BOOL bResult;
	

	m_pSceneRoot = pOutput;

	if (!Open(strFileName))
	{
		return FALSE;
	}

	m_Token =  GetToken(m_TokenString);
	if(m_Token == TOKENR_3DSMAX_ASCIIEXPORT)
	{
		// - version ����
		//m_version	 = Parser.GetFloat();
		SkipBlock();
	}
	else
	{
		TRACE(_T("TOKENR_3DSMAX_ASCIIEXPORT ������\n"));
		return FALSE;
	}

	// - Comment�� ���� ���� �� ���� �ֱ� ������...
	while(m_Token = GetToken(m_TokenString), m_Token == TOKENR_COMMENT)
	{
		// - Comment�� �׳� �����Ѵ�.
		m_Token = GetToken(m_TokenString);
	}

	// 2) Scene Info
	if(m_Token == TOKENR_SCENE)
	{
		Parsing_Scene();		
	}
	// 3) Material List
	//   - Material List�� ������ �ֱ� �ϴ�.
	m_Token = GetToken(m_TokenString);
	if(m_Token == TOKENR_MATERIAL_LIST)
	{
		bResult=Parsing_MaterialList();		
		ASSERT(bResult==TRUE);
	}
	
	

	// 4) Node Objects
	while(m_Token = GetToken(m_TokenString), m_Token != TOKEND_END)
	{

		switch(m_Token)
		{
		case	TOKENR_GEOMOBJECT:
			{

				bResult=Parsing_GeoObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;

			}
			break;

		case	TOKENR_HELPEROBJECT:
			{
				bResult=Parsing_HelperObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;

			}
			break;

		case	TOKENR_SHAPEOBJECT:
			{
				bResult=Parsing_ShapeObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;

			}
			break;

		case	TOKENR_LIGHTOBJECT:
			{
				bResult=Parsing_LightObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;
			}
			break;

		case	TOKENR_CAMERAOBJECT:
			{
				bResult=Parsing_CameraObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;
			}
			break;

		case	TOKENR_GROUP:
			{
				GetString().c_str();
				bResult=Parsing_Group();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;
			}
			break;

		default:
			{					
				TCHAR strToken[256]={0,};				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),strToken,256);							
				TRACE3(_T("���ǵ��� ���� Command�Դϴ�. ( %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__),m_Token,strToken );						
			}		
			ASSERT(bResult==TRUE);
			break;
		};
		
		ASSERT(bResult==TRUE);
	}

	m_arrMatrial.clear();


	// *************************************************************
	// 
	// ���� ��!!!
	// 
	// *************************************************************
	TRACE1(_T("cASEImport Done count%d\n"),m_CNTOBJECT);
	return	TRUE;
}

/*
	Bone,SkinedMesh,Mesh ����� �����𸣴� ������ �غ����� ���߿� ��������
*/
BOOL cASEParser::Parsing_GeoObject()
{	
	UINT totalVertices=0,totalFaces=0,totalBoneRef=0,totalTVertices=0;
	D3DXMATRIX			InverseNodeTM;

	cMeshNode* pNewSceneNode=NULL;
	cRscVertexBuffer* pNewRscVertexBuffer=NULL;
	cRscIndexBuffer* pNewRscIndexBuffer=NULL;
	
	pNewSceneNode=new cMeshNode;		m_pLastObject = pNewSceneNode;
	pNewSceneNode->SetRootNode(m_pSceneRoot);

	//�ν��Ͻ����� ���Ե�����
	vector<BONEREFINFO>&					refArrayBoneRef		= pNewSceneNode->GetArrayBoneRef();
	map<SUBMATINDEX,WORD>&					refMapSubIndexCount	= pNewSceneNode->GetMapSubIndexCount();

	//cRscVertexBuffer�� ������ ����
	vector<NORMALVERTEX>					tempArrayNormalVertex; 
	vector<BLENDVERTEX>						tempArrayBlendVertex;
	//cRscIndexBuffer�� ������ ����
	vector<INDEX_FACE_SUBMATERIAL>		tempArrayFaceIndex;
		
	// cRscVertexBuffer�� ���ؽ� ������ ���� �Ͻ����� ����
	vector<TEXCOORD>	  tempArrayTVertex;
	vector<TEXCOORD>	  tempArrayTVertexOut;
	vector<FACEINDEX16>	  tempArrayTFaceIndex;	
	vector<VNORMAL>		  tempArrayVertexNormal;
	
	// �������� Sphere�� ��������� �ӽ� ����
	D3DXVECTOR3 tempAxisMin=D3DXVECTOR3(0.0f,0.0f,0.0f),tempAxisMax=D3DXVECTOR3(0.0f,0.0f,0.0f);	
	D3DXVECTOR3 tempCenterPos=D3DXVECTOR3(0.0f,0.0f,0.0f);
	float		tempRadius=0.0f;

	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			pNewSceneNode->SetNodeName(GetWString().c_str());				
			break;
		case TOKENR_NODE_PARENT:		
			pNewSceneNode->SetParentName(GetWString().c_str());					
			break;
		case TOKENR_NODE_TM:
			{
				pNewSceneNode->SetNodeTM(GetNodeTM());				
				pNewSceneNode->LinkToParent();		/* NODE_TM�� �ݵ�� ������ �θ𿬰�����Ѵ�.*/

				// ������ ���� �߽����� ���������� NodeTM_INV
				D3DXMatrixInverse(&InverseNodeTM,NULL,&pNewSceneNode->GetNodeTM());
			}				
			break;
		case TOKENR_MESH:
			{

				if (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_START)
					return FALSE;

				while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{
					switch(m_Token)
					{
					case TOKENR_TIMEVALUE:	
						GetInt();
						break;
					case TOKENR_MESH_NUMBONE:
						totalBoneRef=GetInt();
						pNewSceneNode->SetTotalBoneRef(totalBoneRef);
						break;
					case TOKENR_MESH_NUMSKINWEIGHT:
						GetInt();
						break;	
					case TOKENR_MESH_NUMVERTEX:
						totalVertices=GetInt();			
						break;
					case TOKENR_MESH_NUMFACES:
						totalFaces=GetInt();
						break;
					case TOKENR_MESH_VERTEX_LIST:
						{
							if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
								return FALSE;
							while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
							{
								switch(m_Token)
								{
								case TOKENR_MESH_VERTEX:
									if (totalBoneRef==0)
									{
										NORMALVERTEX Item;
										memset(&Item,0,sizeof(Item));
										int index=GetInt();
										GetVector3(&Item.vertex);
										// �����߽� ������ǥ�� �̵�
										D3DXVec3TransformCoord(&Item.vertex,&Item.vertex,&InverseNodeTM);
										tempArrayNormalVertex.push_back(Item);	

										//Bounding Sphere�� ���� �ִ� �ּ� ���
										tempAxisMin.x= min(Item.vertex.x,tempAxisMin.x);
										tempAxisMin.y= min(Item.vertex.y,tempAxisMin.y);
										tempAxisMin.z= min(Item.vertex.z,tempAxisMin.z);

										tempAxisMax.x= max(Item.vertex.x,tempAxisMax.x);
										tempAxisMax.y= max(Item.vertex.y,tempAxisMax.y);
										tempAxisMax.z= max(Item.vertex.z,tempAxisMax.z);		
									}
									else
									{
										BLENDVERTEX Item;
										memset(&Item,0,sizeof(Item));
										int index=GetInt();
										GetVector3(&Item.vertex);		
										// �����߽� ������ǥ�� �̵�
										D3DXVec3TransformCoord(&Item.vertex,&Item.vertex,&InverseNodeTM);
										tempArrayBlendVertex.push_back(Item);	

										//Bounding Sphere�� ���� �ִ� �ּ� ���
										tempAxisMin.x= min(Item.vertex.x,tempAxisMin.x);
										tempAxisMin.y= min(Item.vertex.y,tempAxisMin.y);
										tempAxisMin.z= min(Item.vertex.z,tempAxisMin.z);

										tempAxisMax.x= max(Item.vertex.x,tempAxisMax.x);
										tempAxisMax.y= max(Item.vertex.y,tempAxisMax.y);
										tempAxisMax.z= max(Item.vertex.z,tempAxisMax.z);	
									}						
									break;
								}
							}

						}			
						break;
					case TOKENR_MESH_FACE_LIST:
						{
							//SkipBlock();
							if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
								return FALSE;

							while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
							{
								switch(m_Token)
								{
								case TOKENR_MESH_FACE:						
									int iFace,iMat;
									FACEINDEX16 tFaceIndex;
									iFace=GetInt();			// FaceIndex

									m_Token=GetToken(m_TokenString);		// A:
									tFaceIndex.index[0]=GetInt();			// 0

									m_Token=GetToken(m_TokenString);		// B:
									tFaceIndex.index[2]=GetInt();			// 2

									m_Token=GetToken(m_TokenString);		// C:
									tFaceIndex.index[1]=GetInt();			// 3						

									FindToken(TOKENR_MESH_MTLID);
									iMat=GetInt();
		
									// �����Ʈ���� ID�� Ű�� ID���� ���Ǵ� FACEINDEX���� ī��Ʈ�Ѵ�.
									refMapSubIndexCount[iMat]++;

									INDEX_FACE_SUBMATERIAL temp;		
									temp.faceIndex = tFaceIndex;						
									temp.subMaterialIndex = iMat;									
									tempArrayFaceIndex.push_back(temp);
									break;
								}					
							}
						}	
						break;
					case TOKENR_MESH_FACEMAPLIST:
						{
							SkipBlock();
						}	
						break;
					case TOKENR_MESH_NUMTVERTEX:
						totalTVertices=GetInt();
						tempArrayTVertex.reserve(totalTVertices);

						break;
					case TOKENR_MESH_TVERTLIST:
						{				
							if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
								return FALSE;

							while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
							{
								switch(m_Token)
								{
								case TOKENR_MESH_TVERT:	
									int index=GetInt();

									D3DXVECTOR3 tvertex;
									GetVector3(&tvertex);
									tvertex.z = 1.0f-tvertex.z;
									tempArrayTVertex.push_back(TEXCOORD(tvertex.x,tvertex.z));
									break;
								}					
							}
						}	
						break;
					case TOKENR_MESH_NUMTVFACES:
						SkipBlock();
						break;
					case TOKENR_MESH_TFACELIST:
						{				
							if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
								return FALSE;

							while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
							{
								switch(m_Token)
								{
								case TOKENR_MESH_TFACE:						
									int iTFace;
									FACEINDEX16 tFaceIndex;
									iTFace=GetInt();			// FaceIndex						
									tFaceIndex.index[0]=GetInt();			// 0						
									tFaceIndex.index[2]=GetInt();			// 1						
									tFaceIndex.index[1]=GetInt();			// 2						
									tempArrayTFaceIndex.push_back(tFaceIndex);
									break;
								}					
							}
						}	
						break;

					case TOKENR_MESH_NUMCVERTEX:
						GetInt();
						break;
					case TOKENR_MESH_NORMALS:
						{
							// {
							if(GetToken(m_TokenString) != TOKEND_BLOCK_START)
								return FALSE;

							for (UINT iNUMFACES=0;iNUMFACES<totalFaces;iNUMFACES++)
							{
								// *MESH_FACENORMAL
								if(GetToken(m_TokenString) != TOKENR_MESH_FACENORMAL)
									return FALSE;

								int iRefFace=GetInt();
								GetVector3(NULL);

								for (int triangle=0;triangle<3;triangle++)
								{
									// *MESH_VERTEXNORMAL
									if(GetToken(m_TokenString) != TOKENR_MESH_VERTEXNORMAL)
										return FALSE;


									VNORMAL temp;
									temp.iRefFace= iRefFace;
									temp.iRefVertex= GetInt();		// vertex index						
									GetVector3(&temp.normal);		// vertex normal			
									tempArrayVertexNormal.push_back(temp);					
								}
							}
							// }
							if(GetToken(m_TokenString) != TOKEND_BLOCK_END)
								return FALSE;				
						}
						break;	

					case TOKENR_SKIN_INITTM:
						SkipBlock();
						break;
					case TOKENR_BONE_LIST:
						{
							if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
								return FALSE;

							for (UINT i=0;i<totalBoneRef;i++)
							{
								if(GetToken(m_TokenString)!=TOKENR_BONE)
									return FALSE;
								GetInt();				
								if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
									return FALSE;

								if(GetToken(m_TokenString)!=TOKENR_BONE_NAME)
									return FALSE;				

								BONEREFINFO NewItem;
								NewItem.strNodeName=GetWString();
								NewItem.pBoneRef=NULL;
								refArrayBoneRef.push_back(NewItem);

								if (!FindToken(TOKEND_BLOCK_END))
									return FALSE;
							}

							if (GetToken(m_TokenString)!=TOKEND_BLOCK_END)
								return FALSE;
						}
						break;
					case TOKENR_MESH_WVERTEXS:
						{			
							if(GetToken(m_TokenString) != TOKEND_BLOCK_START)	
								return FALSE;			

							for (UINT iVertex=0;iVertex<tempArrayBlendVertex.size();iVertex++)
							{						
								vector<BONEWEIGHT*>	  arrBoneWeight;

								if(GetToken(m_TokenString) != TOKENR_MESH_WEIGHT)	return FALSE;
								int index=GetInt();

								if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)	return FALSE;
								while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
								{
									switch(m_Token)
									{
									case TOKENR_BONE_BLENGING_WEIGHT:
										int BoneIndex=GetInt();
										float BoneWeight=GetFloat();

										BONEWEIGHT* temp=new BONEWEIGHT;
										temp->bone_index=BoneIndex;
										temp->bone_weight=BoneWeight;
										arrBoneWeight.push_back(temp);
										break;					
									}					
								}

								sort(arrBoneWeight.begin(),arrBoneWeight.end(),BONEWEIGHT::GreatBoneWeight);
								BYTE bindex[4]= { 0,0,0,0};
								float bweight[4]= { 0.0f,0.0f,0.0f,0.0f };	
								for (int iTop=0; (iTop<4)&&(iTop<(int)arrBoneWeight.size());iTop++)
								{
									bindex[iTop]=arrBoneWeight[iTop]->bone_index;
									bweight[iTop]=arrBoneWeight[iTop]->bone_weight;
								}					
								for_each(arrBoneWeight.begin(),arrBoneWeight.end(),FuncDeleteType<BONEWEIGHT*>);

								tempArrayBlendVertex[iVertex].index3 = bindex[3]; 
								tempArrayBlendVertex[iVertex].index2 = bindex[2];
								tempArrayBlendVertex[iVertex].index1 = bindex[1];
								tempArrayBlendVertex[iVertex].index0 = bindex[0];

								tempArrayBlendVertex[iVertex].weight0 = bweight[0]; 
								tempArrayBlendVertex[iVertex].weight1 = bweight[1];
								tempArrayBlendVertex[iVertex].weight2 = bweight[2];
							}
							if(GetToken(m_TokenString) != TOKEND_BLOCK_END)		return FALSE;
						}
						break;			
					}		
				}	
			}	
			break;
		case TOKENR_TM_ANIMATION:
			{			
				cRscTransformAnm* pRscTransformAnm=GetRscTransformAnm(pNewSceneNode->GetLocalTM());
				pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
			}
			break;
		case TOKENR_PROP_MOTIONBLUR:
			GetInt();
			break;
		case TOKENR_PROP_CASTSHADOW:
			GetInt();
			break;
		case TOKENR_PROP_RECVSHADOW:
			GetInt();
			break;
		case TOKENR_WIREFRAME_COLOR:
			GetVector3(NULL);
			break;
		case TOKENR_MATERIAL_REF:	
			{
				UINT MaterialRef=GetInt();
				
				if ( MaterialRef < m_arrMatrial.size())
				{
					pNewSceneNode->SetMatrial( m_arrMatrial[MaterialRef] );
				}
				
			}
			break;	
		}
	}	

	//���ؽ� ���� �ϱ����� Bounding Sphere���ϱ�
	// �ִ� �ּ��� �߰������� Center pos�� ���Ѵ�.
	tempCenterPos.x= tempAxisMin.x + (tempAxisMax.x - tempAxisMin.x)/2.0f;
	tempCenterPos.y= tempAxisMin.y + (tempAxisMax.y - tempAxisMin.y)/2.0f;
	tempCenterPos.z= tempAxisMin.z + (tempAxisMax.z - tempAxisMin.z)/2.0f;

	float MaxLengthSq=0.0f,LengthSq=0.0f;
	if (totalBoneRef==0)
	{
		vector<NORMALVERTEX>::iterator it;
		for (it=tempArrayNormalVertex.begin();it!=tempArrayNormalVertex.end();++it)
		{
			NORMALVERTEX* pNormalVertexItem=&(*it);
			LengthSq=D3DXVec3LengthSq(&D3DXVECTOR3(tempCenterPos - pNormalVertexItem->vertex));
			MaxLengthSq=max(MaxLengthSq,LengthSq);
		}
	}
	else
	{
		vector<BLENDVERTEX>::iterator it;
		for (it=tempArrayBlendVertex.begin();it!=tempArrayBlendVertex.end();++it)
		{
			BLENDVERTEX* pBLENDVERTEXItem=&(*it);
			LengthSq=D3DXVec3LengthSq(&D3DXVECTOR3(tempCenterPos - pBLENDVERTEXItem->vertex));
			MaxLengthSq=max(MaxLengthSq,LengthSq);		
		}
	}
	tempRadius=sqrt(MaxLengthSq);	
	
	cSphere tempSphere;
	tempSphere.Make(tempCenterPos,tempRadius);
	(*pNewSceneNode->GetBoundingSphere())=tempSphere;
	(*pNewSceneNode->GetCullingSphere())=tempSphere;

	// ���� ���ؽ� ����
	// ���ؽ�,�븻 ��ġ��
	if (totalBoneRef==0)
		MergeNormalListIntoVertexList(tempArrayNormalVertex,tempArrayFaceIndex,tempArrayVertexNormal);
	else
		MergeNormalListIntoVertexList(tempArrayBlendVertex,tempArrayFaceIndex,tempArrayVertexNormal);
	
	
	// �ؽ��� ��ǥ���̸鼭 �ؽ�����ǥ �ε��� �����ϱ�, ���ؽ��� ��ġ�鼭 FaceIndex�����ϱ� 
	if (!tempArrayTVertex.empty())
	{	
		//OptimizeTexCoordAndFace(tempArrayTVertexOut,tempArrayTVertex,tempArrayTFaceIndex);		
		if (totalBoneRef==0)
		{
			MergeTexCoordListIntoVertexList(tempArrayNormalVertex,tempArrayFaceIndex,tempArrayTVertex,tempArrayTFaceIndex);
		}
		else
		{
			MergeTexCoordListIntoVertexList(tempArrayBlendVertex,tempArrayFaceIndex,tempArrayTVertex,tempArrayTFaceIndex);
		}
	}

	// �����Ʈ���� ID���� FACEINDEX����
	sort(tempArrayFaceIndex.begin(),tempArrayFaceIndex.end(),INDEX_FACE_SUBMATERIAL::LessFaceIndex);	
	
	// ���ҽ� ���ؽ� ����   ���� -> �����ͺ��� -> �޽�����
	if (!tempArrayNormalVertex.empty())
	{
		pNewSceneNode->SetTotalVertex(tempArrayNormalVertex.size());

		pNewRscVertexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscVertexBuffer(
			sizeof(NORMALVERTEX)*tempArrayNormalVertex.size());

		NORMALVERTEX* pVertices=(NORMALVERTEX*)pNewRscVertexBuffer->Lock();
		for (UINT i=0;i< tempArrayNormalVertex.size();i++)
		{
			memcpy(&pVertices[i],&tempArrayNormalVertex[i],sizeof(NORMALVERTEX));
		}	
		pNewRscVertexBuffer->Unlock();	
		pNewSceneNode->SetRscVetextBuffer(pNewRscVertexBuffer);
	}
	else if (!tempArrayBlendVertex.empty())
	{
		pNewSceneNode->SetTotalVertex(tempArrayBlendVertex.size());

		pNewRscVertexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscVertexBuffer(
			sizeof(BLENDVERTEX)*tempArrayBlendVertex.size());

		BLENDVERTEX* pVertices=(BLENDVERTEX*)pNewRscVertexBuffer->Lock();	
		for (UINT i=0;i< tempArrayBlendVertex.size();i++)
		{
			memcpy(&pVertices[i],&tempArrayBlendVertex[i],sizeof(BLENDVERTEX));
		}
		pNewRscVertexBuffer->Unlock();	
		pNewSceneNode->SetRscVetextBuffer(pNewRscVertexBuffer);
	}
	
	
	// ���ҽ� �ε��� ���� ����-> �����ͺ��� -> �޽� ����
	if (!tempArrayFaceIndex.empty())
	{
		pNewRscIndexBuffer = g_pD3DFramework->GetResourceMng()->CreateRscIndexBuffer(
		sizeof(FACEINDEX16)*tempArrayFaceIndex.size());

		FACEINDEX16* pIndices=(FACEINDEX16*)pNewRscIndexBuffer->Lock();
		for (UINT i=0;i< tempArrayFaceIndex.size();i++)
		{
			memcpy(&pIndices[i],&tempArrayFaceIndex[i].faceIndex,sizeof(FACEINDEX16));			
		}
		pNewRscIndexBuffer->Unlock();
		pNewSceneNode->SetRscIndexBuffer(pNewRscIndexBuffer);
	}			

	return TRUE;
}



BOOL cASEParser::Parsing_MaterialList()
{
	BOOL bRet=TRUE;
	int nNUMMaterial,iMaterial;
	if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	return FALSE;	
	if (GetToken(m_TokenString) != TOKENR_MATERIAL_COUNT)	return FALSE;	// *MATERIAL_COUNT

	nNUMMaterial=GetInt();
	for (int i=0;i<nNUMMaterial;i++)
	{
		cMaterialEx Matrial;		
		if(GetToken(m_TokenString) != TOKENR_MATERIAL)		// *MATERIAL	
			return FALSE;			
		iMaterial=GetInt();									// index
		if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
			return FALSE;		

		while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
		{
			switch(m_Token)
			{
			case TOKENR_MATERIAL_AMBIENT:
				Matrial.Ambient.r=GetFloat();
				Matrial.Ambient.g=GetFloat();
				Matrial.Ambient.b=GetFloat();
				break;
			case TOKENR_MATERIAL_DIFFUSE:
				Matrial.Diffuse.r=GetFloat();
				Matrial.Diffuse.g=GetFloat();
				Matrial.Diffuse.b=GetFloat();
				break;
			case TOKENR_MATERIAL_SPECULAR:
				Matrial.Specular.r=GetFloat();
				Matrial.Specular.g=GetFloat();
				Matrial.Specular.b=GetFloat();			
				break;
			case TOKENR_MATERIAL_SHINE:
				Matrial.SpecularMultiplier =GetFloat();
				break;
			case TOKENR_MATERIAL_SHINESTRENGTH:
				Matrial.Power=GetFloat();
				break;
			case TOKENR_MATERIAL_TRANSPARENCY:
				Matrial.Alpha=GetFloat();
				break;
			case TOKENR_MAP_OPACITY:
				FindToken(TOKEND_BLOCK_START);
				FindToken(TOKEND_BLOCK_END);
				break;
			
			case TOKENR_MAP_REFLECT:
			case TOKENR_MAP_REFRACT:
				SkipBlock();
				break;
			case TOKENR_MAP_BUMP:
				{
					SkipBlock();
				}				
				break;
			case TOKENR_MAP_DIFFUSE:
				{
					if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
						return FALSE;						
					while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
					{
						switch(m_Token)
						{
						case TOKENR_BITMAP:
					
							wstring strFileName=GetWString().c_str();
							cRscTexture* pRscTexture= g_pD3DFramework->GetResourceMng()->CreateRscTexture(strFileName.c_str());
							if(pRscTexture==NULL)
								TRACE1(L"MAP_DIFFUSE: %s �����̾����ϴ�.\n",strFileName.c_str());
							Matrial.SetDiffuseRscTexture(pRscTexture);
							break;
						}		
					}
				}
				break;
			
			case TOKENR_NUMSUBMTLS:
				{
					int nNUMSUBMTLS=GetInt();
					for (int iNUMSUBMTLS=0 ; iNUMSUBMTLS < nNUMSUBMTLS ; iNUMSUBMTLS++)
					{
						cMaterialEx SubMatrial;						
						FindToken(TOKENR_SUBMATERIAL);	// *SUBMATERIAL
						GetInt();						// index
						if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	return FALSE;						
						while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
						{
							switch(m_Token)
							{
							case TOKENR_MATERIAL_AMBIENT:
								SubMatrial.Ambient.r=GetFloat();
								SubMatrial.Ambient.g=GetFloat();
								SubMatrial.Ambient.b=GetFloat();
								break;
							case TOKENR_MATERIAL_DIFFUSE:
								SubMatrial.Diffuse.r=GetFloat();
								SubMatrial.Diffuse.g=GetFloat();
								SubMatrial.Diffuse.b=GetFloat();
								break;
							case TOKENR_MATERIAL_SPECULAR:
								SubMatrial.Specular.r=GetFloat();
								SubMatrial.Specular.g=GetFloat();
								SubMatrial.Specular.b=GetFloat();			
								break;
							case TOKENR_MATERIAL_SHINE:
								SubMatrial.SpecularMultiplier=GetFloat();
								break;
							case TOKENR_MATERIAL_SHINESTRENGTH:
								SubMatrial.Power=GetFloat();
								break;
							case TOKENR_MATERIAL_TRANSPARENCY:
								SubMatrial.Alpha=GetFloat();
								break;
							case TOKENR_MAP_OPACITY:
							case TOKENR_MAP_REFLECT:
							case TOKENR_MAP_REFRACT:	
								SkipBlock();
								break;
							case TOKENR_MAP_BUMP:
								{
									SkipBlock();
								}								
								break;
							case TOKENR_MAP_DIFFUSE:
								{
									if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
										return FALSE;						
									while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
									{			
										switch(m_Token)
										{
										case TOKENR_BITMAP:
											wstring strFileName=GetWString().c_str();
											cRscTexture* pRscTexture= g_pD3DFramework->GetResourceMng()->CreateRscTexture(strFileName.c_str());
											if(pRscTexture==NULL)
												TRACE1(L"MAP_DIFFUSE: %s �����̾����ϴ�.\n",strFileName.c_str());
											SubMatrial.SetDiffuseRscTexture(pRscTexture);
										break;
										}
									}									
								}
								break;							
							case TOKENR_NUMSUBMTLS:
								{
									ASSERT(0&&L"Matrial�� SubMatrial���� SubMatrial�� �Ǵٽ� ����ϰ�����");
								}
								break;
							}						
						}
						//����
						Matrial.InsertSubMaterial(SubMatrial);
					}

				}//case TOKENR_NUMSUBMTLS:
				break;		
			}//switch			

		}//while (m_Token=GetLexer()->GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
		
		m_arrMatrial.push_back(Matrial);
	}//for (int i=0;i<nNUMMaterial;i++)	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_END) 
		return FALSE;	// }	

	
	return bRet;
}


BOOL cASEParser::Parsing_HelperObject()
{	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	cHelperNode* pNewSceneNode=new cHelperNode;
	pNewSceneNode->SetRootNode(m_pSceneRoot);

	m_pLastObject = pNewSceneNode;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			pNewSceneNode->SetNodeName(GetWString().c_str()); 			
			break;
		case TOKENR_NODE_PARENT:			
			pNewSceneNode->SetParentName(GetWString().c_str());			
			break;
		case TOKENR_HELPER_CLASS:
			GetWString();
			break;
		case TOKENR_NODE_TM:
			{		
				pNewSceneNode->SetNodeTM(GetNodeTM());				
				pNewSceneNode->LinkToParent();		/* NODE_TM�� �ݵ�� ������ �θ𿬰�����Ѵ�.*/				
			}	
			break;		
		case	TOKENR_BOUNDINGBOX_MIN:
			GetVector3(NULL);
			break;
		case	TOKENR_BOUNDINGBOX_MAX:
			GetVector3(NULL);
			break;		
		case TOKENR_TM_ANIMATION:
			{						
				cRscTransformAnm* pRscTransformAnm=GetRscTransformAnm(pNewSceneNode->GetLocalTM());
				pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
			}

			break;		
		}		
	}				
	return TRUE;
}

BOOL cASEParser::Parsing_ShapeObject()
{
	// {
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	cShapeNode*	pNewSceneNode=new cShapeNode;
	pNewSceneNode->SetRootNode(m_pSceneRoot);

	m_pLastObject = pNewSceneNode;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			pNewSceneNode->SetNodeName(GetWString().c_str()); 						
			break;		
		case TOKENR_NODE_PARENT:
			pNewSceneNode->SetParentName(GetWString().c_str());			
			break;
		case TOKENR_SHAPE_LINECOUNT:
			{
				int LineCnt;	
				LineCnt=GetInt();
				for (int index=0;index<LineCnt;index++)
				{
					GetToken(m_TokenString);
					GetInt();
					SkipBlock();
				}
			}
			break;
		case TOKENR_NODE_TM:
			{	
				pNewSceneNode->SetNodeTM(GetNodeTM());	
				pNewSceneNode->LinkToParent();		/* NODE_TM�� �ݵ�� ������ �θ𿬰�����Ѵ�.*/		
			}	
			break;		
		case TOKENR_TM_ANIMATION:			
			{						
				cRscTransformAnm* pRscTransformAnm=GetRscTransformAnm(pNewSceneNode->GetLocalTM());
				pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
			}
			break;			
		}
	}
	return TRUE;
}

BOOL cASEParser::Parsing_LightObject()
{	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;


	cLightNode* pNewSceneNode=new cLightNode;	
	m_pLastObject = pNewSceneNode;
	pNewSceneNode->SetRootNode(m_pSceneRoot);	

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			pNewSceneNode->SetNodeName(GetWString().c_str()); 	
			break;
		case TOKENR_LIGHT_TYPE:
			GetIdentifier(NULL);
			break;
		case TOKENR_NODE_PARENT:
			pNewSceneNode->SetParentName(GetWString().c_str());	
			break;
		case TOKENR_NODE_TM:
			{
				pNewSceneNode->SetNodeTM(GetNodeTM());			
				pNewSceneNode->LinkToParent();		/* NODE_TM�� �ݵ�� ������ �θ𿬰�����Ѵ�.*/
			}	
			break;		
		case TOKENR_LIGHT_SHADOWS:
			GetIdentifier(NULL);
			break;
		case TOKENR_LIGHT_USELIGHT:
			GetInt();
			break;
		case TOKENR_LIGHT_SPOTSHAPE:
			GetIdentifier(NULL);
			break;
		case TOKENR_LIGHT_USEGLOBAL:
			GetInt();
			break;
		case TOKENR_LIGHT_ABSMAPBIAS:
			GetInt();
			break;
		case TOKENR_LIGHT_OVERSHOOT:
			GetInt();
			break;
		case TOKENR_LIGHT_SETTINGS:

			SkipBlock();
			break;				

		case TOKENR_TM_ANIMATION:	
			{						
				cRscTransformAnm* pRscTransformAnm=GetRscTransformAnm(pNewSceneNode->GetLocalTM());
				pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
			}
			break;	
		}		
	}	
	
	return TRUE;
}

BOOL cASEParser::Parsing_CameraObject()
{
	// {
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	cCameraNode* pNewSceneNode=new cCameraNode;
	pNewSceneNode->SetRootNode(m_pSceneRoot);	

	m_pLastObject = pNewSceneNode;
	
	BOOL bLoadCameraNodeTM=FALSE;
	BOOL bLoadCameraAnmTM=FALSE;
	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			pNewSceneNode->SetNodeName(GetWString().c_str());
			break;
		case TOKENR_NODE_PARENT:
			pNewSceneNode->SetParentName(GetWString().c_str());					
			break;
		case TOKENR_CAMERA_TYPE:
			GetToken(m_TokenString);
			break;
		case TOKENR_NODE_TM:			
			if (!bLoadCameraNodeTM)	
			{
				D3DXMATRIX nodeTM,NodeRotation;
				nodeTM=GetNodeTM();		// Camera Node TM

				//���Ǻ���
				D3DXMatrixRotationAxis(&NodeRotation,&D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXToRadian(90));
				nodeTM = NodeRotation * nodeTM;				

				pNewSceneNode->SetNodeTM(nodeTM);	
				pNewSceneNode->LinkToParent();		/* NODE_TM�� �ݵ�� ������ �θ𿬰�����Ѵ�.*/
				bLoadCameraNodeTM=TRUE;
			}		
			else
			{
				SkipBlock();	// TargetTM�� ������������Ƿ� Pass
			}
			
			break;
		case TOKENR_CAMERA_SETTINGS:
			{
				if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{
					switch(m_Token)
					{
					case TOKENR_TIMEVALUE:	
						GetInt(); 
						break;
					case TOKENR_CAMERA_NEAR:
						GetFloat(); 
						break;
					case TOKENR_CAMERA_FAR:
						GetFloat(); 
						break;
					case TOKENR_CAMERA_FOV:
						pNewSceneNode->SetFOV(GetFloat());
						break;
					case TOKENR_CAMERA_TDIST:
						GetFloat(); 
						break;

					}
				}
			}
			break;
		case TOKENR_CAMERA_ANIMATION:
			SkipBlock();
			break;
		case TOKENR_TM_ANIMATION:		
			if (bLoadCameraAnmTM==FALSE)
			{				
				cRscTransformAnm* pRscTransformAnm=GetRscTransformAnm(pNewSceneNode->GetLocalTM());
				pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
				bLoadCameraAnmTM=TRUE;												
			}			
			else
			{
				SkipBlock();// TargetTM�� ������������Ƿ� Pass
			}
			break;		
		}
	}	

	
	return TRUE;	
}

BOOL cASEParser::Parsing_Group()
{	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while(m_Token = GetToken(m_TokenString), m_Token != TOKEND_BLOCK_END)
	{
		BOOL bResult;
		switch(m_Token)
		{
		case	TOKENR_GEOMOBJECT:
			{

				bResult=Parsing_GeoObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;

			}
			break;

		case	TOKENR_HELPEROBJECT:
			{
				bResult=Parsing_HelperObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;

			}
			break;

		case	TOKENR_SHAPEOBJECT:
			{
				bResult=Parsing_ShapeObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;

			}
			break;

		case	TOKENR_LIGHTOBJECT:
			{
				bResult=Parsing_LightObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;
			}
			break;

		case	TOKENR_CAMERAOBJECT:
			{
				bResult=Parsing_CameraObject();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;
			}
			break;

		case	TOKENR_GROUP:
			{
				bResult=Parsing_Group();
				ASSERT(bResult==TRUE);
				m_CNTOBJECT++;
			}
			break;

		default:
			{					
				TCHAR strToken[256]={0,};				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),strToken,256);							
				TRACE3(_T("���ǵ��� ���� Command�Դϴ�. ( %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__),m_Token,strToken );						
			}				
			break;

		};		
	}
	return TRUE;
}

BOOL cASEParser::Parsing_Scene()
{
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while(m_Token = GetToken(m_TokenString), m_Token != TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_SCENE_FILENAME:
			m_SceneTime.FILENAME=GetString();
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_SceneTime.FIRSTFRAME=GetInt();			
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_SceneTime.LASTFRAME=GetInt();	
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_SceneTime.FRAMESPEED=GetInt();	
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_SceneTime.TICKSPERFRAME=GetInt();	
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			GetFloat();
			GetFloat();
			GetFloat();
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			GetFloat();
			GetFloat();
			GetFloat();
			break;
		case TOKENR_SCENE_ENVMAP:
			FindToken(TOKEND_BLOCK_START);
			FindToken(TOKEND_BLOCK_END);

			break;
		}
	}
	// 1�����Ӵ� ms���ϱ�
	m_SceneTime.EX_MSPERFRAME= 1000/m_SceneTime.FRAMESPEED;
	// 1ms�� ƽ�� ���ϱ� 
	m_SceneTime.EX_TICKSPERMS= m_SceneTime.TICKSPERFRAME/m_SceneTime.EX_MSPERFRAME;
	// ������ ������ ms���ϱ�
	m_SceneTime.LASTFRAMEMS= m_SceneTime.LASTFRAME*m_SceneTime.EX_MSPERFRAME;
	return TRUE;
}


// �븻����Ʈ�� ���ؽ� ����Ʈ�� ��ģ��. 
// arrVertex [in,out] , arrVNormal [in] ,  arrFaceIndex[in,out]
template <typename T>
void cASEParser::MergeNormalListIntoVertexList(vector<T>& arrVertex,
								   vector<INDEX_FACE_SUBMATERIAL>& arrFaceIndex,
								   const vector<VNORMAL>& arrVNormal)
{
	vector<list<WORD>>						arrVRefFaceList;	// arrVertex�� �����ϴ� �ε����������� FACE����Ʈ�� �����.
	vector<list<pair<D3DXVECTOR3,WORD>>>    arrVRefNormalInsert;		// arrVertex�� �����ϴ� �������� ���� WORD�� �̹������ϴ� �븻������ ���ؽ��� �ε���

	if (arrFaceIndex.empty())
		return;

	arrVRefFaceList.resize(arrVertex.size());
	arrVRefNormalInsert.resize(arrVertex.size());

	//1. arrVertex�� �����ϴ� �ε����������� FACEINDEX����Ʈ�� �����.
	// ��. �ε������� ����Ʈ�� ��� FACEINDEX���� ��� arrVertex�� Ư�� �ε�����ǥ�� ����Ѵ�.
	int nFace=(int)arrFaceIndex.size();
	for (int iFace=0;iFace<nFace;iFace++ )
	{
		arrVRefFaceList[arrFaceIndex[iFace].faceIndex.index[0]].push_back(iFace);	
		arrVRefFaceList[arrFaceIndex[iFace].faceIndex.index[1]].push_back(iFace);
		arrVRefFaceList[arrFaceIndex[iFace].faceIndex.index[2]].push_back(iFace);
	}

	//2. arrFaceNormal �� arrVertex�� ������ arrFace���� ���ؽ��ε����� �����Ѵ�.
	WORD nVNormal=(WORD)arrVNormal.size();
	for (WORD iVNormal=0;iVNormal<nVNormal;iVNormal++)
	{
		list<pair<D3DXVECTOR3,WORD>>* pListNormalnsert;


		WORD iRefVertex=arrVNormal[iVNormal].iRefVertex;
		pListNormalnsert=&arrVRefNormalInsert[iRefVertex];

		if(pListNormalnsert->empty())
		{	// arrVertex �ε����� �븻���� ������ �׳� �ִ´�.			
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
				{	//�븻�� ����Ʈ�� �����ϸ� �ε����� �����Ѵ�.
					WORD iExist=(*it).second;
					WORD iRefFace=arrVNormal[iVNormal].iRefFace;

					if (arrFaceIndex[iRefFace].faceIndex.index[0] == iRefVertex )
						arrFaceIndex[iRefFace].faceIndex.index[0] = iExist;

					if (arrFaceIndex[iRefFace].faceIndex.index[1] == iRefVertex )
						arrFaceIndex[iRefFace].faceIndex.index[1] = iExist;

					if (arrFaceIndex[iRefFace].faceIndex.index[2]== iRefVertex )
						arrFaceIndex[iRefFace].faceIndex.index[2] = iExist;					

					break;
				}
			}

			// �븻 �������� ������ ���ο� ���ؽ� �߰��Ѵ�.
			if (it == pListNormalnsert->end())
			{
				arrVertex.push_back(arrVertex[iRefVertex]);
				WORD iInsert=(WORD)arrVertex.size()-1;
				arrVertex[iInsert].normal = *pNewNormal;

				// �ε����� �ش�Ǵ� �븻����Ʈ�� �븻��,���� ���� �ε��� �߰�
				arrVRefNormalInsert[iRefVertex].push_back(make_pair(*pNewNormal,iInsert));
				WORD iRefFace=arrVNormal[iVNormal].iRefFace;

				if (arrFaceIndex[iRefFace].faceIndex.index[0] == iRefVertex )
					arrFaceIndex[iRefFace].faceIndex.index[0] = iInsert;

				if (arrFaceIndex[iRefFace].faceIndex.index[1] == iRefVertex )
					arrFaceIndex[iRefFace].faceIndex.index[1] = iInsert;

				if (arrFaceIndex[iRefFace].faceIndex.index[2]== iRefVertex )
					arrFaceIndex[iRefFace].faceIndex.index[2] = iInsert;					
			}
		}
	}
}


void cASEParser::OptimizeTexCoordAndFace(vector<TEXCOORD>& arrTexCoordOut,
								const vector<TEXCOORD>& arrTexCoordIn,
								vector<FACEINDEX16>& arrTFaceIndexInOut)
{
	map<float,map<float,WORD>>	mapUVINDEX;
	vector<list<int>>			arrTVIndexedFaceIndexList;	// �ش���ؽ��� ����ϴ� ���̽��ε����� ����Ʈ�� ���ؽ��ε����� �ε���ȭ

	//���� �׼����� ���� �ʱ��Ҵ�
	if (arrTexCoordIn.empty())
		return;

	arrTVIndexedFaceIndexList.resize(arrTexCoordIn.size());
	// arrTexCoordIn�� �����ϴ� �ε����������� FACE����Ʈ�� �����.
	// ��. �ε������� ����Ʈ�� ��� FACE���� ��� arrTexCoordIn�� Ư�� �ε�����ǥ�� ����Ѵ�.
	int nTFace=(int)arrTFaceIndexInOut.size();
	for (int iTFace=0;iTFace<nTFace;iTFace++ )
	{
		arrTVIndexedFaceIndexList[arrTFaceIndexInOut[iTFace].index[0]].push_back(iTFace);	
		arrTVIndexedFaceIndexList[arrTFaceIndexInOut[iTFace].index[1]].push_back(iTFace);
		arrTVIndexedFaceIndexList[arrTFaceIndexInOut[iTFace].index[2]].push_back(iTFace);
	}

	// arrTexCoordIn �� ��ü�� ���� ���ÿ� UVINDEX ���� ����� 
	// ������ �ؽ�����ǥ(arrTexCoordOpt) �� �����ϸ� ���̽�����Ʈ(arrTFaceIndex)�� �ؽ�����ǥ �ε����� �����Ѵ�.
	WORD nNumTexCoord=(WORD)arrTexCoordIn.size();
	for (WORD iTexCoordIn=0;iTexCoordIn<nNumTexCoord;iTexCoordIn++)
	{
		pair<map<float,map<float,WORD>>::iterator	,bool> retItInsertU;
		pair<map<float,WORD>::iterator				,bool> retItInsertV;

		map<float,WORD> mapVINDEX;
		map<float,WORD>* pmapVINDEX;

		// U��ǥ�� �� mapVINDEX ����
		retItInsertU= mapUVINDEX.insert(make_pair(arrTexCoordIn[iTexCoordIn].first,mapVINDEX));
		pmapVINDEX=&(retItInsertU.first->second);		
		retItInsertV=pmapVINDEX->insert(make_pair(arrTexCoordIn[iTexCoordIn].second,0));

		WORD iTexCoordOut;
		if (retItInsertV.second)	// V��ǥ ���� ���� (�������� ����)
		{			
			arrTexCoordOut.push_back(arrTexCoordIn[iTexCoordIn]);
			retItInsertV.first->second=(WORD)arrTexCoordOut.size()-1;		
			iTexCoordOut=retItInsertV.first->second;
		}
		else						// V��ǥ ���� ���� (�������� ����)
		{
			iTexCoordOut=retItInsertV.first->second;		
		}

		// iTexCoordIn,iTexCoordOut �� �̿���  FACE���� �ε��� ����
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

// arrVertexInOut    �������ϴ� ���ؽ��� �ؽ�����ǥ�� ������ �ٸ� ������ ��ȿ�Ѱ� �����Ѵٴ� �������Ѵ�. (���ؽ�,�븻,����ġ������,,)
// arrFaceIndexInOut ���̽��� ���̴� ��ġ���� �ε���,	
// arrTexCoordIn �� �ؽ��� ��ǥ���� 
// arrTFaceIndexIn   ���̽��� ���̴� �ؽ��� ��ǥ�ε��� 
template <typename T>
void cASEParser::MergeTexCoordListIntoVertexList(vector<T>& arrVertexInOut,
									 vector<INDEX_FACE_SUBMATERIAL>& arrVFaceIndexInOut,
									 const vector<TEXCOORD>& arrTexCoordIn,
									 const vector<FACEINDEX16>& arrTFaceIndexIn)
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
	//�����׼����� ���� �Ҵ�
	arrVIndexedTCList.resize(arrVertexInOut.size());

	int nFace=(int)arrVFaceIndexInOut.size();
	for (int iFace=0;iFace<nFace;iFace++)
	{
		FACEINDEX16*		 pVFace=&arrVFaceIndexInOut[iFace].faceIndex;
		const FACEINDEX16* pTFace=&arrTFaceIndexIn[iFace];

		for (int i=0;i<3;i++)
		{
			int iRefVertex=pVFace->index[i];					// ���ؽ� �ε���
			int iRefTexCoord=arrTFaceIndexIn[iFace].index[i];	//�ؽ���ǥ �ε���
			const TEXCOORD* pTexcoord=&arrTexCoordIn[iRefTexCoord]; //�ؽ���ǥ ���


			if(arrVIndexedTCList[iRefVertex].empty())			// ���ؽ��� �ؽ���ǥ ����Ʈ���� ������
			{
				arrVertexInOut[iRefVertex].tex = *pTexcoord;	// ���ؽ�����Ʈ�� ���׽��� �ؽ���ǥ �߰�
				arrVIndexedTCList[iRefVertex].push_back(make_pair(*pTexcoord,iRefVertex)); //����Ʈ�� �ؽ���ǥ�߰�
			}
			else // ���ؽ��ε����� �ؽ���ǥ ����Ʈ�� �ϳ��̻� ����ִ�.
			{				
				list<pair<TEXCOORD,WORD>>* pListTexcoordInsert=&arrVIndexedTCList[iRefVertex];
				list<pair<TEXCOORD,WORD>>::iterator it=pListTexcoordInsert->begin();
				for ( ; it!=pListTexcoordInsert->end();++it)
				{					
					//�ؽ���ǥ�� �������̸� �̹� �������ε����� ����
					if( (*it).first == *pTexcoord )
					{						
						pVFace->index[i]=(*it).second;							
						break;
					}
				}
				//������ ������
				if (it == pListTexcoordInsert->end())
				{
					//���ؽ� �ϳ��� ����� ���� ��ġ���� �� �ε���ȭ�� ����Ʈ���ؽ��߰� 
					T vertex;
					memcpy(&vertex,&arrVertexInOut[iRefVertex],sizeof(vertex));
					vertex.tex=*pTexcoord;
					arrVertexInOut.push_back(vertex);					

					//���ο� �ε���
					iRefVertex=(int)arrVertexInOut.size()-1;

					//���̽� �ε��� ����
					pVFace->index[i]=iRefVertex;					

					//����Ʈ ����,�ؽ��߰�
					list<pair<TEXCOORD,WORD>> temp;
					arrVIndexedTCList.push_back(temp);
					arrVIndexedTCList[iRefVertex].push_back(make_pair(*pTexcoord,iRefVertex));
				}
			}
		}	
	}
}

/*
	�����Ҷ� localTM�� �ʿ��ϴ�.
	cRscTransformAnm�� ���ҽ��Ŵ������� �������� ä���� �����Ѵ�.
*/
cRscTransformAnm* cASEParser::GetRscTransformAnm(const D3DXMATRIX& localTM)
{
	cRscTransformAnm* pRscTransformAnm=NULL;
	vector<POSKEY*> arrayPOSKEY;
	vector<ROTKEY*> arrayROTKEY;
	vector<SCLKEY*> arraySCLKEY;

	pRscTransformAnm = g_pD3DFramework->GetResourceMng()->CreateRscTransformAnm();
	vector<TRANSFORMANM*>& refArrayTRANSFORMANM=pRscTransformAnm->GetArrayTransformAnm();

	m_Token=GetToken(m_TokenString);
	if (m_Token!=TOKEND_BLOCK_START)
		return FALSE;

	while(m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			{

			}
			break;
		case TOKENR_CONTROL_POS_TRACK:
			{
				if(GetToken(m_TokenString)!=TOKEND_BLOCK_START)
					return FALSE;	

				while(m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{
					if(m_Token!=TOKENR_CONTROL_POS_SAMPLE)
						return FALSE;										

					POSKEY* pNewItem=new POSKEY;
					pNewItem->AnmTick = GetInt();
					GetVector3(&pNewItem->TranslationAccum);
					arrayPOSKEY.push_back(pNewItem);							
				}
			}
			break;
		case TOKENR_CONTROL_ROT_TRACK:
			{
				if(GetToken(m_TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while(m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{	
					if(m_Token!=TOKENR_CONTROL_ROT_SAMPLE)
						return FALSE;	

					float ang;
					D3DXVECTOR3 axis;
					ROTKEY* pNewItem=new ROTKEY;
					pNewItem->AnmTick = GetInt();				

					GetVector3(&axis);
					ang = GetFloat();					

					D3DXQuaternionRotationAxis(&pNewItem->RotationAccum,&axis,ang);
					arrayROTKEY.push_back(pNewItem);
				}
			}
			break;
		case TOKENR_CONTROL_SCALE_TRACK:
			{				
				if( GetToken(m_TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while(m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{

				}
			}
			break;
		}
	} 
	
	// ����
	InterpolateTransformAnm(refArrayTRANSFORMANM,arraySCLKEY,arrayROTKEY,arrayPOSKEY,localTM,DEF_INTERPOLATE_TIME );

	//�ӽ� ���� ����
	for_each(arraySCLKEY.begin(),arraySCLKEY.end(),FuncDeleteType<SCLKEY*>);	arraySCLKEY.clear();	
	for_each(arrayROTKEY.begin(),arrayROTKEY.end(),FuncDeleteType<ROTKEY*>);	arrayROTKEY.clear();
	for_each(arrayPOSKEY.begin(),arrayPOSKEY.end(),FuncDeleteType<POSKEY*>);	arrayPOSKEY.clear();
	
	// ������ �ִϸ��̼� ������ ������ ���ҽ������� NULL����
	if( refArrayTRANSFORMANM.empty() )
	{
		pRscTransformAnm->Release();
		pRscTransformAnm=NULL;
	}			
	return pRscTransformAnm;
}

void cASEParser::InterpolateTransformAnm( vector<TRANSFORMANM*>& refArrayTRANSFORMANM,
										 const vector<SCLKEY*>& arraySCLKEY,
										 const vector<ROTKEY*>& arrayROTKEY,
										 const vector<POSKEY*>& arrayPOSKEY,										
										 const D3DXMATRIX& localTM,
										 const DWORD interpolateTime )
{
	vector<ANMKEY*> arrayANMKEY;

	UINT totalInterpolateKey=0;
	DWORD anmEndTick=0,interpolateTick=0;
	if (!arraySCLKEY.empty())	anmEndTick = max(anmEndTick, arraySCLKEY[arraySCLKEY.size()-1]->AnmTick);	
	if (!arrayROTKEY.empty())	anmEndTick = max(anmEndTick, arrayROTKEY[arrayROTKEY.size()-1]->AnmTick);
	if (!arrayPOSKEY.empty())	anmEndTick = max(anmEndTick, arrayPOSKEY[arrayPOSKEY.size()-1]->AnmTick);
	if (anmEndTick==0)
	{		
		return;
	}

	D3DXMATRIX SCLTM,ROTTM,POSTM;
	D3DXMatrixIdentity(&SCLTM);
	D3DXMatrixIdentity(&ROTTM);
	D3DXMatrixIdentity(&POSTM);

	ANMKEY localTM_anmkey;
	D3DXMatrixDecompose(
		&localTM_anmkey.ScaleDelta,
		&localTM_anmkey.RotationDelta,
		&localTM_anmkey.TranslationDelta,
		&localTM);

	D3DXMatrixScaling(&SCLTM,
		localTM_anmkey.ScaleDelta.x,
		localTM_anmkey.ScaleDelta.y,
		localTM_anmkey.ScaleDelta.z);
	D3DXMatrixRotationQuaternion(&ROTTM,
		&localTM_anmkey.RotationDelta);
	D3DXMatrixTranslation(&POSTM,
		localTM_anmkey.TranslationDelta.x,
		localTM_anmkey.TranslationDelta.y,
		localTM_anmkey.TranslationDelta.z);


	//1. ȸ��Ű������ ������ȯ
	D3DXQUATERNION curr_q, prev_q, accum_q;
	D3DXQuaternionIdentity(&curr_q);
	D3DXQuaternionIdentity(&prev_q);
	D3DXQuaternionIdentity(&accum_q);

	vector<ROTKEY*>::const_iterator rot_it=arrayROTKEY.begin();
	for ( ; rot_it!=arrayROTKEY.end() ; rot_it++)
	{
		ROTKEY* pItem;		
		pItem= *rot_it;

		if(pItem->AnmTick == 0)
		{
			pItem->RotationAccum = localTM_anmkey.RotationDelta;			
		}
		curr_q = pItem->RotationAccum;				
		D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//���ʹϾ� ����
		pItem->RotationAccum=accum_q;
		prev_q=accum_q;
	}

	//2. ������ ����� TRANSFORMANM ��ü ũ�ⱸ�ϱ�
	interpolateTick = interpolateTime* m_SceneTime.EX_TICKSPERMS;
	totalInterpolateKey  = anmEndTick / interpolateTick;
	if(totalInterpolateKey==0)	// totalANNM�� 0�̸� �ִϸ��̼��� �ǹ̰�����.
	{		
		return;
	}
	// �Ҵ��� ���� ũ����
	totalInterpolateKey	+= 1;		


	//3. ANMKEY�� �Ҵ�	
	for ( UINT i=0;i<totalInterpolateKey;i++)
	{		
		ANMKEY* pNewItem=new ANMKEY;
		memset(pNewItem,0,sizeof(ANMKEY));
		arrayANMKEY.push_back(pNewItem);	
	}	


	int iANMKEY;
	DWORD accum_AnmTick=0;	
	//4. SCL�����Ͽ� ANMKEY�� �ֱ�	
	for (UINT i=0;i<arraySCLKEY.size();i++)
	{	
		if (arraySCLKEY.size()==0)
			break;
	}

	//5. ROT�����Ͽ� ANMKEY�� �ֱ�
	ROTKEY prev_rotkey,curr_rotkey,temp_rotkey;
	memset(&prev_rotkey,0,sizeof(prev_rotkey));	
	memset(&curr_rotkey,0,sizeof(curr_rotkey));
	memset(&temp_rotkey,0,sizeof(temp_rotkey));

	for (UINT i=0;i<arrayROTKEY.size();i++)
	{
		curr_rotkey.RotationAccum = arrayROTKEY[i]->RotationAccum;
		curr_rotkey.AnmTick = arrayROTKEY[i]->AnmTick;
		accum_AnmTick=prev_rotkey.AnmTick;
		while (accum_AnmTick < curr_rotkey.AnmTick)
		{
			if ( (accum_AnmTick+interpolateTick) > curr_rotkey.AnmTick)
				break;

			accum_AnmTick += interpolateTick;
			temp_rotkey.AnmTick=accum_AnmTick;
			float t=GetInterpolateValue(prev_rotkey.AnmTick,curr_rotkey.AnmTick,accum_AnmTick);						
			D3DXQuaternionSlerp(&temp_rotkey.RotationAccum,&prev_rotkey.RotationAccum,&curr_rotkey.RotationAccum,t);

			iANMKEY=accum_AnmTick/interpolateTick;
			arrayANMKEY[iANMKEY]->RotationDelta =temp_rotkey.RotationAccum;
			arrayANMKEY[iANMKEY]->AnmTick = temp_rotkey.AnmTick;
		}
		iANMKEY=curr_rotkey.AnmTick/interpolateTick;
		arrayANMKEY[iANMKEY]->RotationDelta =curr_rotkey.RotationAccum;
		arrayANMKEY[iANMKEY]->AnmTick = curr_rotkey.AnmTick;
		prev_rotkey = curr_rotkey;
	}

	//6. POS�����Ͽ� ANMKEY�� �ֱ�
	POSKEY prev_poskey,temp_poskey,curr_poskey;	
	memset(&prev_poskey,0,sizeof(prev_poskey));
	memset(&curr_poskey,0,sizeof(curr_poskey));
	memset(&temp_poskey,0,sizeof(temp_poskey));

	for (UINT i=0 ; i<arrayPOSKEY.size() ; i++)
	{		
		curr_poskey.TranslationAccum = arrayPOSKEY[i]->TranslationAccum;
		curr_poskey.AnmTick = arrayPOSKEY[i]->AnmTick;
		accum_AnmTick=prev_poskey.AnmTick;
		while (accum_AnmTick < curr_poskey.AnmTick)
		{						
			if ( (accum_AnmTick+interpolateTick) > curr_poskey.AnmTick)
				break;

			accum_AnmTick += interpolateTick;
			temp_poskey.AnmTick=accum_AnmTick;
			float s=GetInterpolateValue(prev_poskey.AnmTick,curr_poskey.AnmTick,accum_AnmTick);
			D3DXVec3Lerp(&temp_poskey.TranslationAccum,&prev_poskey.TranslationAccum,&curr_poskey.TranslationAccum,s);

			iANMKEY = accum_AnmTick/interpolateTick;
			arrayANMKEY[iANMKEY]->TranslationDelta = temp_poskey.TranslationAccum;
			arrayANMKEY[iANMKEY]->AnmTick = temp_poskey.AnmTick;
		}				

		iANMKEY = curr_poskey.AnmTick/interpolateTick;
		arrayANMKEY[iANMKEY]->TranslationDelta = curr_poskey.TranslationAccum;
		arrayANMKEY[iANMKEY]->AnmTick = curr_poskey.AnmTick;
		prev_poskey=curr_poskey;
	}

	//7. ���������� ����� ANMYKEY�� TRANSFORMANM���� �ٲٱ�
	vector<ANMKEY*>::iterator it=arrayANMKEY.begin();
	for ( ; it!=arrayANMKEY.end() ; it++)
	{
		ANMKEY* p=*it;		

		// SCLTM
		if (D3DXVec3Length(&p->ScaleDelta)!=0.0f)
		{	
			//D3DXMatrixScaling(&SCLTM,LocalTM_anmkey.ScaleDelta.x,LocalTM_anmkey.ScaleDelta.y,LocalTM_anmkey.ScaleDelta.z);
		}

		// ROTTM
		if ( (p->RotationDelta.x!=0.0f)||(p->RotationDelta.y!=0.0f)||(p->RotationDelta.z!=0.0f)||(p->RotationDelta.w!=0.0f))
		{
			D3DXMatrixRotationQuaternion(&ROTTM,&p->RotationDelta);						
		}

		// POSTM
		if (D3DXVec3Length(&p->TranslationDelta)!=0.0f)
		{
			D3DXMatrixTranslation(&POSTM,p->TranslationDelta.x,p->TranslationDelta.y,p->TranslationDelta.z);			
		}	

		// ���� ������ ����
		TRANSFORMANM* pNewItem=new TRANSFORMANM;		
		pNewItem->TM = SCLTM * ROTTM * POSTM;		
		pNewItem->anmTime = p->AnmTick / m_SceneTime.EX_TICKSPERMS;
		refArrayTRANSFORMANM.push_back(pNewItem);
	}	

	for_each(arrayANMKEY.begin(),arrayANMKEY.end(),FuncDeleteType<ANMKEY*>);
}

float cASEParser::GetInterpolateValue( int start_time,int end_time,int inter_time )
{
	float ret;
	int delta_time,offset_time;
	delta_time = end_time - start_time;
	offset_time = inter_time - start_time;

	if (delta_time==0)
	{
		return 0.0f;
	}

	ret=(float)offset_time / (float)delta_time;	
	return ret;
}