#include "stdafx.h"
#include "../Log/Trace.h"

#include "ASEParser.h"
#include "ASELexer.h"
#include "Lexer.h"



#include "MeshNode.h"
#include "HelperNode.h"
#include "ShapeNode.h"
#include "CameraNode.h"
#include "LightNode.h"
#include "Scene.h"
#include "../Framework/D3DFramework.h"


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
	m_token			 = GetToken(m_TokenString);	ASSERT(m_token == TOKEND_NUMBER);
	tNumber		 = (float)atof(m_TokenString);
	return			tNumber;
}

int cASEParser::GetInt()
{
	int				tNumber;
	m_token			 = GetToken(m_TokenString);	ASSERT(m_token == TOKEND_NUMBER);
	tNumber		 = atoi(m_TokenString);
	return			tNumber;	
}

long cASEParser::GetLong()
{
	LONG			tNumber;
	m_token			 = GetToken(m_TokenString);	ASSERT(m_token == TOKEND_NUMBER);
	tNumber		 = strtoul(m_TokenString, NULL, 10);
	return			tNumber;
}

BOOL cASEParser::GetVector3(D3DXVECTOR3* pOutput)
{	
	m_token			 = GetToken(m_TokenString);		ASSERT(m_token == TOKEND_NUMBER);
	if(pOutput!=NULL)	pOutput->x	 = (float)atof(m_TokenString);
	m_token			 = GetToken(m_TokenString);		ASSERT(m_token == TOKEND_NUMBER);
	if(pOutput!=NULL)	pOutput->z	 = (float)atof(m_TokenString);
	m_token			 = GetToken(m_TokenString);		ASSERT(m_token == TOKEND_NUMBER);
	if(pOutput!=NULL)	pOutput->y	 = (float)atof(m_TokenString);
	return			TRUE;
}


string& cASEParser::GetString()
{	
	static string temp;
	// string
	m_token=GetToken(m_TokenString);
	if (m_token!=TOKEND_STRING)
	{
		ASSERT(0&&_T("m_token!=TOKEND_STRING"));	
	}	
	temp=m_TokenString;
	return temp;
}

