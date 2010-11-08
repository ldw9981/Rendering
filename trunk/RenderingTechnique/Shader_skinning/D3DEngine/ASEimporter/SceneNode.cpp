#include "StdAfx.h"
#include "SceneNode.h"
#include "ASEParser.h"
#include "SceneRoot.h"
#include "Parser.h"
#include "../Log/Trace.h"



cSceneNode::cSceneNode(void)
:m_strNodeName(L""),m_strParentName(L"Root")
{
	
	m_pRoot=NULL;
	m_pParent=NULL;
	

	m_NodeType=NONE;
	
	m_IndexTransformAnm=0;
	m_AccumTime=0;
	m_bIsActiveAnimation=FALSE;
	
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

	for_each(m_arrayTRANSFORMANM.begin(),m_arrayTRANSFORMANM.end(),FuncDeleteType<TRANSFORMANM*>);
	m_arrayTRANSFORMANM.clear();	


	
}

void cSceneNode::SetNodeName( LPCWSTR nodename )
{
	m_strNodeName=nodename;
}

wstring& cSceneNode::GetNodeName()
{
	return m_strNodeName;
}

void cSceneNode::SetParentName( LPCWSTR nodename )
{
	m_strParentName=nodename;
}

wstring& cSceneNode::GetParentName()
{
	return m_strParentName;
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

cSceneNode* cSceneNode::FindChildObject( wstring& nodename,int type/*=ANYTHING*/ )
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

void cSceneNode::Update(DWORD elapseTime)
{
	if (GetIsActiveAnimation())
	{
		UpdateTransformAnm( elapseTime);
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
	UpdateChildren(elapseTime);
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
				
				while(Token=pParser->GetToken(TokenString),Token!=TOKEND_BLOCK_END)
				{
					if(Token!=TOKENR_CONTROL_POS_SAMPLE)
						return FALSE;										
					
					POSKEY* pNewItem=new POSKEY;
					pNewItem->AnmTick = pParser->GetInt();
					pParser->GetVector3(&pNewItem->TranslationAccum);
					m_arrayPOSKEY.push_back(pNewItem);							
				}
			}
			break;
		case TOKENR_CONTROL_ROT_TRACK:
			{
				if(pParser->GetToken(TokenString)!=TOKEND_BLOCK_START)
					return FALSE;

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

/*
	보간이후 생성된 키의 개수를 리턴한다.
*/
UINT cSceneNode::InterpolateTransformAnm(DWORD interpolateTime)
{
	UINT totalInterpolateKey=0;
	DWORD anmEndTick=0,interpolateTick=0;
	if (!m_arraySCLKEY.empty())	anmEndTick = max(anmEndTick, m_arraySCLKEY[m_arraySCLKEY.size()-1]->AnmTick);	
	if (!m_arrayROTKEY.empty())	anmEndTick = max(anmEndTick, m_arrayROTKEY[m_arrayROTKEY.size()-1]->AnmTick);
	if (!m_arrayPOSKEY.empty()) anmEndTick = max(anmEndTick, m_arrayPOSKEY[m_arrayPOSKEY.size()-1]->AnmTick);
	if (anmEndTick==0)
	{
		m_bIsActiveAnimation=FALSE;
		return 0;
	}

	D3DXMATRIX SCLTM,ROTTM,POSTM;
	D3DXMatrixIdentity(&SCLTM);
	D3DXMatrixIdentity(&ROTTM);
	D3DXMatrixIdentity(&POSTM);
	
	ANMKEY LocalTM_anmkey;
	D3DXMatrixDecompose(
		&LocalTM_anmkey.ScaleDelta,
		&LocalTM_anmkey.RotationDelta,
		&LocalTM_anmkey.TranslationDelta,
		&m_LocalTM);
	
	D3DXMatrixScaling(&SCLTM,
		LocalTM_anmkey.ScaleDelta.x,
		LocalTM_anmkey.ScaleDelta.y,
		LocalTM_anmkey.ScaleDelta.z);
	D3DXMatrixRotationQuaternion(&ROTTM,
		&LocalTM_anmkey.RotationDelta);
	D3DXMatrixTranslation(&POSTM,
		LocalTM_anmkey.TranslationDelta.x,
		LocalTM_anmkey.TranslationDelta.y,
		LocalTM_anmkey.TranslationDelta.z);

	
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

	//2. 보간을 고려한 TRANSFORMANM 전체 크기구하기
	interpolateTick = interpolateTime* GetRoot()->GetSceneInfo().EX_TICKSPERMS;
	totalInterpolateKey  = anmEndTick / interpolateTick;
	if(totalInterpolateKey==0)	// totalANNM이 0이면 애니메이션의 의미가없다.
	{
		m_bIsActiveAnimation=FALSE;
		return 0;
	}
	// 할당을 위한 크기계산
	totalInterpolateKey	+= 1;		
	

	//3. ANMKEY의 할당	
	for ( UINT i=0;i<totalInterpolateKey;i++)
	{		
		ANMKEY* pNewItem=new ANMKEY;
		memset(pNewItem,0,sizeof(ANMKEY));
		m_arrayANMKEY.push_back(pNewItem);	
	}	

	
	int iANMKEY;
	DWORD accum_AnmTick=0;	
	//4. SCL보간하여 ANMKEY에 넣기	
	for (UINT i=0;i<m_arraySCLKEY.size();i++)
	{	
		if (m_arraySCLKEY.size()==0)
			break;
	}
	
	//5. ROT보간하여 ANMKEY에 넣기
	ROTKEY prev_rotkey,curr_rotkey,temp_rotkey;
	memset(&prev_rotkey,0,sizeof(prev_rotkey));	
	memset(&curr_rotkey,0,sizeof(curr_rotkey));
	memset(&temp_rotkey,0,sizeof(temp_rotkey));

	for (UINT i=0;i<m_arrayROTKEY.size();i++)
	{
		curr_rotkey.RotationAccum = m_arrayROTKEY[i]->RotationAccum;
		curr_rotkey.AnmTick = m_arrayROTKEY[i]->AnmTick;

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
			m_arrayANMKEY[iANMKEY]->RotationDelta =temp_rotkey.RotationAccum;
			m_arrayANMKEY[iANMKEY]->AnmTick = temp_rotkey.AnmTick;
		}

		iANMKEY=curr_rotkey.AnmTick/interpolateTick;
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
			
			if ( (accum_AnmTick+interpolateTick) > curr_poskey.AnmTick)
				break;

			accum_AnmTick += interpolateTick;

			temp_poskey.AnmTick=accum_AnmTick;
			float s=GetInterpolateValue(prev_poskey.AnmTick,curr_poskey.AnmTick,accum_AnmTick);
			D3DXVec3Lerp(&temp_poskey.TranslationAccum,&prev_poskey.TranslationAccum,&curr_poskey.TranslationAccum,s);

			iANMKEY = accum_AnmTick/interpolateTick;
			m_arrayANMKEY[iANMKEY]->TranslationDelta = temp_poskey.TranslationAccum;
			m_arrayANMKEY[iANMKEY]->AnmTick = temp_poskey.AnmTick;
		}				

		iANMKEY = curr_poskey.AnmTick/interpolateTick;
		m_arrayANMKEY[iANMKEY]->TranslationDelta = curr_poskey.TranslationAccum;
		m_arrayANMKEY[iANMKEY]->AnmTick = curr_poskey.AnmTick;
		
		prev_poskey=curr_poskey;
		
	}

	//7. 보간값까지 저장된 ANMYKEY를 TRANSFORMANM으로 바꾸기
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

		TRANSFORMANM* pNewItem=new TRANSFORMANM;		
		pNewItem->TM = SCLTM * ROTTM * POSTM;		
		pNewItem->anmTime = p->AnmTick / GetRoot()->GetSceneInfo().EX_TICKSPERMS;
		m_arrayTRANSFORMANM.push_back(pNewItem);
	}

	
	if (!m_arrayTRANSFORMANM.empty())
	{
		m_bIsActiveAnimation=TRUE;
	}
	return m_arrayTRANSFORMANM.size();
}


