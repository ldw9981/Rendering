#include "stdafx.h"
#include "Foundation/Trace.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "Lexer.h"
#include "Scene/MeshNode.h"
#include "Scene/CameraNode.h"
#include "Scene/SkinnedMeshNode.h"
#include "Resource/ResourceMng.h"
#include "Graphics/RscTransformAnm.h"
#include "Foundation/Define.h"
#include "Framework/EnvironmentVariable.h"
#include "Scene/Entity.h"

cASEParser::cASEParser()
{
	m_CNTOBJECT=0;	
	m_repeat = 0;	
	m_tempAxisMin=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_tempAxisMax=D3DXVECTOR3(0.0f,0.0f,0.0f);	
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


std::string cASEParser::GetString()
{	
	std::string temp;
	// string
	m_Token=GetToken(m_TokenString);
	if (m_Token!=TOKEND_STRING)
	{
		ASSERT(0&&_T("m_Token!=TOKEND_STRING"));	
	}	
	temp=m_TokenString;
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

BOOL cASEParser::Load( const char* strFileName ,Entity* pOutput)
{	
	m_repeat++;
	BOOL bResult;
	
	char filename[256];
	_splitpath_s(strFileName,NULL,0,NULL,0,filename,256,NULL,0);
	m_SceneTime.FILENAME = filename;

	m_pSceneRoot = pOutput;

	if (!Open(strFileName))
	{
		return FALSE;
	}

	m_Token =  GetToken(m_TokenString);
	if(m_Token == TOKENR_3DSMAX_ASCIIEXPORT)
	{
		// - version 정보
		//m_version	 = Parser.GetFloat();
		SkipBlock();
	}
	else
	{
		TRACE("TOKENR_3DSMAX_ASCIIEXPORT 얻기실패\n");
		return FALSE;
	}

	// - Comment는 여러 줄이 될 수가 있기 때문에...
	while(m_Token = GetToken(m_TokenString), m_Token == TOKENR_COMMENT)
	{
		// - Comment는 그냥 무시한다.
		m_Token = GetToken(m_TokenString);
	}

	// 2) Scene Info
	if(m_Token == TOKENR_SCENE)
	{
		Parsing_Scene();		
	}
	// 3) Material List
	//   - Material List는 무조건 있긴 하다.
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
						
				TRACE3("정의되지 않은 Command입니다. ( %s ,token= %d , tokenstring= %s )\n", __FUNCTION__,m_Token,m_TokenString );						
			}		
			ASSERT(bResult==TRUE);
			break;
		};
		
		ASSERT(bResult==TRUE);
	}
	
	m_vecMaterial.clear();

	cSphere temp;
	CalculateSphere(m_tempAxisMin,m_tempAxisMax,temp);
	m_pSceneRoot->GetBoundingSphere() =  temp;
	m_pSceneRoot->SetNodeName(m_SceneTime.FILENAME.c_str());

	// *************************************************************
	// 
	// 완전 끝!!!
	// 
	// *************************************************************
	TRACE1(_T("cASEImport Done count%d\n"),m_CNTOBJECT);
	return	TRUE;
}

