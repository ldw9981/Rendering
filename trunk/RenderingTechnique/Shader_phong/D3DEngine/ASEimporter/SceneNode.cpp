#include "StdAfx.h"
#include "SceneNode.h"
#include "ASELexer.h"
#include "ASEParser.h"
#include "Scene.h"
#include "Parser.h"
#include "../Log/Trace.h"
#include "../Framework/D3DFramework.h"


cSceneNode::cSceneNode(void)
:m_strNodeName(""),m_strParentName("")
{
	
	m_pRoot=NULL;
	m_pParent=NULL;
	

	m_NodeType=NONE;
	m_IndexANMTM=0.0f;
	m_CNTANMKEY=0;
	m_AnmTickEnd=0;
	m_iANMTM=0;
	m_AccumMSTick=0;
	m_bIsActiveAnimation=FALSE;
	
	D3DXMatrixIdentity(&m_NodeTM);				
	
	D3DXMatrixIdentity(&m_WorldTM);		
	D3DXMatrixIdentity(&m_LocalTM);		


	
}

cSceneNode::~cSceneNode(void)
{
	for_each(m_arrayPOSKEY.begin(),m_arrayPOSKEY.end(),FuncDeleteType<POSKEY*>);
	m_arrayPOSKEY.clear();	

	for_each(m_arrayROTKEY.begin(),m_arrayROTKEY.end(),FuncDeleteType<ROTKEY*>);
	m_arrayROTKEY.clear();	

	for_each(m_arraySCLKEY.begin(),m_arraySCLKEY.end(),FuncDeleteType<SCLKEY*>);
	m_arraySCLKEY.clear();	

	for_each(m_arrayANMKEY.begin(),m_arrayANMKEY.end(),FuncDeleteType<ANMKEY*>);
	m_arrayANMKEY.clear();	

	for_each(m_arrayANMTM.begin(),m_arrayANMTM.end(),FuncDeleteType<ANMTM*>);
	m_arrayANMTM.clear();	
}

void cSceneNode::SetNodeName( LPCSTR nodename )
{
	m_strNodeName=nodename;
}

string& cSceneNode::GetNodeName()
{
	return m_strNodeName;
}

void cSceneNode::SetParentName( LPCSTR nodename )
{
	m_strParentName=nodename;
}

string& cSceneNode::GetParentName()
{
	return m_strParentName;
}

BOOL cSceneNode::LoadObjInfo( cASEParser* pParser )
{
	LONG	token;
	CHAR	TokenString[MAX_PARSER_BUFFERSIZE];

	token=pParser->GetToken(TokenString);
	pParser->SkipBlock();	
	return TRUE;
}




BOOL cSceneNode::IsExistParentName()
{
	if (m_strParentName.empty())
	{
		return FALSE;
	}
	return TRUE;
}

void cSceneNode::InsertChildObject( cSceneNode* pItem )
{
	m_listChildObject.push_back(pItem);
}

cSceneNode* cSceneNode::FindChildObject( string& nodename,int type/*=ANYTHING*/ )
{
	if ( (type==ANYTHING)||(type==m_NodeType) )
	{
		// 자신이면 자신의 주소 리턴
		if( nodename.compare(m_strNodeName)==0)
			return this;
	}

	// 아니면 자식에서 검색
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	cSceneNode* pFoundItem=NULL;
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pFoundItem=pItem->FindChildObject(nodename);
		if (pFoundItem!=NULL)
		{
			return pFoundItem;
		}			 
	}	
	return NULL;
}

void cSceneNode::Init()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Init();
	}	
}

void cSceneNode::Uninit()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Uninit();
	}	
}

void cSceneNode::Update()
{
	if (GetIsActiveAnimation())
	{
		SetAnimationTM();
		m_LocalTM=GetAnimationTM();
	}
	
	if (IsParentObject())
	{		
		m_WorldTM = m_LocalTM;	
	}
	else
	{			
		m_WorldTM = m_LocalTM * GetParentPtr()->GetWorldTM();
	}
	UpdateChildren();
}

