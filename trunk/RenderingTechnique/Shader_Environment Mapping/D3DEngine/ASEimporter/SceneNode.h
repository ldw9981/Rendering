#pragma once
#include "../Common/interface.h"
#include "../Common/Define.h"


#define INTERPOLATE_TIME 16

struct POSKEY
{
	DWORD AnmTick;
	D3DXVECTOR3 TranslationAccum;
};


struct ROTKEY
{
	DWORD AnmTick;
	D3DXQUATERNION RotationAccum;
};


struct SCLKEY
{
	DWORD AnmTick;
	D3DXVECTOR3 ScaleAccum;
};


struct ANMKEY
{
	DWORD AnmTick;
	D3DXVECTOR3 ScaleDelta;
	D3DXVECTOR3 TranslationDelta;
	D3DXQUATERNION RotationDelta;
};


struct ANMTM
{
	DWORD AnmTick;
	D3DXMATRIX TM;
};





class cASEParser; 
class cScene;
struct SCENEINFO; 




class cSceneNode:
	public IUnknownObject,
	public cIProgressable,
	public cIRenderable

{
public:
	cSceneNode(void);
	virtual ~cSceneNode(void);

enum ASEOBJ { NONE,MESH,HELPER,LIGHT,CAMEARA,SHAPE,ANYTHING };
	
	
private:
	string			m_strNodeName;
	string			m_strParentName;
	list<cSceneNode*> m_listChildObject;
	cSceneNode*		m_pParent;
	cScene*			m_pScene;
	




	vector<DWORD>	   m_arrayTime;
	vector<POSKEY*> m_arrayPOSKEY;
	vector<ROTKEY*> m_arrayROTKEY;
	vector<SCLKEY*> m_arraySCLKEY;
	vector<ANMKEY*> m_arrayANMKEY;
	vector<ANMTM*>  m_arrayANMTM;
	D3DXMATRIX		m_AnimationTM;	
	float			m_IndexANMTM;		
	int				m_CNTANMKEY;
	DWORD			m_AnmTickEnd;
	int				m_CNTANMTM;
	int				m_iANMTM;
	DWORD			m_AccumMSTick;
	BOOL			m_bIsActiveAnimation;

	UINT			m_NodeType;	




	


	D3DXMATRIX		m_WorldInverseTM;	

	D3DXMATRIX		m_ScaleTM;
	D3DXMATRIX		m_TranslationTM;
	D3DXMATRIX		m_RotationTM;	
	D3DXMATRIX		m_NodeTM;		
	cSceneNode*		m_pRoot;
protected:	
	D3DXMATRIX		m_WorldTM;
	D3DXMATRIX		m_LocalTM;
	

public:

public:
	// loading	
	virtual BOOL	LoadObjInfo(cASEParser* pParser);

			BOOL	LoadANM(cASEParser* pParser);
	

			float	GetInterpolateValue(int start_time,int end_time,int inter_time );

			void	CalculateANMKEY(int interpolate_time);

			void	SetAnimationTM();
		D3DXMATRIX& GetAnimationTM();
			

	//  Object list
	// 자식 오브젝트를 추가한다.
			void	InsertChildObject(cSceneNode* pItem);

	// 자신을 포함한 자식ASE오브젝트에서 ASE오브젝트를 찾는다.
		cSceneNode*	FindChildObject(string& nodename,int type=ANYTHING);

	//Get/Set
			void	SetNodeName( LPCSTR nodename );
			string&	GetNodeName();

			void	SetParentName( LPCSTR nodename );
			string&	GetParentName();
			BOOL	IsExistParentName();		
			BOOL	IsParentObject();
	
		
		D3DXMATRIX& GetNodeTM() { return m_NodeTM; }
			void	SetNodeTM(D3DXMATRIX &val) { m_NodeTM=val; }

		
		D3DXMATRIX& GetWorldTM() { return m_WorldTM; }
			void	SetWorldTM(D3DXMATRIX &val) {m_WorldTM=val; }

			
		D3DXMATRIX& GetLocalTM() { return m_LocalTM; }
			void	SetLocalTM(D3DXMATRIX &val) { m_LocalTM=val; }

		D3DXMATRIX& GetLocalInverseTM();
		D3DXMATRIX& GetWorldInverseTM();

			UINT	GetNodeType() const { return m_NodeType; }
			void	SetNodeType(UINT val) { m_NodeType = val; }

		cSceneNode* GetParentPtr() const { return m_pParent; }
			void	SetParentPtr(cSceneNode* val) { m_pParent = val; }

		BOOL		LinkToParent();

		cSceneNode* GetRootPtr() const { return m_pRoot; }
		void		SetRootPtr(cSceneNode* val) { m_pRoot = val; }



		cScene*		GetScenePtr() const { return m_pScene; }
		void		SetScenePtr(cScene* val) { m_pScene = val; }

		BOOL		GetIsActiveAnimation() const { return m_bIsActiveAnimation; }
		void		SetIsActiveAnimation(BOOL val) { m_bIsActiveAnimation = val; }

		void		GetPosition( D3DXVECTOR3& pos );
		void		SetPosition( D3DXVECTOR3& pos );
	
	D3DXMATRIX&		MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ );
	D3DXMATRIX&		RotateByLocalAxis( float angleX,float angleY,float angleZ );
	virtual void	Init();
	virtual void	Uninit();
	virtual void	Update();
	virtual void	Render();

		void		InitChildren();
		void		UninitChildren();
		void		UpdateChildren();
		void		RenderChildren();


};