void cSceneNode::UpdateTransformAnm(DWORD elapseTime)
{	
	if (m_arrayTRANSFORMANM.empty())
		return;

	m_AccumTime +=  elapseTime ;	
	m_AnimationTM = m_arrayTRANSFORMANM[m_IndexTransformAnm]->TM ;				
	
	//현재 AnmTick보다 같거나크면 인덱스증가
	DWORD IndexTime = m_arrayTRANSFORMANM[m_IndexTransformAnm]->anmTime ;		
	DWORD CurrTime =  m_AccumTime;

	//현재 인덱스의 AnmTick과 같거나 크면
	if (CurrTime >= IndexTime)
	{			
		do 
		{	// 인덱스를 증가시키고
			m_IndexTransformAnm++;
			if (m_IndexTransformAnm >= m_TotalTransformAnm )
			{
				m_IndexTransformAnm= m_TotalTransformAnm-1;
				if (m_AccumTime >= m_arrayTRANSFORMANM[m_TotalTransformAnm-1]->anmTime)
				{		
					m_IndexTransformAnm=0;
					m_AccumTime=0;			
					CurrTime =  m_AccumTime;					
				}		
				break;
			}	
			// 인덱스 AnmTick을 얻어 다시비교한다.
			IndexTime = m_arrayTRANSFORMANM[m_IndexTransformAnm]->anmTime;							
		} while(CurrTime > IndexTime);
	}
}

void cSceneNode::UpdateChildren(DWORD elapseTime)
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	cSceneNode* pFoundItem=NULL;
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->Update(elapseTime);
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

/*
	SetNodeTM(nodeTM);
	SetWorldTM(nodeTM);
	D3DXMATRIX nodeTM_INV;
	D3DXMatrixInverse(&nodeTM_INV,NULL,&nodeTM);

	// Root에 넣으니 무조건 ChildObject 이긴하다.
	if (IsParentObject())
	{
		SetLocalTM( GetWorldTM() );
	}
	else
	{	// m_LocalTM 을 부모기준의 상대 변환으로 설정한다. 
		SetLocalTM(	GetWorldTM() * GetParentPtr()->GetWorldInverseTM() );	
	}		*/


	return TRUE;
}

BOOL cSceneNode::LinkToParent(D3DXMATRIX& nodeTM)
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
	
	SetNodeTM(nodeTM);
	SetWorldTM(nodeTM);
	D3DXMATRIX nodeTM_INV;
	D3DXMatrixInverse(&nodeTM_INV,NULL,&nodeTM);

	// Root에 넣으니 무조건 ChildObject 이긴하다.
	if (IsParentObject())
	{
		SetLocalTM( GetWorldTM() );
	}
	else
	{	// m_LocalTM 을 부모기준의 상대 변환으로 설정한다. 
		SetLocalTM(	GetWorldTM() * GetParentPtr()->GetWorldInverseTM() );	
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

void cSceneNode::FreeChildren()
{
	list<cSceneNode*>::iterator iter;	
	cSceneNode* pItem=NULL;
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		pItem->FreeChildren();
	}	
	
	for ( iter=m_listChildObject.begin() ; iter!=m_listChildObject.end() ; ++iter)
	{
		pItem=*iter;
		delete pItem;
		
	}	

}