void cSceneNode::Render()
{
	RenderChildren();
}

D3DXMATRIX& cSceneNode::GetWorldInverseTM()
{
	D3DXMatrixInverse(&m_WorldInverseTM,NULL,&m_WorldTM);
	return m_WorldInverseTM;
}



BOOL cSceneNode::IsParentObject()
{
	if (m_pParent!=NULL)
	{		
		return FALSE;
	}
	return TRUE;
}

BOOL cSceneNode::LoadANM( cASEParser* pParser )
{
	LONG Token;
	CHAR TokenString[256];
	BOOL bCount=TRUE;	

	Token=pParser->GetToken(TokenString);
	if (Token!=TOKEND_BLOCK_START)
		return FALSE;

	while(Token=pParser->GetToken(TokenString),Token!=TOKEND_BLOCK_END)
	{
		switch(Token)
		{
		case TOKENR_NODE_NAME:
			{

			}
			break;
		case TOKENR_CONTROL_POS_TRACK:
			{
				if(pParser->GetToken(TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				if (m_CNTANMKEY!=0)	bCount=FALSE;
				
				while(Token=pParser->GetToken(TokenString),Token!=TOKEND_BLOCK_END)
				{
					if(Token!=TOKENR_CONTROL_POS_SAMPLE)
						return FALSE;										
					
					POSKEY* pNewItem=new POSKEY;
					pNewItem->AnmTick = pParser->GetInt();
					pParser->GetVector3(&pNewItem->TranslationAccum);
					m_arrayPOSKEY.push_back(pNewItem);			
	
					
					if (bCount)	m_CNTANMKEY++;
					if (pNewItem->AnmTick > m_AnmTickEnd )	m_AnmTickEnd=pNewItem->AnmTick;
				}
			}
			break;
		case TOKENR_CONTROL_ROT_TRACK:
			{
				if(pParser->GetToken(TokenString)!=TOKEND_BLOCK_START)
					return FALSE;
	
				if (m_CNTANMKEY!=0)	bCount=FALSE;

				while(Token=pParser->GetToken(TokenString),Token!=TOKEND_BLOCK_END)
				{	
					if(Token!=TOKENR_CONTROL_ROT_SAMPLE)
						return FALSE;	
					
					float ang;
					D3DXVECTOR3 axis;
					ROTKEY* pNewItem=new ROTKEY;
					pNewItem->AnmTick = pParser->GetInt();				
					
					pParser->GetVector3(&axis);
					ang = pParser->GetFloat();					

					D3DXQuaternionRotationAxis(&pNewItem->RotationAccum,&axis,ang);
					m_arrayROTKEY.push_back(pNewItem);

					if (bCount)	m_CNTANMKEY++;
					if (pNewItem->AnmTick > m_AnmTickEnd  )	m_AnmTickEnd=pNewItem->AnmTick;					
				}
			}
			break;
		case TOKENR_CONTROL_SCALE_TRACK:
			{				
				if(pParser->GetToken(TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

				while(Token=pParser->GetToken(TokenString),Token!=TOKEND_BLOCK_END)
				{
					
				}
			}
			break;
		}
	} 
	
	return TRUE;
}



float cSceneNode::GetInterpolateValue( int start_time,int end_time,int inter_time )
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

void cSceneNode::CalculateANMKEY(int interpolate_time)
{
	D3DXMATRIX SCLTM,ROTTM,POSTM;
	D3DXMatrixIdentity(&SCLTM);
	D3DXMatrixIdentity(&ROTTM);
	D3DXMatrixIdentity(&POSTM);
	
	ANMKEY LocalTM_anmkey;
	D3DXMatrixDecompose(&LocalTM_anmkey.ScaleDelta,&LocalTM_anmkey.RotationDelta,&LocalTM_anmkey.TranslationDelta,&m_LocalTM);
	
	D3DXMatrixScaling(&SCLTM,LocalTM_anmkey.ScaleDelta.x,LocalTM_anmkey.ScaleDelta.y,LocalTM_anmkey.ScaleDelta.z);
	D3DXMatrixRotationQuaternion(&ROTTM,&LocalTM_anmkey.RotationDelta);
	D3DXMatrixTranslation(&POSTM,LocalTM_anmkey.TranslationDelta.x,LocalTM_anmkey.TranslationDelta.y,LocalTM_anmkey.TranslationDelta.z);

	
	//1. 회전키정보의 누적변환
	D3DXQUATERNION curr_q, prev_q, accum_q;
	D3DXQuaternionIdentity(&curr_q);
	D3DXQuaternionIdentity(&prev_q);
	D3DXQuaternionIdentity(&accum_q);
	
	vector<ROTKEY*>::iterator rot_it=m_arrayROTKEY.begin();
	for ( ; rot_it!=m_arrayROTKEY.end() ; rot_it++)
	{
		ROTKEY* pItem;		
		pItem= *rot_it;

		if(pItem->AnmTick == 0)
		{
			pItem->RotationAccum=LocalTM_anmkey.RotationDelta;			
		}
		curr_q = pItem->RotationAccum;				
		D3DXQuaternionMultiply(&accum_q,&accum_q,&curr_q);//쿼터니언 누적
		pItem->RotationAccum=accum_q;
		prev_q=accum_q;
	}

	//2. 보간을 고려한 ANMKEY 전체 크기구하기
	m_CNTANMTM	= (m_AnmTickEnd/interpolate_time)+1;	
	
	if(m_CNTANMTM==0)
		return;

	//3. ANMKEY의 할당
	m_arrayANMKEY.reserve(m_CNTANMTM);
	for ( int i=0;i<m_CNTANMTM;i++)
	{		
		ANMKEY* pNewItem=new ANMKEY;
		memset(pNewItem,0,sizeof(ANMKEY));
		m_arrayANMKEY.push_back(pNewItem);	
	}	

	
	int iANMKEY;
	DWORD accum_AnmTick=0;	
	//4. SCL보간하여 ANMKEY에 넣기	
	for (int i=0;i<m_CNTANMKEY;i++)
	{	
		if (m_arraySCLKEY.size()==0)
			break;
	}
	
	//5. ROT보간하여 ANMKEY에 넣기
	ROTKEY prev_rotkey,curr_rotkey,temp_rotkey;
	memset(&prev_rotkey,0,sizeof(prev_rotkey));	
	memset(&curr_rotkey,0,sizeof(curr_rotkey));
	memset(&temp_rotkey,0,sizeof(temp_rotkey));

	for (int i=0;i<(int)m_arrayROTKEY.size();i++)
	{
		curr_rotkey.RotationAccum = m_arrayROTKEY[i]->RotationAccum;
		curr_rotkey.AnmTick = m_arrayROTKEY[i]->AnmTick;

		accum_AnmTick=prev_rotkey.AnmTick;

		

		while (accum_AnmTick < curr_rotkey.AnmTick)
		{

			if (accum_AnmTick+interpolate_time > curr_rotkey.AnmTick)
				break;

			accum_AnmTick += interpolate_time;


			temp_rotkey.AnmTick=accum_AnmTick;
			float t=GetInterpolateValue(prev_rotkey.AnmTick,curr_rotkey.AnmTick,accum_AnmTick);						
			D3DXQuaternionSlerp(&temp_rotkey.RotationAccum,&prev_rotkey.RotationAccum,&curr_rotkey.RotationAccum,t);
	
			iANMKEY=accum_AnmTick/interpolate_time;
			m_arrayANMKEY[iANMKEY]->RotationDelta =temp_rotkey.RotationAccum;
			m_arrayANMKEY[iANMKEY]->AnmTick = temp_rotkey.AnmTick;
		}

		iANMKEY=curr_rotkey.AnmTick/interpolate_time;
		m_arrayANMKEY[iANMKEY]->RotationDelta =curr_rotkey.RotationAccum;
		m_arrayANMKEY[iANMKEY]->AnmTick = curr_rotkey.AnmTick;
		
		prev_rotkey = curr_rotkey;

	}

	//6. POS보간하여 ANMKEY에 넣기
	POSKEY prev_poskey,temp_poskey,curr_poskey;	
	memset(&prev_poskey,0,sizeof(prev_poskey));
	memset(&curr_poskey,0,sizeof(curr_poskey));
	memset(&temp_poskey,0,sizeof(temp_poskey));

	for (int i=0;i<(int)m_arrayPOSKEY.size();i++)
	{		
		curr_poskey.TranslationAccum = m_arrayPOSKEY[i]->TranslationAccum;
		curr_poskey.AnmTick = m_arrayPOSKEY[i]->AnmTick;
			

		accum_AnmTick=prev_poskey.AnmTick;

		while (accum_AnmTick < curr_poskey.AnmTick)
		{						
			
			if (accum_AnmTick+interpolate_time > curr_poskey.AnmTick)
				break;

			accum_AnmTick += interpolate_time;

			temp_poskey.AnmTick=accum_AnmTick;
			float s=GetInterpolateValue(prev_poskey.AnmTick,curr_poskey.AnmTick,accum_AnmTick);
			D3DXVec3Lerp(&temp_poskey.TranslationAccum,&prev_poskey.TranslationAccum,&curr_poskey.TranslationAccum,s);

			iANMKEY=accum_AnmTick/interpolate_time;
			m_arrayANMKEY[iANMKEY]->TranslationDelta = temp_poskey.TranslationAccum;
			m_arrayANMKEY[iANMKEY]->AnmTick = temp_poskey.AnmTick;
		}				

		iANMKEY=curr_poskey.AnmTick/interpolate_time;
		m_arrayANMKEY[iANMKEY]->TranslationDelta = curr_poskey.TranslationAccum;
		m_arrayANMKEY[iANMKEY]->AnmTick = curr_poskey.AnmTick;
		
		prev_poskey=curr_poskey;
		
	}

	//7. 보간값까지 저장된 ANMYKEY를 ANMTM으로 바꾸기
	vector<ANMKEY*>::iterator it=m_arrayANMKEY.begin();
	for ( ; it!=m_arrayANMKEY.end() ; it++)
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

		ANMTM* pNewItem=new ANMTM;		
		pNewItem->TM = SCLTM * ROTTM * POSTM;
		pNewItem->AnmTick = p->AnmTick;
		m_arrayANMTM.push_back(pNewItem);
	}

	
	if (!m_arrayANMTM.empty())
	{
		m_bIsActiveAnimation=TRUE;
	}
	
}





/*
	
*/
void cSceneNode::SetAnimationTM()
{	

	if (m_arrayANMTM.empty())
		return;

		if ( g_pD3DFramework->GetPrevFrameTick() !=0 )
		{		
			m_AccumMSTick +=  g_pD3DFramework->GetCurrFrameTick() - g_pD3DFramework->GetPrevFrameTick() ;
		}		
		
		

		m_AnimationTM = m_arrayANMTM[m_iANMTM]->TM ;				
	
			
	
		//현재 AnmTick보다 같거나크면 인덱스증가
		DWORD index_AnmTick = m_arrayANMTM[m_iANMTM]->AnmTick;		
		DWORD curr_AnmTick =  m_AccumMSTick * m_pScene->GetSceneInfo().EX_TICKSPERMS;

		//현재 인덱스의 AnmTick과 같거나 크면
		if (curr_AnmTick >= index_AnmTick)
		{			
			do 
			{	// 인덱스를 증가시키고
				m_iANMTM++;
				if (m_iANMTM >= m_CNTANMTM )
				{
					m_iANMTM=m_CNTANMTM-1;
					if (m_AccumMSTick >= m_pScene->GetSceneInfo().LASTFRAMEMS)
					{
			//			TRACE3(_T("%s , m_AccumMSTick=%d , %d\n"),GetNodeName().c_str(), m_AccumMSTick,m_pScene->GetSceneInfo().LASTFRAMEMS);
						m_iANMTM=0;
						m_AccumMSTick=0;			
						curr_AnmTick =  m_AccumMSTick * m_pScene->GetSceneInfo().EX_TICKSPERMS;

						
					}		
					break;
				}	
				// 인덱스 AnmTick을 얻어 다시비교한다.
				index_AnmTick = m_arrayANMTM[m_iANMTM]->AnmTick;							
			} while(curr_AnmTick > index_AnmTick);
		}
/*
		//현재 인덱스의 AnmTick과 같거나 크면
		if (curr_AnmTick >= index_AnmTick)
		{			
			do 
			{	// 인덱스를 증가시키고
				m_iANMTM++;
				if (m_iANMTM >= m_CNTANMTM )
				{
					m_iANMTM=0;
					m_AccumMSTick=0;					
					curr_AnmTick =  m_AccumMSTick * m_pScene->GetSceneInfo().TICKSPERMS;
				}	
				// 인덱스 AnmTick을 얻어 다시비교한다.
				index_AnmTick = m_arrayANMTM[m_iANMTM]->AnmTick;							
			} while(curr_AnmTick > index_AnmTick);
		}
*/		
}

void cSceneNode::UpdateChildren()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	cSceneNode* pFoundItem=NULL;
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Update();
	}	
}

void cSceneNode::RenderChildren()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	cSceneNode* pFoundItem=NULL;
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Render();
	}	
}