BOOL cASEParser::Parsing_GeoObject()
{	
	//인스턴스내에 삽입될정보
	bool bSkinned=false;
	bool bMultiSub=false;
	UINT totalVertices=0,totalFaces=0,totalBoneRef=0;
	UINT totalBaseTVertices=0,totalExtraTVertices=0;
	UINT nMaterialRef=0;
	
	std::vector<BONEREFINFO>					vecBoneRef;		
	std::map<SUBMATINDEX,WORD>				mapIndexCount;	

	//cRscVertexBuffer에 복사할 내용
	std::vector<NORMALVERTEX>				vecNormalVertexForBuffer; 
	std::vector<BLENDVERTEX>				vecBlendVertexForBuffer;
	//cRscIndexBuffer에 복사할 내용
	std::vector<TRIANGLE_SUBMATERIAL>		vecIndexForBuffer;
		
	// 버텍스 가공을 위한 일시적인 정보
	std::vector<TEXCOORD>					vecTempBaseTVertexOut;
	std::vector<VNORMAL>					vecTempVertexNormal;

	std::vector<TEXCOORD>					vecTempBaseTVertex;
	std::vector<TRIANGLE>					vecTempBaseTFaceIndex;
	
	std::vector<TEXCOORD>					vecTempExtraTVertex;
	std::vector<TRIANGLE>					vecTempExtraTFaceIndex;	

	// 정점으로 Sphere를 만들기위한 임시 정보
	
	cRscTransformAnm* pRscTransformAnm = NULL;

	SCENENODEINFO stInfo;	

	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			stInfo.strNodeName = GetString();		
			break;
		case TOKENR_NODE_PARENT:		
			stInfo.strParentName = GetString();							
			stInfo.pParent = m_pSceneRoot->FindNode(stInfo.strParentName);
			break;
		case TOKENR_NODE_TM:			
			stInfo.tmNode = GetNodeTM();
			D3DXMatrixInverse(&stInfo.tmInvNode,NULL,&stInfo.tmNode);
			stInfo.tmWorld = stInfo.tmNode;				

			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX tmInvParentWorld;
				D3DXMatrixInverse(&tmInvParentWorld,NULL,&stInfo.pParent->GetWorldTM());
				stInfo.tmLocal = stInfo.tmWorld * tmInvParentWorld ;
			}			
			break;

		case TOKENR_TM_ANIMATION:
			{					
				pRscTransformAnm = GetRscTransformAnm(m_SceneTime.FILENAME.c_str(),stInfo.strNodeName.c_str(),stInfo.tmLocal);					
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
								ASSERT(m_Token!=TOKEND_BLOCK_START);
								switch(m_Token)
								{
								case TOKENR_MESH_VERTEX:
									if (totalBoneRef==0)
									{
										NORMALVERTEX Item;
										memset(&Item,0,sizeof(Item));
										int index=GetInt();
										GetVector3(&Item.vertex);	

										//Bounding Sphere를 위한 최대 최소 얻기
										m_tempAxisMin.x= min(Item.vertex.x,m_tempAxisMin.x);
										m_tempAxisMin.y= min(Item.vertex.y,m_tempAxisMin.y);
										m_tempAxisMin.z= min(Item.vertex.z,m_tempAxisMin.z);

										m_tempAxisMax.x= max(Item.vertex.x,m_tempAxisMax.x);
										m_tempAxisMax.y= max(Item.vertex.y,m_tempAxisMax.y);
										m_tempAxisMax.z= max(Item.vertex.z,m_tempAxisMax.z);		

										// 원점중심 로컬좌표로 이동
										D3DXVec3TransformCoord(&Item.vertex,&Item.vertex,&stInfo.tmInvNode);
										vecNormalVertexForBuffer.push_back(Item);
									}
									else
									{
										BLENDVERTEX Item;
										memset(&Item,0,sizeof(Item));
										int index=GetInt();
										GetVector3(&Item.vertex);			

										//Bounding Sphere를 위한 최대 최소 얻기
										m_tempAxisMin.x= min(Item.vertex.x,m_tempAxisMin.x);
										m_tempAxisMin.y= min(Item.vertex.y,m_tempAxisMin.y);
										m_tempAxisMin.z= min(Item.vertex.z,m_tempAxisMin.z);

										m_tempAxisMax.x= max(Item.vertex.x,m_tempAxisMax.x);
										m_tempAxisMax.y= max(Item.vertex.y,m_tempAxisMax.y);
										m_tempAxisMax.z= max(Item.vertex.z,m_tempAxisMax.z);

										// 원점중심 로컬좌표로 이동
										D3DXVec3TransformCoord(&Item.vertex,&Item.vertex,&stInfo.tmInvNode);
										vecBlendVertexForBuffer.push_back(Item);	
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
								ASSERT(m_Token!=TOKEND_BLOCK_START);
								switch(m_Token)
								{
								case TOKENR_MESH_FACE:						
									int iFace,iMat;
									TRIANGLE tFaceIndex;
									iFace=GetInt();			// FaceIndex

									m_Token=GetToken(m_TokenString);		// A:
									tFaceIndex.index[0]=GetInt();			// 0

									m_Token=GetToken(m_TokenString);		// B:
									tFaceIndex.index[2]=GetInt();			// 2

									m_Token=GetToken(m_TokenString);		// C:
									tFaceIndex.index[1]=GetInt();			// 3						

									FindToken(TOKENR_MESH_MTLID);
									iMat=GetInt();
		
									// 서브매트리얼 ID를 키로 ID마다 사용되는 FACEINDEX수를 카운트한다.
									mapIndexCount[iMat]++;

									TRIANGLE_SUBMATERIAL temp;		
									temp.triangle = tFaceIndex;						
									temp.subMaterialIndex = iMat;									
									vecIndexForBuffer.push_back(temp);
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
						{
							totalBaseTVertices=GetInt();
							vecTempBaseTVertex.reserve(totalBaseTVertices);
						}
						break;
					case TOKENR_MESH_TVERTLIST:
						{				
							if(!GetTextureVertexList(vecTempBaseTVertex))
								return false;
						}	
						break;
					case TOKENR_MESH_NUMTVFACES:
						{
							GetInt();
						}
						break;
					case TOKENR_MESH_TFACELIST:
						{				
							if(!GetTextureFaceList(vecTempBaseTFaceIndex))
								return false;
						}	
						break;
					case TOKENR_MESH_MAPPINGCHANNEL:
						{
							int n = GetInt();	// extra channel index
							if (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_START)
								return FALSE;

							while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
							{					
								switch(m_Token)
								{
								case TOKENR_MESH_NUMTVERTEX:	
									{
										totalExtraTVertices=GetInt();
										vecTempExtraTVertex.reserve(totalExtraTVertices);
									}
									break;
								case TOKENR_MESH_TVERTLIST:
									{				
										if(!GetTextureVertexList(vecTempExtraTVertex))
											return false;
									}	
									break;
								case TOKENR_MESH_NUMTVFACES:
									{
										GetInt();
									}
									break;
								case TOKENR_MESH_TFACELIST:
									{				
										if(!GetTextureFaceList(vecTempExtraTFaceIndex))
											return false;
									}	
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
									vecTempVertexNormal.push_back(temp);					
								}
							}
							// }
							if(GetToken(m_TokenString) != TOKEND_BLOCK_END)
								return FALSE;				
						}
						break;	
					case TOKENR_MESH_NUMBONE:
						totalBoneRef=GetInt();
						bSkinned=true;
						break;
					case TOKENR_MESH_NUMSKINWEIGHT:
						GetInt();
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
								NewItem.strNodeName=GetString();
								NewItem.pRefBoneMesh=NULL;
								vecBoneRef.push_back(NewItem);

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

							for (UINT iVertex=0;iVertex<vecBlendVertexForBuffer.size();iVertex++)
							{												

								if(GetToken(m_TokenString) != TOKENR_MESH_WEIGHT)	
									return FALSE;
								
								GetInt();

								if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)	
									return FALSE;

								std::vector<BONEWEIGHT>	  vecBoneWeight;
								while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
								{
									ASSERT(m_Token!=TOKEND_BLOCK_START);
									switch(m_Token)
									{
									case TOKENR_BONE_BLENGING_WEIGHT:
										int BoneIndex=GetInt();
										float BoneWeight=GetFloat();

										BONEWEIGHT temp;
										temp.bone_index=BoneIndex;
										temp.bone_weight=BoneWeight;
										vecBoneWeight.push_back(temp);
										break;					
									}					
								}

								// 가중치가 큰 기준으로 정렬
								sort(vecBoneWeight.begin(),vecBoneWeight.end(),BONEWEIGHT::GreatBoneWeight);
								BYTE bindex[4]= { 0,0,0,0 };	
								float bweight[4]= { 0.0f,0.0f,0.0f,0.0f };	
								for (int iTop=0; (iTop<4)&&(iTop<(int)vecBoneWeight.size());iTop++)
								{
									bindex[iTop]=vecBoneWeight[iTop].bone_index;
									bweight[iTop]=vecBoneWeight[iTop].bone_weight;
								}												

								vecBlendVertexForBuffer[iVertex].SetIndex(bindex);
								vecBlendVertexForBuffer[iVertex].SetWeight(bweight); 

							}
							if(GetToken(m_TokenString) != TOKEND_BLOCK_END)		return FALSE;
						}
						break;			
					}		
				}	
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
				nMaterialRef=GetInt();
				assert(nMaterialRef < m_vecMaterial.size());				
			}
			break;	
		}
	}	

	// 부모가 없으면 루트를 부모로 한다.
	if (stInfo.pParent==NULL)
	{
		assert(m_pSceneRoot!=NULL);
		stInfo.pParent=m_pSceneRoot;
	}

	
		
	// 이제 버텍스 가공 버텍스,노말 합치기
	if (!bSkinned) MergeNormalListIntoVertexList(vecNormalVertexForBuffer,vecIndexForBuffer,vecTempVertexNormal);
	else MergeNormalListIntoVertexList(vecBlendVertexForBuffer,vecIndexForBuffer,vecTempVertexNormal);	

	// 텍스쳐 좌표줄이면서 텍스쳐좌표 인덱스 수정하기, 버텍스랑 합치면서 FaceIndex수정하기 	
	if (!bSkinned) MergeTexCoordListIntoVertexList(true,vecNormalVertexForBuffer,vecIndexForBuffer,vecTempBaseTVertex,vecTempBaseTFaceIndex);
	else MergeTexCoordListIntoVertexList(true,vecBlendVertexForBuffer,vecIndexForBuffer,vecTempBaseTVertex,vecTempBaseTFaceIndex);

	if (!bSkinned) MergeTexCoordListIntoVertexList(false,vecNormalVertexForBuffer,vecIndexForBuffer,vecTempExtraTVertex,vecTempExtraTFaceIndex);
	else MergeTexCoordListIntoVertexList(false,vecBlendVertexForBuffer,vecIndexForBuffer,vecTempExtraTVertex,vecTempExtraTFaceIndex);

	
	// 서브매트리얼 ID별로 FACEINDEX정렬
	sort(vecIndexForBuffer.begin(),vecIndexForBuffer.end(),TRIANGLE_SUBMATERIAL::LessFaceIndex);	

	// 리소스 버텍스 버퍼   생성 -> 데이터복사 -> 메쉬셋팅
	cRscVertexBuffer* pNewRscVertexBuffer=NULL;
	if(!bSkinned) pNewRscVertexBuffer = CreateRscVertexBuffer(stInfo.strNodeName.c_str(),vecNormalVertexForBuffer);
	else pNewRscVertexBuffer = CreateRscVertexBuffer(stInfo.strNodeName.c_str(),vecBlendVertexForBuffer);		

	// 리소스 인덱스 버퍼 생성-> 데이터복사 -> 메쉬 세팅
	cRscIndexBuffer* pNewRscIndexBuffer = CreateRscIndexBuffer(stInfo.strNodeName.c_str(),vecIndexForBuffer);
	cSceneNode* pNewSceneNode=NULL;
	if (pNewRscIndexBuffer==NULL || pNewRscVertexBuffer==NULL)
	{
		pNewSceneNode = CreateSceneNode(stInfo);		
	}
	else
	{
		if (!bSkinned)	pNewSceneNode = CreateMeshNode(stInfo,pNewRscVertexBuffer,pNewRscIndexBuffer,mapIndexCount,nMaterialRef);		
		else pNewSceneNode = CreateSkinnedMeshNode(stInfo,pNewRscVertexBuffer,pNewRscIndexBuffer,mapIndexCount,nMaterialRef,vecBoneRef );			
	}	
	pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);


	return TRUE;
}



