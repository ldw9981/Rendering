#include "stdafx.h"
#include "Foundation/Trace.h"
#include "ASEParser.h"
#include "ASELexer.h"
#include "Lexer.h"
#include "Scene/MeshNode.h"
#include "Scene/CameraNode.h"
#include "Scene/SkinnedMeshNode.h"
#include "Resource/ResourceMng.h"
#include "Foundation/Define.h"
#include "Foundation/StringUtil.h"
#include "Foundation/EnvironmentVariable.h"
#include "Graphics/Entity.h"
#include "Scene/Skeleton.h"
namespace Sophia
{

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
	
	StringUtil::SplitPath(std::string(strFileName),NULL,NULL,&m_SceneTime.FILENAME,NULL);

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
		TRACE("TOKENR_3DSMAX_ASCIIEXPORT ������\n");
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
						
				TRACE3("���ǵ��� ���� Command�Դϴ�. ( %s ,token= %d , tokenstring= %s )\n", __FUNCTION__,m_Token,m_TokenString );						
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
	m_pSceneRoot->PushAnimation(m_pEntityAnimation);
	m_pSceneRoot->PushMaterial(m_pEntityMaterial);


	// *************************************************************
	// 
	// ���� ��!!!
	// 
	// *************************************************************
	TRACE1(_T("cASEImport Done count%d\n"),m_CNTOBJECT);
	return	TRUE;
}