wstring& cASEParser::GetWString()
{
	static wstring temp;
	m_token=GetToken(m_TokenString);
	if (m_token!=TOKEND_STRING)
	{
		ASSERT(0&&_T("m_token!=TOKEND_STRING"));	
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
		m_token =  GetToken(m_TokenString);

		if (m_token==TOKEND_BLOCK_START)
			level++;
		else if (m_token==TOKEND_BLOCK_END)
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
	do 
	{
		m_token =  GetToken(m_TokenString);

		if(m_token==token)
		{
			return TRUE;
		}

	} while (m_token!=TOKEND_END );
	return FALSE;
}

LONG cASEParser::GetNextASEToken()
{
	do 
	{
		m_token =  GetToken(m_TokenString);

		if(m_token < TOKENR_MAX )
		{
			return m_token;
		}

	} while (m_token!=TOKEND_END );
	return TOKEND_END;
}

BOOL cASEParser::GetNodeTM( D3DXMATRIX &output )
{
	m_token =  GetToken(m_TokenString);
	if (m_token != TOKEND_BLOCK_START)
		return FALSE;

	D3DXMatrixIdentity(&output);
	int row;
	for (int i=0;i<4;i++)
	{			
		if (!FindToken(TOKENR_TM_ROW0+i))
			return FALSE;

		D3DXVECTOR3 tVector3;
		if(!GetVector3(&tVector3))
			return FALSE;

		if (i==1) row=2;
		else if (i==2) row=1;
		else row=i;				
		output.m[row][0]=tVector3.x;
		output.m[row][1]=tVector3.y;
		output.m[row][2]=tVector3.z;
	}		

	if(!FindToken(TOKEND_BLOCK_END))
		return FALSE;

	return TRUE;
}

BOOL cASEParser::GetMatrix(D3DXMATRIX *output )
{
	int row;
	for (int i=0;i<4;i++)
	{			
		if (!FindToken(TOKENR_TM_ROW0+i))
		{
			return FALSE;
		}
		D3DXVECTOR3 tVector3;
		if(!GetVector3(&tVector3))
			return FALSE;

		if (i==1) row=2;
		else if (i==2) row=1;
		else row=i;				
		output->m[row][0]=tVector3.x;
		output->m[row][1]=tVector3.y;
		output->m[row][2]=tVector3.z;
	}		
	return	TRUE;
}

BOOL cASEParser::GetIdentifier( LPSTR pOutput )
{
	m_token=GetToken(m_TokenString);
	if (m_token != TOKEND_IDENTIFIER)
	{
		ASSERT(0&&_T("m_token!=TOKEND_IDENTIFIER"));
		return FALSE;
	}
	if (pOutput!=NULL)
	{
		memcpy(pOutput,m_TokenString,strlen(m_TokenString));
	}

	return TRUE;
}

BOOL cASEParser::Load( LPCTSTR strFileName ,cScene* pOutput)
{	
	BOOL bResult;


	m_pScene = pOutput;

	if (!Open(strFileName))
	{
		return FALSE;
	}

	m_token =  GetToken(m_TokenString);
	if(m_token == TOKENR_3DSMAX_ASCIIEXPORT)
	{
		// - version 정보
		//m_version	 = Parser.GetFloat();
		SkipBlock();
	}
	else
	{
		TRACE(_T("TOKENR_3DSMAX_ASCIIEXPORT 얻기실패\n"));
		return FALSE;
	}

	// - Comment는 여러 줄이 될 수가 있기 때문에...
	while(m_token = GetToken(m_TokenString), m_token == TOKENR_COMMENT)
	{
		// - Comment는 그냥 무시한다.
		m_token = GetToken(m_TokenString);
	}

	// 2) Scene Info
	if(m_token == TOKENR_SCENE)
	{
		Parsing_Scene();		
	}
	// 3) Material List
	//   - Material List는 무조건 있긴 하다.
	m_token = GetToken(m_TokenString);
	if(m_token == TOKENR_MATERIAL_LIST)
	{
		bResult=Parsing_MaterialList();		
		ASSERT(bResult==TRUE);
	}



	// 4) Node Objects
	while(m_token = GetToken(m_TokenString), m_token != TOKEND_END)
	{

		switch(m_token)
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
				TRACE3(_T("정의되지 않은 Command입니다. ( %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__),m_token,strToken );						
			}		
			ASSERT(bResult==TRUE);
			break;
		};

		ASSERT(bResult==TRUE);
	}

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
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	cMeshNode* pNewItem=new cMeshNode;
	m_pLastObject = pNewItem;

	while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
	{
		switch(m_token)
		{
		case TOKENR_NODE_NAME:
			pNewItem->SetNodeName(GetString().c_str());	
			break;
		case TOKENR_NODE_PARENT:
			pNewItem->SetParentName(GetString().c_str());
			break;

		case TOKENR_NODE_TM:
			GetNodeTM(pNewItem->GetNodeTM());		
			break;
		case TOKENR_MESH:
			if(!pNewItem->LoadObjInfo(this))	return FALSE;
			break;		

		case TOKENR_TM_ANIMATION:
			if(!pNewItem->LoadANM(this))	return FALSE;
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
				pNewItem->SetMatrial(*m_pScene->GetMatrial(GetInt()));
			}
			break;
		default:
			{	
				string test="test";
				TCHAR strToken[256]={0,};
				TCHAR strNodeName[256]={0,};	
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),strToken,256);				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pNewItem->GetNodeName().c_str(),pNewItem->GetNodeName().length(),strNodeName,256);			
				TRACE4(_T("정의되지 않은 Command입니다. ( %s ,NodeName= %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__), strNodeName,m_token,strToken );						
			}
			break;
		}
	}	

	m_pScene->InsertSceneNode(pNewItem);				
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
		cMaterialNode Matrial;		
		if(GetToken(m_TokenString) != TOKENR_MATERIAL)		// *MATERIAL	
			return FALSE;			
		iMaterial=GetInt();									// index
		if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
			return FALSE;		

		while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
		{
			switch(m_token)
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
			case TOKENR_MAP_OPACITY:
				FindToken(TOKEND_BLOCK_START);
				FindToken(TOKEND_BLOCK_END);
				break;

			case TOKENR_MAP_REFLECT:
			case TOKENR_MAP_BUMP:
			case TOKENR_MAP_REFRACT:
				FindToken(TOKEND_BLOCK_START);
				FindToken(TOKEND_BLOCK_END);
				break;
			case TOKENR_MAP_DIFFUSE:
				{
					if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
						return FALSE;						
					while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
					{
						switch(m_token)
						{
						case TOKENR_BITMAP:
							cD3DGraphics* pD3DGraphcis=g_pD3DFramework->GetRenderer();
							wstring strFileName=GetWString().c_str();
							cRscTexture* pRscTexture= pD3DGraphcis->m_ResourceMng.CreateRscTexture(strFileName.c_str());
							TRACE1(L"MAP_DIFFUSE: %s 파일이없습니다.",strFileName.c_str());
							Matrial.SetRscTexture(pRscTexture);
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
						cMaterialNode SubMatrial;						
						FindToken(TOKENR_SUBMATERIAL);	// *SUBMATERIAL
						GetInt();						// index
						if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	return FALSE;						
						while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
						{
							switch(m_token)
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
							case TOKENR_MAP_REFLECT:
							case TOKENR_MAP_BUMP:
							case TOKENR_MAP_REFRACT:	
								FindToken(TOKEND_BLOCK_START);
								FindToken(TOKEND_BLOCK_END);
								break;
							case TOKENR_MAP_DIFFUSE:
								{
									if (GetToken(m_TokenString) != TOKEND_BLOCK_START)	
										return FALSE;						
									while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
									{			
										switch(m_token)
										{
										case TOKENR_BITMAP:
											cD3DGraphics* pD3DGraphcis=g_pD3DFramework->GetRenderer();
											cRscTexture* pRscTexture= pD3DGraphcis->m_ResourceMng.CreateRscTexture(GetWString().c_str());
											SubMatrial.SetRscTexture(pRscTexture);
											break;
										}
									}									
								}
								break;							
							case TOKENR_MAP_OPACITY:
								if(!FindToken(TOKEND_BLOCK_START)) return FALSE;	
								if(!FindToken(TOKEND_BLOCK_END)) return FALSE;	
								break;
							case TOKENR_NUMSUBMTLS:
								{
									ASSERT(0&&L"Matrial의 SubMatrial에서 SubMatrial을 또다시 사용하고있음");
								}
								break;
							}						
						}
						//삽입
						Matrial.InsertSubMaterial(SubMatrial);
					}

				}//case TOKENR_NUMSUBMTLS:
				break;		
			}//switch			

		}//while (m_token=GetLexer()->GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)

		m_pScene->InsertMatrial(Matrial);
	}//for (int i=0;i<nNUMMaterial;i++)	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_END) 
		return FALSE;	// }	


	return bRet;
}