BOOL cASEParser::Parsing_MaterialList()
{
	BOOL bRet=TRUE;
	std::string strMaterialClass;
	int nMaterialCount,nMaterialIndex;
	if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	return FALSE;	
	if (GetToken(m_TokenString) != TOKENR_MATERIAL_COUNT)	return FALSE;	// *MATERIAL_COUNT
	nMaterialCount=GetInt();	
	
	for (int i=0;i<nMaterialCount;i++)
	{
		Material material;		
		std::vector<Material> vecSubMatrial;
		m_vecMaterial.push_back(vecSubMatrial);

		if(GetToken(m_TokenString) != TOKENR_MATERIAL)		// *MATERIAL	
			return FALSE;			
		nMaterialIndex=GetInt();									// index
		if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
			return FALSE;		

		while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
		{
			switch(m_Token)
			{
			case TOKENR_MATERIAL_CLASS:
				strMaterialClass = GetString();
				break;
			case TOKENR_MATERIAL_AMBIENT:
				material.Ambient.r=GetFloat();
				material.Ambient.g=GetFloat();
				material.Ambient.b=GetFloat();
				break;
			case TOKENR_MATERIAL_DIFFUSE:
				material.Diffuse.r=GetFloat();
				material.Diffuse.g=GetFloat();
				material.Diffuse.b=GetFloat();
				break;
			case TOKENR_MATERIAL_SPECULAR:
				material.Specular.r=GetFloat();
				material.Specular.g=GetFloat();
				material.Specular.b=GetFloat();			
				break;
			case TOKENR_MATERIAL_SHINE:				
				material.Multiply =GetFloat();
				break;
			case TOKENR_MATERIAL_SHINESTRENGTH:				
				material.Power=GetFloat();
				break;
			case TOKENR_MATERIAL_TRANSPARENCY:				
				material.Transparency=GetFloat();
				break;
			case TOKENR_MAP_NAME:
			case TOKENR_MAP_CLASS:		
			case TOKENR_MAP_SUBNO:
			case TOKENR_MAP_AMOUNT:			
			case TOKENR_MAP_SPECULAR:
			case TOKENR_MAP_SHINE:
			case TOKENR_MAP_GENERIC:
			case TOKENR_MAP_TYPE:
			case TOKENR_MAP_OPACITY:
			case TOKENR_MAP_REFLECT:
				{
					if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
						return FALSE;						
					while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
					{
						ASSERT(m_Token!=TOKEND_BLOCK_START);
						switch(m_Token)
						{
						case TOKENR_BITMAP:
						
							/*

							string strFileName=GetString().c_str();							
							string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							string strFullPath = strDataPath;
							strFullPath += strFileName;

							cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_REFLECT: %s 파일이없습니다.\n",strFullPath.c_str());
							
							SubMatrial.SetMapRefract(pRscTexture);
							*/
							break;
						case TOKENR_MAP_GENERIC:
							if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
								return FALSE;						
							while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
							{
								ASSERT(m_Token!=TOKEND_BLOCK_START);
							}
							break;
						}		
					}
				}
				break;
			case TOKENR_MAP_REFRACT:
				{
					if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
						return FALSE;						
					while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
					{
						ASSERT(m_Token!=TOKEND_BLOCK_START);
						switch(m_Token)
						{
						case TOKENR_BITMAP:

							std::string strFileName=GetString().c_str();							
							/*
							std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							std::string strFullPath = strDataPath;
							strFullPath += strFileName;

							cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_REFRACT: %s 파일이없습니다.\n",strFullPath.c_str());
							material.SetMapRefract(pRscTexture);
							*/
							break;
						}		
					}
				}
				break;
			case TOKENR_MAP_BUMP:
				{
					if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
						return FALSE;						
					while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
					{
						ASSERT(m_Token!=TOKEND_BLOCK_START);
						switch(m_Token)
						{
						case TOKENR_BITMAP:

							std::string strFileName=GetString().c_str();							
							std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							std::string strFullPath = strDataPath;
							strFullPath += strFileName;					

							cRscTexture* pRscTexture= cResourceMng::m_pResourceMng->CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_BUMP: %s 파일이없습니다.\n",strFullPath.c_str());
							material.SetMapNormal(pRscTexture);
							break;
						}		
					}
				}
				break;
			case TOKENR_MAP_SELFILLUM:
			case TOKENR_MAP_AMBIENT:
			case TOKENR_MAP_SHINESTRENGTH:
			case TOKENR_MAP_FILTERCOLOR:		
				FindToken(TOKEND_BLOCK_START);
				FindToken(TOKEND_BLOCK_END);
				break;
			case TOKENR_MAP_DIFFUSE:
				{
					if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
						return FALSE;						
					while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
					{
						ASSERT(m_Token!=TOKEND_BLOCK_START);
						switch(m_Token)
						{
						case TOKENR_BITMAP:
					
							std::string strFileName=GetString().c_str();							
							std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							std::string strFullPath = strDataPath;
							strFullPath += strFileName;

							cRscTexture* pRscTexture= cResourceMng::m_pResourceMng->CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_DIFFUSE: %s 파일이없습니다.\n",strFullPath.c_str());
							material.SetMapDiffuse(pRscTexture);
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
						Material subMaterial;				
						
						FindToken(TOKENR_SUBMATERIAL);	// *SUBMATERIAL
						GetInt();						// index
						if(!GetSubMaterial(subMaterial))
							return false;

						if (strMaterialClass == "Shell Material")
						{							
							if(iNUMSUBMTLS == 0 )
								material.SetMapDiffuse(subMaterial.GetMapDiffuse());
							if(iNUMSUBMTLS == 1 )
								material.SetMapLight(subMaterial.GetMapDiffuse());
						}
						else
						{
							m_vecMaterial[nMaterialIndex].push_back(subMaterial);			
						}															
					}

				}//case TOKENR_NUMSUBMTLS:
				break;		
			}//switch			

		}//while (m_Token=GetLexer()->GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
		
		if (m_vecMaterial[nMaterialIndex].empty())
		{
			m_vecMaterial[nMaterialIndex].push_back(material);					
		}
	}//for (int i=0;i<nNUMMaterial;i++)	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_END) 
		return FALSE;	// }	

	
	return bRet;
}


BOOL cASEParser::Parsing_HelperObject()
{	
	SCENENODEINFO stInfo;
	cRscTransformAnm* pRscTransformAnm = NULL;

	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			stInfo.strNodeName = GetString();		
			break;
		case TOKENR_NODE_PARENT:		
			stInfo.strParentName = GetString();							
			stInfo.pParent = m_pSceneRoot->FindNode(stInfo.strParentName);
			break;
		case TOKENR_NODE_TM:			
			stInfo.tmNode = GetNodeTM();
			D3DXMatrixInverse(&stInfo.tmInvNode,NULL,&stInfo.tmNode);
			stInfo.tmWorld = stInfo.tmNode;				

			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX tmInvParentWorld;
				D3DXMatrixInverse(&tmInvParentWorld,NULL,&stInfo.pParent->GetWorldTM());
				stInfo.tmLocal = stInfo.tmWorld * tmInvParentWorld ;
			}			
			break;

		case TOKENR_TM_ANIMATION:
			{					
				pRscTransformAnm = GetRscTransformAnm(m_SceneTime.FILENAME.c_str(),stInfo.strNodeName.c_str(),stInfo.tmLocal);					
			}
			break;
		case	TOKENR_BOUNDINGBOX_MIN:
			GetVector3(NULL);
			break;
		case	TOKENR_BOUNDINGBOX_MAX:
			GetVector3(NULL);
			break;		
		}		
	}

	cSceneNode* pNewSceneNode = CreateSceneNode(stInfo);
	pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
	return TRUE;
}