void cSceneNode::InitChildren()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;	
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Init();
	}	
}

void cSceneNode::UninitChildren()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;	
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Uninit();
	}	
}

BOOL cSceneNode::LinkToParent()
{
	// 부모노드를 찾아서 자식으로 넣는다.
	ASSERT(m_pRoot!=NULL);
	cSceneNode* pFoundObject=NULL;
	if (IsExistParentName())
	{		
		pFoundObject=m_pRoot->FindChildObject(GetParentName());
		if (pFoundObject!=NULL)
		{
			pFoundObject->InsertChildObject(this);
			//부모주소를 넣는다.
			SetParentPtr(pFoundObject);
		}			
		else
		{
			ASSERT(pFoundObject!=NULL);
			return FALSE;
		}
	}
	if (pFoundObject==NULL)
	{
		m_pRoot->InsertChildObject(this);
	}
	return TRUE;
}



D3DXMATRIX& cSceneNode::MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ )
{
	D3DXMatrixTranslation(&m_TranslationTM,deltaX,deltaY,deltaZ);
	m_LocalTM =  m_TranslationTM * m_LocalTM;
	return m_TranslationTM;
}

D3DXMATRIX& cSceneNode::RotateByLocalAxis( float angleX,float angleY,float angleZ )
{
	D3DXMatrixRotationYawPitchRoll(&m_RotationTM,D3DXToRadian(angleY),D3DXToRadian(angleX),D3DXToRadian(angleZ));
	m_LocalTM =  m_RotationTM * m_LocalTM;
	return m_RotationTM;
}

D3DXMATRIX& cSceneNode::GetAnimationTM()
{
	return m_AnimationTM;
}

void cSceneNode::SetPosition( D3DXVECTOR3& pos )
{
	m_LocalTM._41 =  pos.x; 
	m_LocalTM._42 =  pos.y; 
	m_LocalTM._43 =  pos.z; 
}

void cSceneNode::GetPosition( D3DXVECTOR3& pos )
{
	pos.x = m_LocalTM._41;   
	pos.y = m_LocalTM._42;  
	pos.z = m_LocalTM._43;  
}