BOOL cASEParser::Parsing_GeoObject()
{	
	//�ν��Ͻ����� ���Ե�����
	bool bSkinned=false;
	bool bMultiSub=false;
	UINT totalVertices=0,totalFaces=0,totalBoneRef=0;
	UINT totalBaseTVertices=0,totalExtraTVertices=0;
	UINT nMaterialRef=0;
	
	std::vector<BONEREFINFO>					vecBoneRef;		
	std::map<SUBMATINDEX,WORD>				mapIndexCount;	

	//cRscVertexBuffer�� ������ ����
	std::vector<NORMALVERTEX>				vecNormalVertexForBuffer; 
	std::vector<BLENDVERTEX>				vecBlendVertexForBuffer;
	//cRscIndexBuffer�� ������ ����
	std::vector<TRIANGLE_SUBMATERIAL>		vecIndexForBuffer;
		
	// ���ؽ� ������ ���� �Ͻ����� ����
	std::vector<TEXCOORD>					vecTempBaseTVertexOut;
	std::vector<VNORMAL>					vecTempVertexNormal;

	std::vector<TEXCOORD>					vecTempBaseTVertex;
	std::vector<TRIANGLE>					vecTempBaseTFaceIndex;
	
	std::vector<TEXCOORD>					vecTempExtraTVertex;
	std::vector<TRIANGLE>					vecTempExtraTFaceIndex;	

	// �������� Sphere�� ��������� �ӽ� ����
	SceneAnimation* pSceneAnimation = NULL;

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

			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX matNodeInv;
				D3DXMatrixInverse(&matNodeInv,NULL,&stInfo.pParent->GetNodeTM());
				stInfo.tmLocal = stInfo.tmNode * matNodeInv ;
			}			
			break;

		case TOKENR_TM_ANIMATION:
			{					
				GetSceneAnimation(stInfo.strNodeName.c_str(),stInfo.tmLocal);
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

										//Bounding Sphere�� ���� �ִ� �ּ� ���
										m_tempAxisMin.x= min(Item.vertex.x,m_tempAxisMin.x);
										m_tempAxisMin.y= min(Item.vertex.y,m_tempAxisMin.y);
										m_tempAxisMin.z= min(Item.vertex.z,m_tempAxisMin.z);

										m_tempAxisMax.x= max(Item.vertex.x,m_tempAxisMax.x);
										m_tempAxisMax.y= max(Item.vertex.y,m_tempAxisMax.y);
										m_tempAxisMax.z= max(Item.vertex.z,m_tempAxisMax.z);		

										// �����߽� ������ǥ�� �̵�
										D3DXVec3TransformCoord(&Item.vertex,&Item.vertex,&stInfo.tmInvNode);
										vecNormalVertexForBuffer.push_back(Item);
									}
									else
									{
										BLENDVERTEX Item;
										memset(&Item,0,sizeof(Item));
										int index=GetInt();
										GetVector3(&Item.vertex);			

										//Bounding Sphere�� ���� �ִ� �ּ� ���
										m_tempAxisMin.x= min(Item.vertex.x,m_tempAxisMin.x);
										m_tempAxisMin.y= min(Item.vertex.y,m_tempAxisMin.y);
										m_tempAxisMin.z= min(Item.vertex.z,m_tempAxisMin.z);

										m_tempAxisMax.x= max(Item.vertex.x,m_tempAxisMax.x);
										m_tempAxisMax.y= max(Item.vertex.y,m_tempAxisMax.y);
										m_tempAxisMax.z= max(Item.vertex.z,m_tempAxisMax.z);

										// �����߽� ������ǥ�� �̵�
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
		
									// �����Ʈ���� ID�� Ű�� ID���� ���Ǵ� FACEINDEX���� ī��Ʈ�Ѵ�.
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
								NewItem.pNode=NULL;
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

								// ����ġ�� ū �������� ����
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

	// �θ� ������ ��Ʈ�� �θ�� �Ѵ�.
	if (stInfo.pParent==NULL)
	{
		assert(m_pSceneRoot!=NULL);
		stInfo.pParent=m_pSceneRoot;
	}

	
		
	// ���� ���ؽ� ���� ���ؽ�,�븻 ��ġ��
	if (!bSkinned) MergeNormalListIntoVertexList(vecNormalVertexForBuffer,vecIndexForBuffer,vecTempVertexNormal);
	else MergeNormalListIntoVertexList(vecBlendVertexForBuffer,vecIndexForBuffer,vecTempVertexNormal);	

	// �ؽ��� ��ǥ���̸鼭 �ؽ�����ǥ �ε��� �����ϱ�, ���ؽ��� ��ġ�鼭 FaceIndex�����ϱ� 	
	if (!bSkinned) MergeTexCoordListIntoVertexList(true,vecNormalVertexForBuffer,vecIndexForBuffer,vecTempBaseTVertex,vecTempBaseTFaceIndex);
	else MergeTexCoordListIntoVertexList(true,vecBlendVertexForBuffer,vecIndexForBuffer,vecTempBaseTVertex,vecTempBaseTFaceIndex);

	if (!bSkinned) MergeTexCoordListIntoVertexList(false,vecNormalVertexForBuffer,vecIndexForBuffer,vecTempExtraTVertex,vecTempExtraTFaceIndex);
	else MergeTexCoordListIntoVertexList(false,vecBlendVertexForBuffer,vecIndexForBuffer,vecTempExtraTVertex,vecTempExtraTFaceIndex);

	
	// �����Ʈ���� ID���� FACEINDEX����
	sort(vecIndexForBuffer.begin(),vecIndexForBuffer.end(),TRIANGLE_SUBMATERIAL::LessFaceIndex);	

	// ���ҽ� ���ؽ� ����   ���� -> �����ͺ��� -> �޽�����
	cRscVertexBuffer* pNewRscVertexBuffer=NULL;
	if(!bSkinned) pNewRscVertexBuffer = CreateRscVertexBuffer(stInfo.strNodeName.c_str(),vecNormalVertexForBuffer);
	else pNewRscVertexBuffer = CreateRscVertexBuffer(stInfo.strNodeName.c_str(),vecBlendVertexForBuffer);		

	// ���ҽ� �ε��� ���� ����-> �����ͺ��� -> �޽� ����
	cRscIndexBuffer* pNewRscIndexBuffer = CreateRscIndexBuffer(stInfo.strNodeName.c_str(),vecIndexForBuffer);
	cSceneNode* pNewSceneNode=NULL;
	if (pNewRscIndexBuffer==NULL || pNewRscVertexBuffer==NULL )
	{
		pNewSceneNode = CreateSceneNode(stInfo);		
	}
	else
	{
		pNewRscVertexBuffer->AddRef();
		pNewRscIndexBuffer->AddRef();

		if (!bSkinned)	
		{
			bool bSkeleton = false;			
			if( stInfo.strNodeName.find("Bip") != std::string::npos || stInfo.strNodeName.find("bip") != std::string::npos ||
				stInfo.strNodeName.find("Bone") != std::string::npos || stInfo.strNodeName.find("bone") != std::string::npos )
			{
				bSkeleton = true;			
			}
			
			pNewSceneNode = CreateMeshNode(bSkeleton,stInfo,pNewRscVertexBuffer,pNewRscIndexBuffer,mapIndexCount,nMaterialRef);				
		}
		else 
		{
			pNewSceneNode = CreateSkinnedMeshNode(stInfo,pNewRscVertexBuffer,pNewRscIndexBuffer,mapIndexCount,nMaterialRef,vecBoneRef );		
		}		
		SAFE_RELEASE(pNewRscIndexBuffer);
		SAFE_RELEASE(pNewRscVertexBuffer);
	}	

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
								TRACE1("MAP_REFLECT: %s �����̾����ϴ�.\n",strFullPath.c_str());
							
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
								TRACE1("MAP_REFRACT: %s �����̾����ϴ�.\n",strFullPath.c_str());
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
						//ASSERT(m_Token!=TOKEND_BLOCK_START);
						switch(m_Token)
						{
						case TOKENR_BITMAP:

							std::string strFileName=GetString().c_str();							
							std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							std::string strFullPath = strDataPath;
							strFullPath += strFileName;					

							cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_BUMP: %s �����̾����ϴ�.\n",strFullPath.c_str());
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
						//ASSERT(m_Token!=TOKEND_BLOCK_START);
						switch(m_Token)
						{
						case TOKENR_BITMAP:
					
							std::string strFileName=GetString().c_str();							
							std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
							std::string strFullPath = strDataPath;
							strFullPath += strFileName;

							cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
							if(pRscTexture==NULL)
								TRACE1("MAP_DIFFUSE: %s �����̾����ϴ�.\n",strFullPath.c_str());
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

	if (m_vecMaterial.empty())
	{
		Material material;		
		std::vector<Material> vecSubMatrial;
		vecSubMatrial.push_back(material);
		m_vecMaterial.push_back(vecSubMatrial);
	}

	m_pEntityMaterial = cResourceMng::m_pInstance->CreateEntityMaterial(m_SceneTime.FILENAME.c_str());
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
			
			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX matNodeInv;
				D3DXMatrixInverse(&matNodeInv,NULL,&stInfo.pParent->GetNodeTM());
				stInfo.tmLocal = stInfo.tmNode * matNodeInv ;
			}			
			break;

		case TOKENR_TM_ANIMATION:
			{									
				GetSceneAnimation(stInfo.strNodeName.c_str(),stInfo.tmLocal);
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
			
			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX matNodeInv;
				D3DXMatrixInverse(&matNodeInv,NULL,&stInfo.pParent->GetNodeTM());
				stInfo.tmLocal = stInfo.tmNode * matNodeInv ;
			}			
			break;
		case TOKENR_TM_ANIMATION:
			{					
				GetSceneAnimation(stInfo.strNodeName.c_str(),stInfo.tmLocal);
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

	//�������� ������
	if(stInfo.pParent == NULL)
		stInfo.pParent = m_pSceneRoot;

	SetNodeInfo(pNewSceneNode,stInfo);
	stInfo.pParent->AttachChildNode(pNewSceneNode);
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
			
			if (stInfo.pParent==NULL)
			{					
				stInfo.tmLocal = stInfo.tmNode;
			}
			else
			{					
				D3DXMATRIX matNodeInv;
				D3DXMatrixInverse(&matNodeInv,NULL,&stInfo.pParent->GetNodeTM());
				stInfo.tmLocal = stInfo.tmNode * matNodeInv ;
			}			
			break;
		case TOKENR_TM_ANIMATION:
			{					
				GetSceneAnimation(stInfo.strNodeName.c_str(),stInfo.tmLocal);
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
	
	//�������� ������
	if(stInfo.pParent == NULL)
		stInfo.pParent = m_pSceneRoot;
	SetNodeInfo(pNewSceneNode,stInfo);
	stInfo.pParent->AttachChildNode(pNewSceneNode);

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

				//���Ǻ���
				D3DXMatrixRotationAxis(&NodeRotation,&D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXToRadian(90));
				nodeTM = NodeRotation * nodeTM;				

				stInfo.tmNode = nodeTM;
				D3DXMatrixInverse(&stInfo.tmInvNode,NULL,&stInfo.tmNode);
				
				if (stInfo.pParent==NULL)
				{					
					stInfo.tmLocal = stInfo.tmNode;
				}
				else
				{					
					D3DXMATRIX matNodeInv;
					D3DXMatrixInverse(&matNodeInv,NULL,&stInfo.pParent->GetNodeTM());
					stInfo.tmLocal = stInfo.tmNode * matNodeInv ;
				}			

				bLoadCameraNodeTM=TRUE;
			}		
			else
			{
				SkipBlock();	// TargetTM�� ������������Ƿ� Pass
			}
			
			break;
		case TOKENR_TM_ANIMATION:		
			if (bLoadCameraAnmTM==FALSE)
			{						
				GetSceneAnimation(stInfo.strNodeName.c_str(),stInfo.tmLocal);
				bLoadCameraAnmTM=TRUE;												
			}			
			else
			{
				SkipBlock();// TargetTM�� ������������Ƿ� Pass
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

	//�������� ������
	if(stInfo.pParent == NULL)
		stInfo.pParent = m_pSceneRoot;

	SetNodeInfo(pNewSceneNode,stInfo);
	stInfo.pParent->AttachChildNode(pNewSceneNode);
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
				TRACE3("���ǵ��� ���� Command�Դϴ�. ( %s ,token= %d , tokenstring= %s )\n",__FUNCTION__,m_Token,m_TokenString );						
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
			//m_SceneTime.FILENAME=GetString(); ���������̸��� �ƽ����� ���� �̸��� �ٸ��������Ƿ� �����������
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
	m_SceneTime.EX_MSPERFRAME = 1000.0f / (float)m_SceneTime.FRAMESPEED;
	// 1ms�� ƽ�� ���ϱ� 
	m_SceneTime.EX_TICKSPERMS = (float)m_SceneTime.TICKSPERFRAME/m_SceneTime.EX_MSPERFRAME;
	// ������ ������ ms���ϱ�
	m_SceneTime.EX_LASTFRAMEMS = (float)m_SceneTime.LASTFRAME * m_SceneTime.EX_MSPERFRAME;

	m_pEntityAnimation = cResourceMng::m_pInstance->CreateEntityAnimation(m_SceneTime.FILENAME.c_str());
	if (m_pEntityAnimation->GetRefCounter()==0)
	{
		m_pEntityAnimation->m_dwTimeLength = (DWORD)m_SceneTime.EX_LASTFRAMEMS;
	}

	return TRUE;
}


// �븻����Ʈ�� ���ؽ� ����Ʈ�� ��ģ��. 
// arrVertex [in,out] , arrVNormal [in] ,  arrFaceIndex[in,out]
template <typename T>
void cASEParser::MergeNormalListIntoVertexList(std::vector<T>& arrVertex,
								   std::vector<TRIANGLE_SUBMATERIAL>& arrFaceIndex,
								   const std::vector<VNORMAL>& arrVNormal)
{
	// arrVertex�� �����ϴ� �ε����������� FACE����Ʈ�� �����.
	std::vector<std::list<WORD>>						arrVRefFaceList;	
	// arrVertex�� �����ϴ� �������� ���� WORD�� �̹������ϴ� �븻������ ���ؽ��� �ε���
	std::vector<std::list<std::pair<D3DXVECTOR3,WORD>>>    arrVRefNormalInsert;			

	if (arrVertex.empty())
		return;

	if (arrFaceIndex.empty())
		return;

	arrVRefFaceList.resize(arrVertex.size());
	arrVRefNormalInsert.resize(arrVertex.size());

	//1. arrVertex�� �����ϴ� �ε����������� FACEINDEX����Ʈ�� �����.
	// ��. �ε������� ����Ʈ�� ��� FACEINDEX���� ��� arrVertex�� Ư�� �ε�����ǥ�� ����Ѵ�.
	int nFace=(int)arrFaceIndex.size();
	for (int iFace=0;iFace<nFace;iFace++ )
	{
		arrVRefFaceList[arrFaceIndex[iFace].triangle.index[0]].push_back(iFace);	
		arrVRefFaceList[arrFaceIndex[iFace].triangle.index[1]].push_back(iFace);
		arrVRefFaceList[arrFaceIndex[iFace].triangle.index[2]].push_back(iFace);
	}

	//2. arrFaceNormal �� arrVertex�� ������ arrFace���� ���ؽ��ε����� �����Ѵ�.
	WORD nVNormal=(WORD)arrVNormal.size();
	for (WORD iVNormal=0;iVNormal<nVNormal;iVNormal++)
	{
		std::list<std::pair<D3DXVECTOR3,WORD>>* pListNormalnsert;


		WORD iRefVertex=arrVNormal[iVNormal].iRefVertex;
		pListNormalnsert=&arrVRefNormalInsert[iRefVertex];

		if(pListNormalnsert->empty())
		{	// arrVertex �ε����� �븻���� ������ �׳� �ִ´�.			
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
				{	//�븻�� ����Ʈ�� �����ϸ� �ε����� �����Ѵ�.
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

			// �븻 �������� ������ ���ο� ���ؽ� �߰��Ѵ�.
			if (it == pListNormalnsert->end())
			{
				arrVertex.push_back(arrVertex[iRefVertex]);
				WORD iInsert=(WORD)arrVertex.size()-1;
				arrVertex[iInsert].normal = *pNewNormal;

				// �ε����� �ش�Ǵ� �븻����Ʈ�� �븻��,���� ���� �ε��� �߰�
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
	std::vector<std::list<int>>			arrTVIndexedFaceIndexList;	// �ش���ؽ��� ����ϴ� ���̽��ε����� ����Ʈ�� ���ؽ��ε����� �ε���ȭ

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
		std::pair<std::map<float,std::map<float,WORD>>::iterator	,bool> retItInsertU;
		std::pair<std::map<float,WORD>::iterator				,bool> retItInsertV;

		std::map<float,WORD> mapVINDEX;
		std::map<float,WORD>* pmapVINDEX;

		// U��ǥ�� �� mapVINDEX ����
		retItInsertU= mapUVINDEX.insert(std::make_pair(arrTexCoordIn[iTexCoordIn].u,mapVINDEX));
		pmapVINDEX=&(retItInsertU.first->second);		
		retItInsertV=pmapVINDEX->insert(std::make_pair(arrTexCoordIn[iTexCoordIn].v,0));

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

// arrVertexInOut    �������ϴ� ���ؽ��� �ؽ�����ǥ�� ������ �ٸ� ������ ��ȿ�Ѱ� �����Ѵٴ� �������Ѵ�. (���ؽ�,�븻,����ġ������,,)
// arrFaceIndexInOut ���̽��� ���̴� ��ġ���� �ε���,	
// arrTexCoordIn �� �ؽ��� ��ǥ���� 
// arrTFaceIndexIn   ���̽��� ���̴� �ؽ��� ��ǥ�ε��� 
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
	//�����׼����� ���� �Ҵ�
	arrVIndexedTCList.resize(arrVertexInOut.size());

	int nFace=(int)arrVFaceIndexInOut.size();
	for (int iFace=0;iFace<nFace;iFace++)
	{
		TRIANGLE*		 pVFace=&arrVFaceIndexInOut[iFace].triangle;
		const TRIANGLE* pTFace=&arrBaseTFaceIndexIn[iFace];

		for (int i=0;i<3;i++)
		{
			int iRefVertex=pVFace->index[i];					// ���ؽ� �ε���
			int iRefTexCoord=arrBaseTFaceIndexIn[iFace].index[i];	//�ؽ���ǥ �ε���
			const TEXCOORD* pTexcoord=&arrBaseTexCoordIn[iRefTexCoord]; //�ؽ���ǥ ���

			if(arrVIndexedTCList[iRefVertex].empty())			// ���ؽ��� �ؽ���ǥ ����Ʈ���� ������
			{
				if (bBaseMapChannel) 
					arrVertexInOut[iRefVertex].uv0 = *pTexcoord;	// ���ؽ�����Ʈ�� ���׽��� �ؽ���ǥ �߰�
				else
					arrVertexInOut[iRefVertex].uv1 = *pTexcoord;	// ���ؽ�����Ʈ�� ���׽��� �ؽ���ǥ �߰�
				

				std::pair<TEXCOORD,WORD> temp;
				temp.first = *pTexcoord;
				temp.second = iRefVertex;
				arrVIndexedTCList[iRefVertex].push_back(temp); //����Ʈ�� �ؽ���ǥ�߰�
			}
			else // ���ؽ��ε����� �ؽ���ǥ ����Ʈ�� �ϳ��̻� ����ִ�.
			{				
				std::list<std::pair<TEXCOORD,WORD>>* pListTexcoordInsert=&arrVIndexedTCList[iRefVertex];
				std::list<std::pair<TEXCOORD,WORD>>::iterator it=pListTexcoordInsert->begin();
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

					if (bBaseMapChannel) 
						vertex.uv0 = *pTexcoord;	// ���ؽ�����Ʈ�� ���׽��� �ؽ���ǥ �߰�
					else
						vertex.uv1 = *pTexcoord;	// ���ؽ�����Ʈ�� ���׽��� �ؽ���ǥ �߰�

					arrVertexInOut.push_back(vertex);					

					//���ο� �ε���
					iRefVertex=(int)arrVertexInOut.size()-1;

					//���̽� �ε��� ����
					pVFace->index[i]=iRefVertex;					

					//����Ʈ ����,�ؽ��߰�
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





// ȸ��Ű������ ������ȯ
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
		D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//���ʹϾ� ����
		Item.RotationAccum=accum_q;
		prev_q=accum_q;
	}
}

// ȸ��Ű������ ������ȯ
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
		D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//���ʹϾ� ����
		Item.second=accum_q;
		prev_q=accum_q;
	}
}


cMeshNode* 
cASEParser::CreateMeshNode(bool bSkeleton,SCENENODEINFO& stInfo,
							cRscVertexBuffer* pVertexBuffer,
							cRscIndexBuffer* pIndexBuffer,
							std::map<SUBMATINDEX,WORD>& mapIndexCount,
							int nMaterialRef)
{
	assert(pIndexBuffer!=NULL);
	assert(pVertexBuffer!=NULL);

	cMeshNode* pNewSceneNode = NULL; 
	if (bSkeleton) pNewSceneNode = new Skeleton;
	else pNewSceneNode = new cMeshNode;	

	//�������� ������
	if(stInfo.pParent == NULL)
		stInfo.pParent = m_pSceneRoot;

	stInfo.pParent->AttachChildNode(pNewSceneNode);
	SetNodeInfo(pNewSceneNode,stInfo);
	
	if (bSkeleton)
	{
		m_pSceneRoot->InsertBone(pNewSceneNode);
	}	

	if (m_pEntityMaterial->GetRefCounter()==0)
	{
		SceneMaterial* pSceneMaterial = m_pEntityMaterial->CreateSceneMaterial(stInfo.strNodeName);
		pSceneMaterial->m_container = m_vecMaterial[nMaterialRef];
	}

	pNewSceneNode->SetRscVertextBuffer(pVertexBuffer);		
	pNewSceneNode->SetRscIndexBuffer(pIndexBuffer);

	int primitiveCount=0,startIndex=0;
	SUBMATINDEX matIndex=0;
	if ( m_vecMaterial[nMaterialRef].size() == 1)
	{
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nCount = (*it).second;
			primitiveCount += nCount;			
		}		
		pNewSceneNode->AddMultiSub(startIndex,primitiveCount,matIndex);	
	}
	else
	{		
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			matIndex = (*it).first;
			primitiveCount = (*it).second;			
			pNewSceneNode->AddMultiSub(startIndex,primitiveCount,matIndex);
			startIndex += primitiveCount*3; //cnt
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

	//�������� ������
	if(stInfo.pParent == NULL)
		stInfo.pParent = m_pSceneRoot;

	
	SetNodeInfo(pNewSceneNode,stInfo);
	stInfo.pParent->AttachChildNode(pNewSceneNode);

	//pNewSceneNode->SetMaterial(m_vecMaterial[nMaterialRef]);		
	if (m_pEntityMaterial->GetRefCounter()==0)
	{
		SceneMaterial* pSceneMaterial = m_pEntityMaterial->CreateSceneMaterial(stInfo.strNodeName);
		pSceneMaterial->m_container = m_vecMaterial[nMaterialRef];
	}
	pNewSceneNode->SetRscVertextBuffer(pVertexBuffer);		
	pNewSceneNode->SetRscIndexBuffer(pIndexBuffer);
	pNewSceneNode->SetBoneRef(boneRef);
	

	int primitiveCount=0,startIndex=0;
	SUBMATINDEX matIndex=0;
	if ( m_vecMaterial[nMaterialRef].size() == 1)
	{
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			int nCount = (*it).second;
			primitiveCount += nCount;			
		}		
		pNewSceneNode->AddMultiSub(startIndex,primitiveCount,matIndex);	
	}
	else
	{		
		std::map<SUBMATINDEX,WORD>::iterator it;		
		for (it=mapIndexCount.begin() ; it!=mapIndexCount.end(); ++it )
		{
			matIndex = (*it).first;
			primitiveCount = (*it).second;			
			pNewSceneNode->AddMultiSub(startIndex,primitiveCount,matIndex);
			startIndex += primitiveCount*3; //cnt
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
		pVertexBuffer = cResourceMng::m_pInstance->CreateRscVertexBuffer(m_SceneTime.FILENAME.c_str(),meshName,sizeof(T)*nCount);

		if (pVertexBuffer->GetRefCounter()==0)
		{
			T* pVertices=(T*)pVertexBuffer->Lock();
			for (UINT i=0;i< nCount;i++)
			{
				memcpy(&pVertices[i],&arrVertex[i],sizeof(T));
			}	
			pVertexBuffer->Unlock();			
			pVertexBuffer->SetCount(nCount);
		}
	}
	return pVertexBuffer;
}


cRscIndexBuffer* cASEParser::CreateRscIndexBuffer(const char* meshName,std::vector<TRIANGLE_SUBMATERIAL>& arrIndex)
{
	cRscIndexBuffer* pIndexBuffer=NULL;
	if (!arrIndex.empty())
	{
		DWORD nCount=(DWORD)arrIndex.size();
		pIndexBuffer = cResourceMng::m_pInstance->CreateRscIndexBuffer(m_SceneTime.FILENAME.c_str(),meshName,
			sizeof(TRIANGLE)*nCount);

		if (pIndexBuffer->GetRefCounter()==0)
		{
			TRIANGLE* pIndices=(TRIANGLE*)pIndexBuffer->Lock();
			for (UINT i=0;i< nCount;i++)
			{
				memcpy(&pIndices[i],&arrIndex[i].triangle,sizeof(TRIANGLE));			
			}
			pIndexBuffer->Unlock();		
			pIndexBuffer->SetCount(nCount);
		}
	}			
	return pIndexBuffer;
}

cSceneNode* cASEParser::CreateSceneNode(SCENENODEINFO& stInfo)
{
	cSceneNode* pNewSceneNode=new cSceneNode;	
	m_pLastObject = pNewSceneNode;

	//�������� ������
	if(stInfo.pParent == NULL)
		stInfo.pParent = m_pSceneRoot;

	SetNodeInfo(pNewSceneNode,stInfo);
	stInfo.pParent->AttachChildNode(pNewSceneNode);	
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
	m_SceneTime.FILENAME.clear();
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
						TRACE1("MAP_REFLECT: %s �����̾����ϴ�.\n",strFullPath.c_str());

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
							TRACE1("MAP_REFRACT: %s �����̾����ϴ�.\n",strFullPath.c_str());
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
					//ASSERT(m_Token!=TOKEND_BLOCK_START);
					switch(m_Token)
					{
					case TOKENR_BITMAP:

						std::string strFileName=GetString().c_str();							
						std::string strDataPath=EnvironmentVariable::GetInstance().GetString("DataPath");
						std::string strFullPath = strDataPath;
						strFullPath += strFileName;

						cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
						if(pRscTexture==NULL)
							TRACE1("MAP_BUMP: %s �����̾����ϴ�.\n",strFullPath.c_str());
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

						cRscTexture* pRscTexture= cResourceMng::m_pInstance->CreateRscTexture(strFullPath.c_str());
						if(pRscTexture==NULL)
							TRACE1("MAP_DIFFUSE: %s �����̾����ϴ�.\n",strFullPath.c_str());
						material.SetMapDiffuse(pRscTexture);
						break;
					}
				}									
			}
			break;													
		case TOKENR_NUMSUBMTLS:
			{
				ASSERT(0&&"Matrial�� SubMatrial���� SubMatrial�� �Ǵٽ� ����ϰ�����");
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
	pNode->SetParentNode(stInfo.pParent);
	pNode->SetNodeTM(stInfo.tmNode);
}

SceneAnimation* cASEParser::GetSceneAnimation(const char* meshName,const D3DXMATRIX& localTM )
{
	SceneAnimation* pSceneAnimation = m_pEntityAnimation->CreateSceneAnimation(std::string(meshName));
	if ( !pSceneAnimation->m_arrayANMKEY.empty() )
	{
		SkipBlock();
		return pSceneAnimation;	
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

					dwTimeKey =  DWORD((float)GetInt() / m_SceneTime.EX_TICKSPERMS);			

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
					dwTimeKey =  DWORD((float)GetInt() / m_SceneTime.EX_TICKSPERMS);			
					GetVector3(&axis);

					std::pair<DWORD,D3DXQUATERNION> ItemDelta;
					ItemDelta.first = dwTimeKey;
					ang = GetFloat();					
					D3DXQuaternionRotationAxis(&ItemDelta.second,&axis,ang);
					arrayROTKEY.push_back(ItemDelta);
				}		

				// ȸ�� ��ȭ�� ���� ���� ȸ�������ͷ� �ٲ۴�.
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
					D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//���ʹϾ� ����
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

	assert(dwTimeKey== (DWORD)m_SceneTime.EX_LASTFRAMEMS);
	std::vector<ANMKEY>& refArrAnmKey=pSceneAnimation->m_arrayANMKEY;

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

	return pSceneAnimation;
}



}