BOOL cASEParser::Parsing_ShapeObject()
{
	SCENENODEINFO stInfo;	
	cRscTransformAnm* pRscTransformAnm = NULL;

	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			stInfo.strNodeName = GetString();		
			break;
		case TOKENR_NODE_PARENT:		
			stInfo.strParentName = GetString();							
			stInfo.pParent = m_pSceneRoot->FindNode(stInfo.strParentName);
			break;
		case TOKENR_NODE_TM:			
			stInfo.tmNode = GetNodeTM();
			D3DXMatrixInverse(&stInfo.tmInvNode,NULL,&stInfo.tmNode);
			stInfo.tmWorld = stInfo.tmNode;				

			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX tmInvParentWorld;
				D3DXMatrixInverse(&tmInvParentWorld,NULL,&stInfo.pParent->GetWorldTM());
				stInfo.tmLocal = stInfo.tmWorld * tmInvParentWorld ;
			}			
			break;
		case TOKENR_TM_ANIMATION:
			{					
				pRscTransformAnm = GetRscTransformAnm(m_SceneTime.FILENAME.c_str(),stInfo.strNodeName.c_str(),stInfo.tmLocal);					
			}
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
		}
	}

	cSceneNode*	pNewSceneNode=new cSceneNode;
	m_pLastObject = pNewSceneNode;

	//공통적인 데이터
	SetNodeInfo(pNewSceneNode,stInfo);
	if (stInfo.pParent!=NULL)
	{
		stInfo.pParent->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(stInfo.pParent);
	}
	else
	{		
		m_pSceneRoot->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(m_pSceneRoot);
	}	
	pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
	return TRUE;
}

BOOL cASEParser::Parsing_LightObject()
{	
	SCENENODEINFO stInfo;	
	cRscTransformAnm* pRscTransformAnm = NULL;

	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			stInfo.strNodeName = GetString();		
			break;
		case TOKENR_NODE_PARENT:		
			stInfo.strParentName = GetString();							
			stInfo.pParent = m_pSceneRoot->FindNode(stInfo.strParentName);
			break;
		case TOKENR_NODE_TM:			
			stInfo.tmNode = GetNodeTM();
			D3DXMatrixInverse(&stInfo.tmInvNode,NULL,&stInfo.tmNode);
			stInfo.tmWorld = stInfo.tmNode;				

			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX tmInvParentWorld;
				D3DXMatrixInverse(&tmInvParentWorld,NULL,&stInfo.pParent->GetWorldTM());
				stInfo.tmLocal = stInfo.tmWorld * tmInvParentWorld ;
			}			
			break;
		case TOKENR_TM_ANIMATION:
			{					
				pRscTransformAnm = GetRscTransformAnm(m_SceneTime.FILENAME.c_str(),stInfo.strNodeName.c_str(),stInfo.tmLocal);					
			}
			break;		
		case TOKENR_LIGHT_TYPE:
			GetIdentifier(NULL);
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


		}		
	}	

	cSceneNode* pNewSceneNode=new cSceneNode;	
	m_pLastObject = pNewSceneNode;
	
	//공통적인 데이터
	SetNodeInfo(pNewSceneNode,stInfo);
	if (stInfo.pParent!=NULL)
	{
		stInfo.pParent->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(stInfo.pParent);
	}
	else
	{		
		m_pSceneRoot->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(m_pSceneRoot);
	}	
	pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);
	return TRUE;
}

BOOL cASEParser::Parsing_CameraObject()
{
	SCENENODEINFO stInfo;	
	cRscTransformAnm* pRscTransformAnm = NULL;

	// {
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;
	
	BOOL bLoadCameraNodeTM=FALSE;
	BOOL bLoadCameraAnmTM=FALSE;
	float fFov=0.0f;
	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		switch(m_Token)
		{
		case TOKENR_NODE_NAME:
			stInfo.strNodeName = GetString();		
			break;
		case TOKENR_NODE_PARENT:		
			stInfo.strParentName = GetString();							
			stInfo.pParent = m_pSceneRoot->FindNode(stInfo.strParentName);
			break;
		case TOKENR_NODE_TM:			
			if (!bLoadCameraNodeTM)	
			{
				D3DXMATRIX nodeTM,NodeRotation;
				nodeTM=GetNodeTM();		// Camera Node TM

				//축의변경
				D3DXMatrixRotationAxis(&NodeRotation,&D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXToRadian(90));
				nodeTM = NodeRotation * nodeTM;				

				stInfo.tmNode = nodeTM;
				D3DXMatrixInverse(&stInfo.tmInvNode,NULL,&stInfo.tmNode);
				stInfo.tmWorld = stInfo.tmNode;				

				if (stInfo.pParent==NULL)
				{					
					stInfo.tmLocal = stInfo.tmNode;
				}
				else
				{					
					D3DXMATRIX tmInvParentWorld;
					D3DXMatrixInverse(&tmInvParentWorld,NULL,&stInfo.pParent->GetWorldTM());
					stInfo.tmLocal = stInfo.tmWorld * tmInvParentWorld ;
				}			

				bLoadCameraNodeTM=TRUE;
			}		
			else
			{
				SkipBlock();	// TargetTM은 사용하지않으므로 Pass
			}
			
			break;
		case TOKENR_TM_ANIMATION:		
			if (bLoadCameraAnmTM==FALSE)
			{				
				pRscTransformAnm = GetRscTransformAnm(m_SceneTime.FILENAME.c_str(),stInfo.strNodeName.c_str(),stInfo.tmLocal);					
				bLoadCameraAnmTM=TRUE;												
			}			
			else
			{
				SkipBlock();// TargetTM은 사용하지않으므로 Pass
			}
			break;		
		case TOKENR_CAMERA_TYPE:
			GetToken(m_TokenString);
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
						fFov=GetFloat();
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
		}
	}	

	cSceneNode* pNewSceneNode=new cSceneNode;
	m_pLastObject = pNewSceneNode;

	//공통적인 데이터
	SetNodeInfo(pNewSceneNode,stInfo);
	if (stInfo.pParent!=NULL)
	{
		stInfo.pParent->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(stInfo.pParent);
	}
	else
	{		
		m_pSceneRoot->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(m_pSceneRoot);
	}	
	//pNewSceneNode->SetFOV(fFov);
	pNewSceneNode->SetRscTransformAnm(pRscTransformAnm);

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
				TRACE3("정의되지 않은 Command입니다. ( %s ,token= %d , tokenstring= %s )\n",__FUNCTION__,m_Token,m_TokenString );						
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
			//m_SceneTime.FILENAME=GetString(); 실제파일이름과 맥스파일 안의 이름이 다를수있으므로 사용하지않음
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
	// 1프레임당 ms구하기
	m_SceneTime.EX_MSPERFRAME= 1000/m_SceneTime.FRAMESPEED;
	// 1ms당 틱수 구하기 
	m_SceneTime.EX_TICKSPERMS= m_SceneTime.TICKSPERFRAME/m_SceneTime.EX_MSPERFRAME;
	// 마지막 프레임 ms구하기
	m_SceneTime.LASTFRAMEMS= m_SceneTime.LASTFRAME*m_SceneTime.EX_MSPERFRAME;
	return TRUE;
}