BOOL cASEParser::Parsing_HelperObject()
{	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	cHelperNode* pNewItem=new cHelperNode;
	m_pLastObject = pNewItem;

	while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
	{
		switch(m_token)
		{
		case TOKENR_NODE_NAME:
			pNewItem->SetNodeName(GetString().c_str()); 						
			break;
		case TOKENR_NODE_PARENT:
			pNewItem->SetParentName(GetString().c_str());
			break;
		case TOKENR_HELPER_CLASS:
			((cHelperNode*)&(*pNewItem))->SetClassName(GetString().c_str());
			break;
		case TOKENR_NODE_TM:
			if (!GetNodeTM(pNewItem->GetNodeTM())) return FALSE;
			break;		
		case	TOKENR_BOUNDINGBOX_MIN:
			GetVector3(NULL);
			break;
		case	TOKENR_BOUNDINGBOX_MAX:
			GetVector3(NULL);
			break;		
		case TOKENR_TM_ANIMATION:
			if (!pNewItem->LoadANM(this)) return FALSE;					
			break;	
		default:
			{	
				string test="test";
				TCHAR strToken[256]={0,};
				TCHAR strNodeName[256]={0,};	
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),strToken,256);				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pNewItem->GetNodeName().c_str(),pNewItem->GetNodeName().length(),strNodeName,256);			
				TRACE4(_T("정의되지 않은 Command입니다. ( %s ,NodeName= %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__), strNodeName,m_token,strToken );						
			}			
			break;
		}		

	}		
	m_pScene->InsertSceneNode(pNewItem);	
	return TRUE;
}

