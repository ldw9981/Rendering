#include "stdafx.h"
#include "Foundation/Trace.h"

#include "ASEParser.h"
#include "ASELexer.h"
#include "Lexer.h"



#include "Scene/MeshNode.h"
#include "Scene/HelperNode.h"
#include "Scene/ShapeNode.h"
#include "Scene/CameraNode.h"
#include "Scene/LightNode.h"
#include "Scene/SkinnedMeshNode.h"
#include "Framework/D3DFramework.h"
#include "D3D9Server/RscTransformAnm.h"
#include "Foundation/Define.h"
#include "Framework/EnvironmentVariable.h"

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


string cASEParser::GetString()
{	
	string temp;
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

BOOL cASEParser::Load( const char* strFileName ,cSceneNode* pOutput)
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
	UINT totalVertices=0,totalFaces=0,totalBoneRef=0,totalTVertices=0;
	UINT nMaterialRef=0;
	
	vector<BONEREFINFO>					vecBoneRef;		
	map<SUBMATINDEX,WORD>				mapIndexCount;	

	//cRscVertexBuffer에 복사할 내용
	vector<NORMALVERTEX>				vecTempNormalVertex; 
	vector<BLENDVERTEX>					vecTempBlendVertex;
	//cRscIndexBuffer에 복사할 내용
	vector<INDEX_FACE_SUBMATERIAL>		vecTempFaceIndex;
		
	// 버텍스 가공을 위한 일시적인 정보
	vector<TEXCOORD>					vecTempTVertex;
	vector<TEXCOORD>					vecTempTVertexOut;
	vector<FACEINDEX16>					vecTempTFaceIndex;	
	vector<VNORMAL>						vecTempVertexNormal;
	
	// 정점으로 Sphere를 만들기위한 임시 정보
	D3DXVECTOR3 tempAxisMin=D3DXVECTOR3(0.0f,0.0f,0.0f),tempAxisMax=D3DXVECTOR3(0.0f,0.0f,0.0f);	
	


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
				stInfo.pRscTransform = GetRscTransformAnm(stInfo.tmLocal);				
				
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
								switch(m_Token)
								{
								case TOKENR_MESH_VERTEX:
									if (totalBoneRef==0)
									{
										NORMALVERTEX Item;
										memset(&Item,0,sizeof(Item));
										int index=GetInt();
										GetVector3(&Item.vertex);
										// 원점중심 로컬좌표로 이동
										D3DXVec3TransformCoord(&Item.vertex,&Item.vertex,&stInfo.tmInvNode);
										vecTempNormalVertex.push_back(Item);	

										//Bounding Sphere를 위한 최대 최소 얻기
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
										// 원점중심 로컬좌표로 이동
										D3DXVec3TransformCoord(&Item.vertex,&Item.vertex,&stInfo.tmInvNode);
										vecTempBlendVertex.push_back(Item);	

										//Bounding Sphere를 위한 최대 최소 얻기
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
		
									// 서브매트리얼 ID를 키로 ID마다 사용되는 FACEINDEX수를 카운트한다.
									mapIndexCount[iMat]++;

									INDEX_FACE_SUBMATERIAL temp;		
									temp.faceIndex = tFaceIndex;						
									temp.subMaterialIndex = iMat;									
									vecTempFaceIndex.push_back(temp);
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
						vecTempTVertex.reserve(totalTVertices);

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
									vecTempTVertex.push_back(TEXCOORD(tvertex.x,tvertex.z));
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
									vecTempTFaceIndex.push_back(tFaceIndex);
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

							for (UINT iVertex=0;iVertex<vecTempBlendVertex.size();iVertex++)
							{												

								if(GetToken(m_TokenString) != TOKENR_MESH_WEIGHT)	
									return FALSE;
								
								GetInt();

								if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)	
									return FALSE;

								vector<BONEWEIGHT>	  vecBoneWeight;
								while (m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
								{
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

								vecTempBlendVertex[iVertex].SetIndex(bindex);
								vecTempBlendVertex[iVertex].SetWeight(bweight); 

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

	cSphere tempSphere;
	if (!bSkinned)	
	{
		CalculateSphere(tempAxisMin,tempAxisMax,vecTempNormalVertex,tempSphere);
	}
	else
	{
		CalculateSphere(tempAxisMin,tempAxisMax,vecTempBlendVertex,tempSphere);	
	}

	// 이제 버텍스 가공 버텍스,노말 합치기
	if (!bSkinned)	
	{
		MergeNormalListIntoVertexList(vecTempNormalVertex,vecTempFaceIndex,vecTempVertexNormal);
	}
	else
	{
		MergeNormalListIntoVertexList(vecTempBlendVertex,vecTempFaceIndex,vecTempVertexNormal);
	}

	// 텍스쳐 좌표줄이면서 텍스쳐좌표 인덱스 수정하기, 버텍스랑 합치면서 FaceIndex수정하기 	
	if (!bSkinned)	
	{
		MergeTexCoordListIntoVertexList(vecTempNormalVertex,vecTempFaceIndex,vecTempTVertex,vecTempTFaceIndex);
	}
	else
	{
		MergeTexCoordListIntoVertexList(vecTempBlendVertex,vecTempFaceIndex,vecTempTVertex,vecTempTFaceIndex);
	}

	// 서브매트리얼 ID별로 FACEINDEX정렬
	sort(vecTempFaceIndex.begin(),vecTempFaceIndex.end(),INDEX_FACE_SUBMATERIAL::LessFaceIndex);	

	// 리소스 버텍스 버퍼   생성 -> 데이터복사 -> 메쉬셋팅
	cRscVertexBuffer* pNewRscVertexBuffer=NULL;
	if(!bSkinned)	
	{
		pNewRscVertexBuffer = CreateRscVertexBuffer(vecTempNormalVertex);
	}
	else
	{
		pNewRscVertexBuffer = CreateRscVertexBuffer(vecTempBlendVertex);	
	}

	// 리소스 인덱스 버퍼 생성-> 데이터복사 -> 메쉬 세팅
	cRscIndexBuffer* pNewRscIndexBuffer=NULL;	
	pNewRscIndexBuffer = CreateRscIndexBuffer(vecTempFaceIndex);

	cSceneNode* pNewSceneNode=NULL;

	if (pNewRscIndexBuffer==NULL || pNewRscVertexBuffer==NULL)
	{
		pNewSceneNode = CreateHelperNode(stInfo);
		
	}
	else
	{
		if (!bSkinned)
		{
			pNewSceneNode = CreateMeshNode(stInfo,pNewRscVertexBuffer,pNewRscIndexBuffer,mapIndexCount,nMaterialRef);		
		
		}
		else
		{
			pNewSceneNode = CreateSkinnedMeshNode(stInfo,pNewRscVertexBuffer,pNewRscIndexBuffer,mapIndexCount,nMaterialRef,vecBoneRef );	
		}

		pNewSceneNode->SetBoundingSphere(tempSphere);
		pNewSceneNode->SetCullingSphere(tempSphere);
	}

	
	return TRUE;
}



BOOL cASEParser::Parsing_MaterialList()
{
	BOOL bRet=TRUE;
	int nMaterialCount,nMaterialIndex;
	if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	return FALSE;	
	if (GetToken(m_TokenString) != TOKENR_MATERIAL_COUNT)	return FALSE;	// *MATERIAL_COUNT
	nMaterialCount=GetInt();

	
	
	for (int i=0;i<nMaterialCount;i++)
	{
		Material Matrial;		
		vector<Material> vecSubMatrial;
		m_vecMultiSubMaterial.push_back(vecSubMatrial);

		if(GetToken(m_TokenString) != TOKENR_MATERIAL)		// *MATERIAL	
			return FALSE;			
		nMaterialIndex=GetInt();									// index
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
				Matrial.Multiply =GetFloat();
				break;
			case TOKENR_MATERIAL_SHINESTRENGTH:				
				Matrial.Power=GetFloat();
				break;
			case TOKENR_MATERIAL_TRANSPARENCY:				
				Matrial.Transparency=GetFloat();
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
						switch(m_Token)
						{
						case TOKENR_BITMAP:

							string strFileName=GetString().c_str();							
							string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							string strFullPath = strDataPath;
							strFullPath += strFileName;

							cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_REFRACT: %s 파일이없습니다.\n",strFullPath.c_str());
							Matrial.SetMapRefract(pRscTexture);
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
						switch(m_Token)
						{
						case TOKENR_BITMAP:

							string strFileName=GetString().c_str();							
							string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							string strFullPath = strDataPath;
							strFullPath += strFileName;

							cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_BUMP: %s 파일이없습니다.\n",strFullPath.c_str());
							Matrial.SetMapBump(pRscTexture);
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
					
							string strFileName=GetString().c_str();							
							string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							string strFullPath = strDataPath;
							strFullPath += strFileName;

							cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_DIFFUSE: %s 파일이없습니다.\n",strFullPath.c_str());
							Matrial.SetMapDiffuse(pRscTexture);
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
						Material SubMatrial;						
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
										switch(m_Token)
										{
										case TOKENR_BITMAP:

											string strFileName=GetString().c_str();							
											string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
											string strFullPath = strDataPath;
											strFullPath += strFileName;

											cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
											if(pRscTexture==NULL)
												TRACE1("MAP_REFRACT: %s 파일이없습니다.\n",strFullPath.c_str());
											SubMatrial.SetMapRefract(pRscTexture);
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
										switch(m_Token)
										{
										case TOKENR_BITMAP:

											string strFileName=GetString().c_str();							
											string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
											string strFullPath = strDataPath;
											strFullPath += strFileName;

											cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
											if(pRscTexture==NULL)
												TRACE1("MAP_BUMP: %s 파일이없습니다.\n",strFullPath.c_str());
											Matrial.SetMapBump(pRscTexture);
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
											string strFileName=GetString().c_str();							
											string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
											string strFullPath = strDataPath;
											strFullPath += strFileName;

											cRscTexture* pRscTexture= m_ResourceMng.CreateRscTexture(strFullPath.c_str());
											if(pRscTexture==NULL)
												TRACE1("MAP_DIFFUSE: %s 파일이없습니다.\n",strFullPath.c_str());
											SubMatrial.SetMapDiffuse(pRscTexture);
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
						//삽입
						 m_vecMultiSubMaterial[nMaterialIndex].push_back(SubMatrial);
					}

				}//case TOKENR_NUMSUBMTLS:
				break;		
			}//switch			

		}//while (m_Token=GetLexer()->GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
		
		m_vecMaterial.push_back(Matrial);
	}//for (int i=0;i<nNUMMaterial;i++)	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_END) 
		return FALSE;	// }	

	
	return bRet;
}


BOOL cASEParser::Parsing_HelperObject()
{	
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
				stInfo.pRscTransform = GetRscTransformAnm(stInfo.tmLocal);
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



	cHelperNode* pNewSceneNode = CreateHelperNode(stInfo);
	return TRUE;
}

BOOL cASEParser::Parsing_ShapeObject()
{
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
				stInfo.pRscTransform = GetRscTransformAnm(stInfo.tmLocal);
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

	cShapeNode*	pNewSceneNode=new cShapeNode;
	m_pLastObject = pNewSceneNode;

	//공통적인 데이터
	pNewSceneNode->SetNodeInfo(stInfo);

	pNewSceneNode->SetRootNode(m_pSceneRoot);
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



	return TRUE;
}

BOOL cASEParser::Parsing_LightObject()
{	
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
				stInfo.pRscTransform = GetRscTransformAnm(stInfo.tmLocal);
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

	cLightNode* pNewSceneNode=new cLightNode;	
	m_pLastObject = pNewSceneNode;
	
	//공통적인 데이터
	pNewSceneNode->SetNodeInfo(stInfo);

	pNewSceneNode->SetRootNode(m_pSceneRoot);
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
	
	return TRUE;
}

BOOL cASEParser::Parsing_CameraObject()
{
	SCENENODEINFO stInfo;	

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
				stInfo.pRscTransform = GetRscTransformAnm(stInfo.tmLocal);
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

	cCameraNode* pNewSceneNode=new cCameraNode;
	m_pLastObject = pNewSceneNode;

	//공통적인 데이터
	pNewSceneNode->SetNodeInfo(stInfo);

	pNewSceneNode->SetRootNode(m_pSceneRoot);
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
	

	pNewSceneNode->SetFOV(fFov);
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
void cASEParser::MergeNormalListIntoVertexList(vector<T>& arrVertex,
								   vector<INDEX_FACE_SUBMATERIAL>& arrFaceIndex,
								   const vector<VNORMAL>& arrVNormal)
{
	// arrVertex를 참조하는 인덱스기준으로 FACE리스트를 만든다.
	vector<list<WORD>>						arrVRefFaceList;	
	// arrVertex를 참조하는 기준으로 뒤쪽 WORD는 이미존재하는 노말을갖는 버텍스의 인덱스
	vector<list<pair<D3DXVECTOR3,WORD>>>    arrVRefNormalInsert;			

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
		arrVRefFaceList[arrFaceIndex[iFace].faceIndex.index[0]].push_back(iFace);	
		arrVRefFaceList[arrFaceIndex[iFace].faceIndex.index[1]].push_back(iFace);
		arrVRefFaceList[arrFaceIndex[iFace].faceIndex.index[2]].push_back(iFace);
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

					if (arrFaceIndex[iRefFace].faceIndex.index[0] == iRefVertex )
						arrFaceIndex[iRefFace].faceIndex.index[0] = iExist;

					if (arrFaceIndex[iRefFace].faceIndex.index[1] == iRefVertex )
						arrFaceIndex[iRefFace].faceIndex.index[1] = iExist;

					if (arrFaceIndex[iRefFace].faceIndex.index[2]== iRefVertex )
						arrFaceIndex[iRefFace].faceIndex.index[2] = iExist;					

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
	//랜덤액세스를 위한 할당
	arrVIndexedTCList.resize(arrVertexInOut.size());

	int nFace=(int)arrVFaceIndexInOut.size();
	for (int iFace=0;iFace<nFace;iFace++)
	{
		FACEINDEX16*		 pVFace=&arrVFaceIndexInOut[iFace].faceIndex;
		const FACEINDEX16* pTFace=&arrTFaceIndexIn[iFace];

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

template <typename T>
void cASEParser::CalculateSphere(D3DXVECTOR3& tempAxisMin,D3DXVECTOR3& tempAxisMax,vector<T>& arrVertex,cSphere& out )
{
	D3DXVECTOR3 tempCenterPos=D3DXVECTOR3(0.0f,0.0f,0.0f);
	float		tempRadius=0.0f;
	//버텍스 가공 하기전에 Bounding Sphere구하기
	// 최대 최소의 중간값으로 Center pos을 구한다.
	tempCenterPos.x= tempAxisMin.x + (tempAxisMax.x - tempAxisMin.x)/2.0f;
	tempCenterPos.y= tempAxisMin.y + (tempAxisMax.y - tempAxisMin.y)/2.0f;
	tempCenterPos.z= tempAxisMin.z + (tempAxisMax.z - tempAxisMin.z)/2.0f;

	float MaxLengthSq=0.0f,LengthSq=0.0f;
	int nCount = (int)arrVertex.size();
	for (int i=0;i<nCount;i++)
	{		
		LengthSq=D3DXVec3LengthSq(&D3DXVECTOR3(tempCenterPos - arrVertex[i].vertex));
		MaxLengthSq=max(MaxLengthSq,LengthSq);		
	}	
	tempRadius=sqrt(MaxLengthSq);
	
	out.Make(tempCenterPos,tempRadius);
}



cRscTransformAnm* cASEParser::GetRscTransformAnm( const D3DXMATRIX& localTM )
{
	cRscTransformAnm* pRscTransformAnm = m_ResourceMng.CreateRscTransformAnm();
	ANMKEY localTM_anmkey;
	D3DXMatrixDecompose(
		&localTM_anmkey.ScaleAccum,
		&localTM_anmkey.RotationAccum,
		&localTM_anmkey.TranslationAccum,
		&localTM);	

	// time - AnmKey
	DWORD dwTimeKey=0;
	
	map<DWORD,ANMKEY> mapAnmKey;

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
				vector<pair<DWORD,D3DXQUATERNION>> arrayROTKEY;

				if(GetToken(m_TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while(m_Token=GetToken(m_TokenString),m_Token!=TOKEND_BLOCK_END)
				{	
					if(m_Token!=TOKENR_CONTROL_ROT_SAMPLE)
						return FALSE;	

					float ang;
					D3DXVECTOR3 axis;
					dwTimeKey =  GetInt() / m_SceneTime.EX_TICKSPERMS;	
					GetVector3(&axis);

					pair<DWORD,D3DXQUATERNION> ItemDelta;
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

				vector<pair<DWORD,D3DXQUATERNION>>::iterator rot_it=arrayROTKEY.begin();
				for ( ; rot_it!=arrayROTKEY.end() ; rot_it++)
				{
					pair<DWORD,D3DXQUATERNION>& Item = *rot_it;

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

				}
			}
			break;
		}
	} 
	

	pRscTransformAnm->SetTimeLength(dwTimeKey);
	vector<ANMKEY>& refArrAnmKey=pRscTransformAnm->GetArrayANMKEY();
	
	ANMKEY prevItem=localTM_anmkey;
	map<DWORD,ANMKEY>::iterator iter = mapAnmKey.begin();
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
void cASEParser::ConvertAccQuaternion(vector<ROTKEY>& arrayROTKEY,const D3DXMATRIX& localTM)
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

	vector<ROTKEY>::iterator rot_it=arrayROTKEY.begin();
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
void cASEParser::ConvertAccQuaternionEX(vector<pair<DWORD,D3DXQUATERNION>>& inArrayROTKEY,const D3DXMATRIX& inLocalTM,map<DWORD,ANMKEY> outRefMapAnmKey)
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

	vector<pair<DWORD,D3DXQUATERNION>>::iterator rot_it=inArrayROTKEY.begin();
	for ( ; rot_it!=inArrayROTKEY.end() ; rot_it++)
	{
		pair<DWORD,D3DXQUATERNION>& Item = *rot_it;

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
							map<SUBMATINDEX,WORD>& mapIndexCount,
							int nMaterialRef)
{
	assert(pIndexBuffer!=NULL);
	assert(pVertexBuffer!=NULL);

	cMeshNode* pNewSceneNode= new cMeshNode;

	//공통적인 데이터
	pNewSceneNode->SetNodeInfo(stInfo);

	pNewSceneNode->SetRootNode(m_pSceneRoot);
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

	
	bool bIsMultiSub=false;
	if(  !m_vecMultiSubMaterial.empty() && !m_vecMultiSubMaterial[nMaterialRef].empty())
	{
		bIsMultiSub=true;
	}

	int nPrimitiveCount=0,nStartIndex=0;
	if (!bIsMultiSub)
	{
		map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nCount = (*it).second;
			nPrimitiveCount += nCount;			
		}			

		pNewSceneNode->SetPrimitiveCount(nPrimitiveCount);
		pNewSceneNode->SetStartIndex(0);		

		pNewSceneNode->SetRscVertextBuffer(pVertexBuffer);		
		pNewSceneNode->SetRscIndexBuffer(pIndexBuffer);

		if (!m_vecMaterial.empty())
		{
			pNewSceneNode->SetMatrial(m_vecMaterial[nMaterialRef]);
		}		
	}
	else
	{
		map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nSubMaterialIndex = (*it).first;
			nPrimitiveCount= (*it).second;			

			cMeshNode* pSubNode= new cMeshNode;
			pNewSceneNode->AddMultiSub(pSubNode);
			
			pSubNode->SetNodeInfo(stInfo);
			pSubNode->SetRootNode(m_pSceneRoot);
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

			vector<Material>& refSubMaterial=m_vecMultiSubMaterial[nMaterialRef];
			pSubNode->SetMatrial(refSubMaterial[nSubMaterialIndex]);

			nStartIndex+=nPrimitiveCount*3; //cnt
		}			

	}
	return pNewSceneNode;
}

SkinnedMeshNode* 
cASEParser::CreateSkinnedMeshNode(SCENENODEINFO& stInfo,
								  cRscVertexBuffer* pVertexBuffer,
								  cRscIndexBuffer* pIndexBuffer,
								  map<SUBMATINDEX,WORD>& mapIndexCount,
								  int nMaterialRef,
								  vector<BONEREFINFO>& boneRef)
{
	assert(pIndexBuffer!=NULL);
	assert(pVertexBuffer!=NULL);

	SkinnedMeshNode* pNewSceneNode= new SkinnedMeshNode;

	//공통적인 데이터
	pNewSceneNode->SetNodeInfo(stInfo);

	pNewSceneNode->SetRootNode(m_pSceneRoot);
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

	bool bIsMultiSub=false;
	if(  !m_vecMultiSubMaterial.empty() && !m_vecMultiSubMaterial[nMaterialRef].empty())
	{
		bIsMultiSub=true;
	}

	int nPrimitiveCount=0,nStartIndex=0;
	if (!bIsMultiSub)
	{
		map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			nPrimitiveCount=(*it).second;
			nStartIndex+=nPrimitiveCount*3; //cnt
		}			

		pNewSceneNode->SetPrimitiveCount(nPrimitiveCount);
		pNewSceneNode->SetStartIndex(nStartIndex);		

		pNewSceneNode->SetRscVertextBuffer(pVertexBuffer);		
		pNewSceneNode->SetRscIndexBuffer(pIndexBuffer);

		pNewSceneNode->SetMatrial(m_vecMaterial[nMaterialRef]);
		pNewSceneNode->SetBoneRef(boneRef);
	}
	else
	{
		map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nSubMaterialIndex = (*it).first;
			nPrimitiveCount= (*it).second;			

			SkinnedMeshNode* pSubNode= new SkinnedMeshNode;
			pNewSceneNode->AddMultiSub(pSubNode);

			pSubNode->SetNodeInfo(stInfo);
			pSubNode->SetRootNode(m_pSceneRoot);
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

			pSubNode->SetMatrial(m_vecMultiSubMaterial[nMaterialRef][nSubMaterialIndex]);
			pSubNode->SetBoneRef(boneRef);

			nStartIndex+=nPrimitiveCount*3; //cnt

		}			
	}
	return pNewSceneNode;
}

template <typename T>
cRscVertexBuffer* cASEParser::CreateRscVertexBuffer(vector<T>& arrVertex)
{
	cRscVertexBuffer* pVertexBuffer=NULL;
	if (!arrVertex.empty())
	{
		DWORD nVertices=(DWORD)arrVertex.size();
		pVertexBuffer = m_ResourceMng.CreateRscVertexBuffer(sizeof(T)*nVertices);

		T* pVertices=(T*)pVertexBuffer->Lock();
		for (UINT i=0;i< arrVertex.size();i++)
		{
			memcpy(&pVertices[i],&arrVertex[i],sizeof(T));
		}	
		pVertexBuffer->Unlock();			
		pVertexBuffer->SetVerties(nVertices);
	}
	return pVertexBuffer;
}


cRscIndexBuffer* cASEParser::CreateRscIndexBuffer(vector<INDEX_FACE_SUBMATERIAL>& arrIndex)
{
	cRscIndexBuffer* pIndexBuffer=NULL;
	if (!arrIndex.empty())
	{
		pIndexBuffer = m_ResourceMng.CreateRscIndexBuffer(
			sizeof(FACEINDEX16)*(DWORD)arrIndex.size());

		FACEINDEX16* pIndices=(FACEINDEX16*)pIndexBuffer->Lock();
		for (UINT i=0;i< arrIndex.size();i++)
		{
			memcpy(&pIndices[i],&arrIndex[i].faceIndex,sizeof(FACEINDEX16));			
		}
		pIndexBuffer->Unlock();		
	}			
	return pIndexBuffer;
}

cHelperNode* cASEParser::CreateHelperNode(SCENENODEINFO& stInfo)
{
	cHelperNode* pNewSceneNode=new cHelperNode;	
	m_pLastObject = pNewSceneNode;

	//공통적인 데이터
	pNewSceneNode->SetNodeInfo(stInfo);

	pNewSceneNode->SetRootNode(m_pSceneRoot);
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