// 노말리스트를 버텍스 리스트에 합친다. 
// arrVertex [in,out] , arrVNormal [in] ,  arrFaceIndex[in,out]
template <typename T>
void cASEParser::MergeNormalListIntoVertexList(std::vector<T>& arrVertex,
								   std::vector<TRIANGLE_SUBMATERIAL>& arrFaceIndex,
								   const std::vector<VNORMAL>& arrVNormal)
{
	// arrVertex를 참조하는 인덱스기준으로 FACE리스트를 만든다.
	std::vector<std::list<WORD>>						arrVRefFaceList;	
	// arrVertex를 참조하는 기준으로 뒤쪽 WORD는 이미존재하는 노말을갖는 버텍스의 인덱스
	std::vector<std::list<std::pair<D3DXVECTOR3,WORD>>>    arrVRefNormalInsert;			

	if (arrVertex.empty())
		return;

	if (arrFaceIndex.empty())
		return;

	arrVRefFaceList.resize(arrVertex.size());
	arrVRefNormalInsert.resize(arrVertex.size());

	//1. arrVertex를 참조하는 인덱스기준으로 FACEINDEX리스트를 만든다.
	// 즉. 인덱스안의 리스트에 담긴 FACEINDEX들은 모두 arrVertex의 특정 인덱스좌표를 사용한다.
	int nFace=(int)arrFaceIndex.size();
	for (int iFace=0;iFace<nFace;iFace++ )
	{
		arrVRefFaceList[arrFaceIndex[iFace].triangle.index[0]].push_back(iFace);	
		arrVRefFaceList[arrFaceIndex[iFace].triangle.index[1]].push_back(iFace);
		arrVRefFaceList[arrFaceIndex[iFace].triangle.index[2]].push_back(iFace);
	}

	//2. arrFaceNormal 을 arrVertex에 넣으며 arrFace내의 버텍스인덱스를 수정한다.
	WORD nVNormal=(WORD)arrVNormal.size();
	for (WORD iVNormal=0;iVNormal<nVNormal;iVNormal++)
	{
		std::list<std::pair<D3DXVECTOR3,WORD>>* pListNormalnsert;


		WORD iRefVertex=arrVNormal[iVNormal].iRefVertex;
		pListNormalnsert=&arrVRefNormalInsert[iRefVertex];

		if(pListNormalnsert->empty())
		{	// arrVertex 인덱스의 노말들이 없으면 그냥 넣는다.			
			arrVertex[iRefVertex].normal = arrVNormal[iVNormal].normal;			
			arrVRefNormalInsert[iRefVertex].push_back(std::make_pair(arrVNormal[iVNormal].normal,iRefVertex));
		}
		else
		{					
			const D3DXVECTOR3* pNewNormal=&arrVNormal[iVNormal].normal;

			std::list<std::pair<D3DXVECTOR3,WORD>>::iterator it;
			it=pListNormalnsert->begin();
			for ( ; it != pListNormalnsert->end() ; ++it)
			{				
				D3DXVECTOR3* pExistNormal=&(*it).first;				

				if (D3DXVec3Dot(pExistNormal,pNewNormal) > 0.9999f )
				{	//노말이 리스트에 존재하면 인덱스를 수정한다.
					WORD iExist=(*it).second;
					WORD iRefFace=arrVNormal[iVNormal].iRefFace;

					if (arrFaceIndex[iRefFace].triangle.index[0] == iRefVertex )
						arrFaceIndex[iRefFace].triangle.index[0] = iExist;

					if (arrFaceIndex[iRefFace].triangle.index[1] == iRefVertex )
						arrFaceIndex[iRefFace].triangle.index[1] = iExist;

					if (arrFaceIndex[iRefFace].triangle.index[2]== iRefVertex )
						arrFaceIndex[iRefFace].triangle.index[2] = iExist;					

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
				arrVRefNormalInsert[iRefVertex].push_back(std::make_pair(*pNewNormal,iInsert));
				WORD iRefFace=arrVNormal[iVNormal].iRefFace;

				if (arrFaceIndex[iRefFace].triangle.index[0] == iRefVertex )
					arrFaceIndex[iRefFace].triangle.index[0] = iInsert;

				if (arrFaceIndex[iRefFace].triangle.index[1] == iRefVertex )
					arrFaceIndex[iRefFace].triangle.index[1] = iInsert;

				if (arrFaceIndex[iRefFace].triangle.index[2]== iRefVertex )
					arrFaceIndex[iRefFace].triangle.index[2] = iInsert;					
			}
		}
	}
}


void cASEParser::OptimizeTexCoordAndFace(std::vector<TEXCOORD>& arrTexCoordOut,
								const std::vector<TEXCOORD>& arrTexCoordIn,
								std::vector<TRIANGLE>& arrTFaceIndexInOut)
{
	std::map<float,std::map<float,WORD>>	mapUVINDEX;
	std::vector<std::list<int>>			arrTVIndexedFaceIndexList;	// 해당버텍스를 사용하는 페이스인덱스의 리스트를 버텍스인덱스로 인덱스화

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
		std::pair<std::map<float,std::map<float,WORD>>::iterator	,bool> retItInsertU;
		std::pair<std::map<float,WORD>::iterator				,bool> retItInsertV;

		std::map<float,WORD> mapVINDEX;
		std::map<float,WORD>* pmapVINDEX;

		// U좌표와 빈 mapVINDEX 삽입
		retItInsertU= mapUVINDEX.insert(std::make_pair(arrTexCoordIn[iTexCoordIn].u,mapVINDEX));
		pmapVINDEX=&(retItInsertU.first->second);		
		retItInsertV=pmapVINDEX->insert(std::make_pair(arrTexCoordIn[iTexCoordIn].v,0));

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
		std::list<int>::iterator it2=arrTVIndexedFaceIndexList[iTexCoordIn].begin();		
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
void cASEParser::MergeTexCoordListIntoVertexList(bool bBaseMapChannel,std::vector<T>& arrVertexInOut,
									 std::vector<TRIANGLE_SUBMATERIAL>& arrVFaceIndexInOut,
									 const std::vector<TEXCOORD>& arrBaseTexCoordIn,
									 const std::vector<TRIANGLE>& arrBaseTFaceIndexIn)
{	
	if (arrVertexInOut.empty())
		return;
	if (arrVertexInOut.empty())
		return;
	if (arrBaseTexCoordIn.empty())
		return;
	if (arrBaseTFaceIndexIn.empty())
		return;

	std::vector<std::list<std::pair<TEXCOORD,WORD>>> arrVIndexedTCList;
	//랜덤액세스를 위한 할당
	arrVIndexedTCList.resize(arrVertexInOut.size());

	int nFace=(int)arrVFaceIndexInOut.size();
	for (int iFace=0;iFace<nFace;iFace++)
	{
		TRIANGLE*		 pVFace=&arrVFaceIndexInOut[iFace].triangle;
		const TRIANGLE* pTFace=&arrBaseTFaceIndexIn[iFace];

		for (int i=0;i<3;i++)
		{
			int iRefVertex=pVFace->index[i];					// 버텍스 인덱스
			int iRefTexCoord=arrBaseTFaceIndexIn[iFace].index[i];	//텍스좌표 인덱스
			const TEXCOORD* pTexcoord=&arrBaseTexCoordIn[iRefTexCoord]; //텍스좌표 얻기

			if(arrVIndexedTCList[iRefVertex].empty())			// 버텍스에 텍스좌표 리스트들이 없으면
			{
				if (bBaseMapChannel) 
					arrVertexInOut[iRefVertex].uv0 = *pTexcoord;	// 버텍스리스트에 버테스의 텍스좌표 추가
				else
					arrVertexInOut[iRefVertex].uv1 = *pTexcoord;	// 버텍스리스트에 버테스의 텍스좌표 추가
				

				std::pair<TEXCOORD,WORD> temp;
				temp.first = *pTexcoord;
				temp.second = iRefVertex;
				arrVIndexedTCList[iRefVertex].push_back(temp); //리스트에 텍스좌표추가
			}
			else // 버텍스인덱스별 텍스좌표 리스트에 하나이상 들어있다.
			{				
				std::list<std::pair<TEXCOORD,WORD>>* pListTexcoordInsert=&arrVIndexedTCList[iRefVertex];
				std::list<std::pair<TEXCOORD,WORD>>::iterator it=pListTexcoordInsert->begin();
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

					if (bBaseMapChannel) 
						vertex.uv0 = *pTexcoord;	// 버텍스리스트에 버테스의 텍스좌표 추가
					else
						vertex.uv1 = *pTexcoord;	// 버텍스리스트에 버테스의 텍스좌표 추가

					arrVertexInOut.push_back(vertex);					

					//새로운 인덱스
					iRefVertex=(int)arrVertexInOut.size()-1;

					//페이스 인덱스 수정
					pVFace->index[i]=iRefVertex;					

					//리스트 생성,텍스추가
					std::list<std::pair<TEXCOORD,WORD>> tempList;
					arrVIndexedTCList.push_back(tempList);

					std::pair<TEXCOORD,WORD> tempValue;
					tempValue.first = *pTexcoord;
					tempValue.second = iRefVertex;

					arrVIndexedTCList[iRefVertex].push_back(tempValue);
				}
			}
		}			
	}
}

void cASEParser::CalculateSphere(D3DXVECTOR3& tempAxisMin,D3DXVECTOR3& tempAxisMax,cSphere& out )
{
	float tempRadius=0.0f;
	float MaxLengthSq=0.0f,LengthSq1=0.0f,LengthSq2=0.0f;
	LengthSq2 = D3DXVec3LengthSq(&tempAxisMin);
	LengthSq1 = D3DXVec3LengthSq(&tempAxisMax);
	MaxLengthSq=max(LengthSq1,LengthSq2);		
	out.Make(D3DXVECTOR3(0.0f,0.0f,0.0f),sqrt(MaxLengthSq));
}



cRscTransformAnm* cASEParser::GetRscTransformAnm(const char* rootName,const char* meshName, const D3DXMATRIX& localTM )
{
	cRscTransformAnm* pRscTransformAnm = cResourceMng::m_pResourceMng->CreateRscTransformAnm(rootName,meshName,"DEFAULT");

	// 리소스 가 이미 있으면 있는거 전달
	if( pRscTransformAnm->GetArrayANMKEY().empty() == false)
	{
		SkipBlock();
		return pRscTransformAnm;
	}

	ANMKEY localTM_anmkey;
	D3DXMatrixDecompose(
		&localTM_anmkey.ScaleAccum,
		&localTM_anmkey.RotationAccum,
		&localTM_anmkey.TranslationAccum,
		&localTM);	

	// time - AnmKey
	DWORD dwTimeKey=0;
	
	std::map<DWORD,ANMKEY> mapAnmKey;

	mapAnmKey[0].AnmTick=0;
	mapAnmKey[0].TranslationAccum=localTM_anmkey.TranslationAccum;
	mapAnmKey[0].RotationAccum=localTM_anmkey.RotationAccum;
	mapAnmKey[0].ScaleAccum=localTM_anmkey.ScaleAccum;


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
					ASSERT(m_Token!=TOKEND_BLOCK_START);
					if(m_Token!=TOKENR_CONTROL_POS_SAMPLE)
						return FALSE;										

					dwTimeKey = GetInt() / m_SceneTime.EX_TICKSPERMS;			

					D3DXVECTOR3 vecTranslationAccum;
					GetVector3(&vecTranslationAccum);

					mapAnmKey[dwTimeKey].AnmTick = dwTimeKey;
					mapAnmKey[dwTimeKey].TranslationAccum = vecTranslationAccum;
				}
			}
			break;
		case TOKENR_CONTROL_ROT_TRACK:
			{
				std::vector<std::pair<DWORD,D3DXQUATERNION>> arrayROTKEY;

				if(GetToken(m_TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while(m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{	
					ASSERT(m_Token!=TOKEND_BLOCK_START);
					if(m_Token!=TOKENR_CONTROL_ROT_SAMPLE)
						return FALSE;	

					float ang;
					D3DXVECTOR3 axis;
					dwTimeKey =  GetInt() / m_SceneTime.EX_TICKSPERMS;	
					GetVector3(&axis);

					std::pair<DWORD,D3DXQUATERNION> ItemDelta;
					ItemDelta.first = dwTimeKey;
					ang = GetFloat();					
					D3DXQuaternionRotationAxis(&ItemDelta.second,&axis,ang);
					arrayROTKEY.push_back(ItemDelta);
				}		

				// 회전 변화량 값을 누적 회전데이터로 바꾼다.
				D3DXQUATERNION curr_q, prev_q, accum_q;
				D3DXQuaternionIdentity(&curr_q);
				D3DXQuaternionIdentity(&prev_q);
				D3DXQuaternionIdentity(&accum_q);

				std::vector<std::pair<DWORD,D3DXQUATERNION>>::iterator rot_it=arrayROTKEY.begin();
				for ( ; rot_it!=arrayROTKEY.end() ; rot_it++)
				{
					std::pair<DWORD,D3DXQUATERNION>& Item = *rot_it;

					if(rot_it == arrayROTKEY.begin())
					{
						Item.second = localTM_anmkey.RotationAccum;			
					}
					curr_q = Item.second;				
					D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//쿼터니언 누적
					Item.second=accum_q;

					mapAnmKey[Item.first].AnmTick = Item.first;
					mapAnmKey[Item.first].RotationAccum = Item.second;

					prev_q=accum_q;
				}

			}
			break;
		case TOKENR_CONTROL_SCALE_TRACK:
			{				
				if( GetToken(m_TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while(m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{
					ASSERT(m_Token!=TOKEND_BLOCK_START);
				}
			}
			break;
		}
	} 

	pRscTransformAnm->SetTimeLength(dwTimeKey);
	std::vector<ANMKEY>& refArrAnmKey=pRscTransformAnm->GetArrayANMKEY();

	ANMKEY prevItem=localTM_anmkey;
	std::map<DWORD,ANMKEY>::iterator iter = mapAnmKey.begin();
	for ( ; iter != mapAnmKey.end() ;iter++ )
	{	
		ANMKEY& currItem = iter->second;
		// Scale
		if (D3DXVec3LengthSq(&currItem.ScaleAccum)==0.0f)
		{	
			currItem.ScaleAccum = prevItem.ScaleAccum;
		}

		// Rotate
		if ( (currItem.RotationAccum.x==0.0f)||(currItem.RotationAccum.y==0.0f)||(currItem.RotationAccum.z==0.0f)||(currItem.RotationAccum.w==0.0f))
		{
			currItem.RotationAccum = prevItem.RotationAccum;		
		}

		// POSTM
		if (D3DXVec3Length(&currItem.TranslationAccum)==0.0f)
		{
			currItem.TranslationAccum = prevItem.TranslationAccum;		
		}	

		refArrAnmKey.push_back(currItem);
		prevItem=currItem;
	}

	// 생성된 애니메이션 정보가 없으면 리소스해제후 NULL리턴
	if( refArrAnmKey.empty() )
	{
		pRscTransformAnm->Release();
		pRscTransformAnm=NULL;
	}			
	

	return pRscTransformAnm;
}



// 회전키정보의 누적변환
void cASEParser::ConvertAccQuaternion(std::vector<ROTKEY>& arrayROTKEY,const D3DXMATRIX& localTM)
{
	ANMKEY localTM_anmkey;
	D3DXMatrixDecompose(
		&localTM_anmkey.ScaleAccum,
		&localTM_anmkey.RotationAccum,
		&localTM_anmkey.TranslationAccum,
		&localTM);			

	
	D3DXQUATERNION curr_q, prev_q, accum_q;
	D3DXQuaternionIdentity(&curr_q);
	D3DXQuaternionIdentity(&prev_q);
	D3DXQuaternionIdentity(&accum_q);

	std::vector<ROTKEY>::iterator rot_it=arrayROTKEY.begin();
	for ( ; rot_it!=arrayROTKEY.end() ; rot_it++)
	{
		ROTKEY& Item = *rot_it;

		if(Item.AnmTick == 0)
		{
			Item.RotationAccum = localTM_anmkey.RotationAccum;			
		}
		curr_q = Item.RotationAccum;				
		D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//쿼터니언 누적
		Item.RotationAccum=accum_q;
		prev_q=accum_q;
	}
}

// 회전키정보의 누적변환
void cASEParser::ConvertAccQuaternionEX(std::vector<std::pair<DWORD,D3DXQUATERNION>>& inArrayROTKEY,const D3DXMATRIX& inLocalTM,std::map<DWORD,ANMKEY> outRefMapAnmKey)
{
	ANMKEY localTM_anmkey;
	D3DXMatrixDecompose(
		&localTM_anmkey.ScaleAccum,
		&localTM_anmkey.RotationAccum,
		&localTM_anmkey.TranslationAccum,
		&inLocalTM);			


	D3DXQUATERNION curr_q, prev_q, accum_q;
	D3DXQuaternionIdentity(&curr_q);
	D3DXQuaternionIdentity(&prev_q);
	D3DXQuaternionIdentity(&accum_q);

	std::vector<std::pair<DWORD,D3DXQUATERNION>>::iterator rot_it=inArrayROTKEY.begin();
	for ( ; rot_it!=inArrayROTKEY.end() ; rot_it++)
	{
		std::pair<DWORD,D3DXQUATERNION>& Item = *rot_it;

		if(rot_it == inArrayROTKEY.begin())
		{
			Item.second = localTM_anmkey.RotationAccum;			
		}
		curr_q = Item.second;				
		D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//쿼터니언 누적
		Item.second=accum_q;
		prev_q=accum_q;
	}
}


cMeshNode* 
cASEParser::CreateMeshNode(SCENENODEINFO& stInfo,
							cRscVertexBuffer* pVertexBuffer,
							cRscIndexBuffer* pIndexBuffer,
							std::map<SUBMATINDEX,WORD>& mapIndexCount,
							int nMaterialRef)
{
	assert(pIndexBuffer!=NULL);
	assert(pVertexBuffer!=NULL);

	cMeshNode* pNewSceneNode= new cMeshNode;

	//공통적인 데이터
	SetNodeInfo(pNewSceneNode,stInfo);
	if (stInfo.pParent!=NULL)
	{
		stInfo.pParent->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(stInfo.pParent);
	}
	else
	{		
		m_pSceneRoot->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(m_pSceneRoot);
	}	



	int nPrimitiveCount=0,nStartIndex=0;
	if ( m_vecMaterial[nMaterialRef].size() == 1)
	{
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nCount = (*it).second;
			nPrimitiveCount += nCount;			
		}			

		pNewSceneNode->SetPrimitiveCount(nPrimitiveCount);
		pNewSceneNode->SetStartIndex(0);		

		pNewSceneNode->SetRscVertextBuffer(pVertexBuffer);		
		pNewSceneNode->SetRscIndexBuffer(pIndexBuffer);
		pNewSceneNode->SetMatrial(m_vecMaterial[nMaterialRef][0]);				
	}
	else
	{
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nSubMaterialIndex = (*it).first;
			nPrimitiveCount= (*it).second;			

			cMeshNode* pSubNode= new cMeshNode;
			pNewSceneNode->AddMultiSub(pSubNode);
			
			SetNodeInfo(pSubNode,stInfo);
			if (stInfo.pParent!=NULL)
			{				
				pSubNode->SetParentNode(stInfo.pParent);
			}
			else
			{						
				pSubNode->SetParentNode(m_pSceneRoot);
			}	


			pSubNode->SetPrimitiveCount(nPrimitiveCount);
			pSubNode->SetStartIndex(nStartIndex);		

			pSubNode->SetRscVertextBuffer(pVertexBuffer);		
			pSubNode->SetRscIndexBuffer(pIndexBuffer);

			std::vector<Material>& refSubMaterial=m_vecMaterial[nMaterialRef];
			
			if ( (size_t)nSubMaterialIndex >= refSubMaterial.size()  )
			{
				pSubNode->SetMatrial(refSubMaterial[0]);
			}
			else
			{
				pSubNode->SetMatrial(refSubMaterial[nSubMaterialIndex]);
			}
			

			nStartIndex+=nPrimitiveCount*3; //cnt
		}			

	}
	return pNewSceneNode;
}

SkinnedMeshNode* 
cASEParser::CreateSkinnedMeshNode(SCENENODEINFO& stInfo,
								  cRscVertexBuffer* pVertexBuffer,
								  cRscIndexBuffer* pIndexBuffer,
								  std::map<SUBMATINDEX,WORD>& mapIndexCount,
								  int nMaterialRef,
								  std::vector<BONEREFINFO>& boneRef)
{
	assert(pIndexBuffer!=NULL);
	assert(pVertexBuffer!=NULL);

	SkinnedMeshNode* pNewSceneNode= new SkinnedMeshNode;

	//공통적인 데이터
	SetNodeInfo(pNewSceneNode,stInfo);
	if (stInfo.pParent!=NULL)
	{
		stInfo.pParent->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(stInfo.pParent);
	}
	else
	{		
		m_pSceneRoot->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(m_pSceneRoot);
	}	

	int nPrimitiveCount=0,nStartIndex=0;
	if (m_vecMaterial[nMaterialRef].size()==1)
	{
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			nPrimitiveCount=(*it).second;
			nStartIndex+=nPrimitiveCount*3; //cnt
		}			

		pNewSceneNode->SetPrimitiveCount(nPrimitiveCount);
		pNewSceneNode->SetStartIndex(nStartIndex);		

		pNewSceneNode->SetRscVertextBuffer(pVertexBuffer);		
		pNewSceneNode->SetRscIndexBuffer(pIndexBuffer);

		pNewSceneNode->SetMatrial(m_vecMaterial[nMaterialRef][0]);
		pNewSceneNode->SetBoneRef(boneRef);
	}
	else
	{
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nSubMaterialIndex = (*it).first;
			nPrimitiveCount= (*it).second;			

			SkinnedMeshNode* pSubNode= new SkinnedMeshNode;
			pNewSceneNode->AddMultiSub(pSubNode);

			SetNodeInfo(pSubNode,stInfo);
			if (stInfo.pParent!=NULL)
			{				
				pSubNode->SetParentNode(stInfo.pParent);
			}
			else
			{						
				pSubNode->SetParentNode(m_pSceneRoot);
			}				

			pSubNode->SetPrimitiveCount(nPrimitiveCount);
			pSubNode->SetStartIndex(nStartIndex);		

			pSubNode->SetRscVertextBuffer(pVertexBuffer);		
			pSubNode->SetRscIndexBuffer(pIndexBuffer);		

			pSubNode->SetMatrial(m_vecMaterial[nMaterialRef][nSubMaterialIndex]);
			pSubNode->SetBoneRef(boneRef);

			nStartIndex+=nPrimitiveCount*3; //cnt

		}			
	}
	return pNewSceneNode;
}