BOOL cASEParser::Parsing_ShapeObject()
{
	// {
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	cShapeNode*	pNewItem=new cShapeNode;

	m_pLastObject = pNewItem;

	while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
	{
		switch(m_token)
		{
		case TOKENR_NODE_NAME:
			pNewItem->SetNodeName(GetString().c_str()); 						
			break;		
		case TOKENR_NODE_PARENT:
			pNewItem->SetParentName(GetString().c_str());
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
			if (!GetNodeTM(pNewItem->GetNodeTM())) return FALSE;		
			break;		
		case TOKENR_TM_ANIMATION:
			if (!pNewItem->LoadANM(this))	return FALSE;					
			break;	
		default:
			{	
				string test="test";
				TCHAR strToken[256]={0,};
				TCHAR strNodeName[256]={0,};	
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),strToken,256);				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pNewItem->GetNodeName().c_str(),pNewItem->GetNodeName().length(),strNodeName,256);			
				TRACE4(_T("정의되지 않은 Command입니다. ( %s ,NodeName= %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__), strNodeName,m_token,strToken );						
			}
			break;
		}
	}

	m_pScene->InsertSceneNode(pNewItem);		
	return TRUE;
}

BOOL cASEParser::Parsing_LightObject()
{	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;


	cLightNode* pNewItem=new cLightNode;	
	m_pLastObject = pNewItem;

	while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
	{
		switch(m_token)
		{
		case TOKENR_NODE_NAME:
			pNewItem->SetNodeName(GetString().c_str()); 						
			break;
		case TOKENR_LIGHT_TYPE:

			GetIdentifier(NULL);
			break;
		case TOKENR_NODE_PARENT:
			pNewItem->SetParentName(GetString().c_str());
			break;
		case TOKENR_NODE_TM:
			if (!GetNodeTM(pNewItem->GetNodeTM())) return FALSE;		
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
			if (!pNewItem->LoadANM(this))	return FALSE;					
			break;	
		default:
			{	
				string test="test";
				TCHAR strToken[256]={0,};
				TCHAR strNodeName[256]={0,};	
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),strToken,256);				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pNewItem->GetNodeName().c_str(),pNewItem->GetNodeName().length(),strNodeName,256);			
				TRACE4(_T("정의되지 않은 Command입니다. ( %s ,NodeName= %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__), strNodeName,m_token,strToken );						
			}
			break;
		}		
	}	

	m_pScene->InsertSceneNode(pNewItem);		
	return TRUE;
}