template <typename T>
cRscVertexBuffer* cASEParser::CreateRscVertexBuffer(const char* meshName,std::vector<T>& arrVertex)
{
	cRscVertexBuffer* pVertexBuffer=NULL;
	if (!arrVertex.empty())
	{
		DWORD nCount=(DWORD)arrVertex.size();
		pVertexBuffer = cResourceMng::m_pResourceMng->CreateRscVertexBuffer(m_SceneTime.FILENAME.c_str(),meshName,sizeof(T)*nCount);

		T* pVertices=(T*)pVertexBuffer->Lock();
		for (UINT i=0;i< nCount;i++)
		{
			memcpy(&pVertices[i],&arrVertex[i],sizeof(T));
		}	
		pVertexBuffer->Unlock();			
		pVertexBuffer->SetCount(nCount);
	}
	return pVertexBuffer;
}


cRscIndexBuffer* cASEParser::CreateRscIndexBuffer(const char* meshName,std::vector<TRIANGLE_SUBMATERIAL>& arrIndex)
{
	cRscIndexBuffer* pIndexBuffer=NULL;
	if (!arrIndex.empty())
	{
		DWORD nCount=(DWORD)arrIndex.size();
		pIndexBuffer = cResourceMng::m_pResourceMng->CreateRscIndexBuffer(m_SceneTime.FILENAME.c_str(),meshName,
			sizeof(TRIANGLE)*nCount);

		TRIANGLE* pIndices=(TRIANGLE*)pIndexBuffer->Lock();
		for (UINT i=0;i< nCount;i++)
		{
			memcpy(&pIndices[i],&arrIndex[i].triangle,sizeof(TRIANGLE));			
		}
		pIndexBuffer->Unlock();		
		pIndexBuffer->SetCount(nCount);
	}			
	return pIndexBuffer;
}

cSceneNode* cASEParser::CreateSceneNode(SCENENODEINFO& stInfo)
{
	cSceneNode* pNewSceneNode=new cSceneNode;	
	m_pLastObject = pNewSceneNode;

	//공통적인 데이터
	SetNodeInfo(pNewSceneNode,stInfo);
	if (stInfo.pParent!=NULL)
	{
		stInfo.pParent->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(stInfo.pParent);
	}
	else
	{		
		m_pSceneRoot->AttachChildNode(pNewSceneNode);
		pNewSceneNode->SetParentNode(m_pSceneRoot);
	}	
	return pNewSceneNode;
}

void cASEParser::Close()
{
	cASELexer::Close();
	m_vecMaterial.clear();
	m_tempAxisMin=D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_tempAxisMax=D3DXVECTOR3(0.0f,0.0f,0.0f);	
	m_pSceneRoot = NULL;
	m_pLastObject = NULL;
	m_CNTOBJECT = 0;
}

bool cASEParser::GetSubMaterial( Material& material)
{
	if (GetToken(m_TokenString) != TOKEND_BLOCK_START)
	{
			return FALSE;						
	}
	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		ASSERT(m_Token!=TOKEND_BLOCK_START);
		switch(m_Token)
		{
		case TOKENR_MATERIAL_AMBIENT:
			material.Ambient.r=GetFloat();
			material.Ambient.g=GetFloat();
			material.Ambient.b=GetFloat();
			break;
		case TOKENR_MATERIAL_DIFFUSE:
			material.Diffuse.r=GetFloat();
			material.Diffuse.g=GetFloat();
			material.Diffuse.b=GetFloat();
			break;
		case TOKENR_MATERIAL_SPECULAR:
			material.Specular.r=GetFloat();
			material.Specular.g=GetFloat();
			material.Specular.b=GetFloat();			
			break;
		case TOKENR_MAP_NAME:
		case TOKENR_MAP_CLASS:
			break;
		case TOKENR_MAP_SUBNO:
		case TOKENR_MAP_AMOUNT:			
		case TOKENR_MAP_SPECULAR:
		case TOKENR_MAP_SHINE:
		case TOKENR_MAP_GENERIC:
		case TOKENR_MAP_TYPE:
		case TOKENR_MAP_OPACITY:
		case TOKENR_MAP_REFLECT:
			{
				if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
					return FALSE;						
				while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{
					ASSERT(m_Token!=TOKEND_BLOCK_START);
					switch(m_Token)
					{
					case TOKENR_BITMAP:
						/*

						string strFileName=GetString().c_str();							
						string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
						string strFullPath = strDataPath;
						strFullPath += strFileName;

						cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
						if(pRscTexture==NULL)
						TRACE1("MAP_REFLECT: %s 파일이없습니다.\n",strFullPath.c_str());

						SubMatrial.SetMapRefract(pRscTexture);
						*/
						break;
					}		
				}
			}
			break;
		case TOKENR_MAP_REFRACT:
			{
				if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
					return FALSE;						
				while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{
					ASSERT(m_Token!=TOKEND_BLOCK_START);
					switch(m_Token)
					{
					case TOKENR_BITMAP:

						std::string strFileName=GetString().c_str();							
						/*
						std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
						std::string strFullPath = strDataPath;
						strFullPath += strFileName;

						cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
						if(pRscTexture==NULL)
							TRACE1("MAP_REFRACT: %s 파일이없습니다.\n",strFullPath.c_str());
						material.SetMapRefract(pRscTexture);
						*/
						break;
					}		
				}
			}
			break;
		case TOKENR_MAP_BUMP:
			{
				if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
					return FALSE;						
				while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{
					ASSERT(m_Token!=TOKEND_BLOCK_START);
					switch(m_Token)
					{
					case TOKENR_BITMAP:

						std::string strFileName=GetString().c_str();							
						std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
						std::string strFullPath = strDataPath;
						strFullPath += strFileName;

						cRscTexture* pRscTexture= cResourceMng::m_pResourceMng->CreateRscTexture(strFullPath.c_str());
						if(pRscTexture==NULL)
							TRACE1("MAP_BUMP: %s 파일이없습니다.\n",strFullPath.c_str());
						material.SetMapNormal(pRscTexture);
						break;
					}		
				}
			}
			break;
		case TOKENR_MAP_SELFILLUM:
		case TOKENR_MAP_AMBIENT:
		case TOKENR_MAP_SHINESTRENGTH:
		case TOKENR_MAP_FILTERCOLOR:		
			FindToken(TOKEND_BLOCK_START);
			FindToken(TOKEND_BLOCK_END);
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
						std::string strFileName=GetString().c_str();							
						std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
						std::string strFullPath = strDataPath;
						strFullPath += strFileName;

						cRscTexture* pRscTexture= cResourceMng::m_pResourceMng->CreateRscTexture(strFullPath.c_str());
						if(pRscTexture==NULL)
							TRACE1("MAP_DIFFUSE: %s 파일이없습니다.\n",strFullPath.c_str());
						material.SetMapDiffuse(pRscTexture);
						break;
					}
				}									
			}
			break;													
		case TOKENR_NUMSUBMTLS:
			{
				ASSERT(0&&"Matrial의 SubMatrial에서 SubMatrial을 또다시 사용하고있음");
			}
			break;
		}						
	}
	return true;
}