BOOL cASEParser::Parsing_CameraObject()
{
	// {
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;


	cCameraNode* pNewItem=new cCameraNode;

	m_pLastObject = pNewItem;

	BOOL bLoadCameraNodeTM=FALSE;
	BOOL bLoadCameraAnmTM=FALSE;
	while (m_token=GetToken(m_TokenString),m_token!=TOKEND_BLOCK_END)
	{
		switch(m_token)
		{
		case TOKENR_NODE_NAME:
			pNewItem->SetNodeName(GetString().c_str());
			break;
		case TOKENR_NODE_PARENT:
			pNewItem->SetParentName(GetString().c_str());
			break;
		case TOKENR_CAMERA_TYPE:
			GetToken(m_TokenString);
			break;
		case TOKENR_NODE_TM:
			if (bLoadCameraNodeTM==FALSE)
			{
				D3DXMATRIX NodeTM,NodeRotation;
				GetNodeTM(NodeTM);		// Camera Node TM

				D3DXMatrixRotationAxis(&NodeRotation,&D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXToRadian(90));
				NodeTM = NodeRotation * NodeTM;

				pNewItem->SetNodeTM(NodeTM);	
				bLoadCameraNodeTM=TRUE;
			}	
			else
			{
				SkipBlock();
			}

			break;
		case TOKENR_CAMERA_SETTINGS:
			if (!pNewItem->LoadObjInfo(this))	return FALSE;				
			break;
		case TOKENR_CAMERA_ANIMATION:
			SkipBlock();
			break;
		case TOKENR_TM_ANIMATION:
			if (bLoadCameraAnmTM==FALSE)
			{
				if(!pNewItem->LoadANM(this))	return FALSE;
				bLoadCameraAnmTM=TRUE;												
			}			
			else
			{
				SkipBlock();
			}
			break;

		default:
			{	
				string test="test";
				TCHAR strToken[256]={0,};
				TCHAR strNodeName[256]={0,};	
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,m_TokenString,strlen(m_TokenString),strToken,256);				
				::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pNewItem->GetNodeName().c_str(),pNewItem->GetNodeName().length(),strNodeName,256);			
				TRACE4(_T("정의되지 않은 Command입니다. ( %s ,NodeName= %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__), strNodeName,m_token,strToken );						

			}	
			break;
		}
	}	

	m_pScene->InsertSceneNode(pNewItem);		

	return TRUE;	
}

BOOL cASEParser::Parsing_Group()
{	
	if (GetToken(m_TokenString)!=TOKEND_BLOCK_START)
		return FALSE;

	while(m_token = GetToken(m_TokenString), m_token != TOKEND_BLOCK_END)
	{
		BOOL bResult;
		switch(m_token)
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
				TRACE3(_T("정의되지 않은 Command입니다. ( %s ,token= %d , tokenstring= %s )\n"), _T(__FUNCTION__),m_token,strToken );						
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

	while(m_token = GetToken(m_TokenString), m_token != TOKEND_BLOCK_END)
	{
		switch(m_token)
		{
		case TOKENR_SCENE_FILENAME:
			m_pScene->GetSceneInfo().FILENAME=GetString();
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_pScene->GetSceneInfo().FIRSTFRAME=GetInt();			
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_pScene->GetSceneInfo().LASTFRAME=GetInt();	
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_pScene->GetSceneInfo().FRAMESPEED=GetInt();	
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_pScene->GetSceneInfo().TICKSPERFRAME=GetInt();	
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
	m_pScene->GetSceneInfo().EX_MSPERFRAME= 1000/m_pScene->GetSceneInfo().FRAMESPEED;
	// 1ms당 틱수 구하기 
	m_pScene->GetSceneInfo().EX_TICKSPERMS= m_pScene->GetSceneInfo().TICKSPERFRAME/m_pScene->GetSceneInfo().EX_MSPERFRAME;
	// 마지막 프레임 ms구하기
	m_pScene->GetSceneInfo().LASTFRAMEMS= m_pScene->GetSceneInfo().LASTFRAME*m_pScene->GetSceneInfo().EX_MSPERFRAME;
	return TRUE;
}