bool cASEParser::GetTextureVertexList(std::vector<TEXCOORD>& out)
{
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		ASSERT(m_Token!=TOKEND_BLOCK_START);
		switch(m_Token)
		{
		case TOKENR_MESH_TVERT:	
			int index=GetInt();

			D3DXVECTOR3 tvertex;
			GetVector3(&tvertex);
			tvertex.z = 1.0f-tvertex.z;
			out.push_back(TEXCOORD(tvertex.x,tvertex.z));
			break;
		}					
	}

	return true;
}

bool cASEParser::GetTextureFaceList(std::vector<TRIANGLE>& out)
{
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
	{
		ASSERT(m_Token!=TOKEND_BLOCK_START);
		switch(m_Token)
		{
		case TOKENR_MESH_TFACE:						
			int iTFace;
			TRIANGLE tFaceIndex;
			iTFace=GetInt();			// FaceIndex						
			tFaceIndex.index[0]=GetInt();			// 0						
			tFaceIndex.index[2]=GetInt();			// 1						
			tFaceIndex.index[1]=GetInt();			// 2						
			out.push_back(tFaceIndex);
			break;
		}					
	}
	return true;
}

void cASEParser::SetNodeInfo( cSceneNode* pNode,SCENENODEINFO& stInfo )
{
	pNode->SetNodeName(stInfo.strNodeName.c_str());
	pNode->SetParentName(stInfo.strParentName.c_str());
	pNode->SetLocalTM(stInfo.tmLocal);
	pNode->SetWorldTM(stInfo.tmWorld);
}
