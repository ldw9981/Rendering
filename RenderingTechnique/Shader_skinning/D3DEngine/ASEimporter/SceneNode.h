#pragma once
#include "../Common/interface.h"

class cSceneRoot;

#define DEF_INTERPOLATE_TIME 16

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


struct TRANSFORMANM
{
	DWORD	anmTime;
	D3DXMATRIX TM;
};





class cASEParser; 
class cSceneRoot;
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
	UINT			m_NodeType;	
	wstring			m_strNodeName;
	wstring			m_strParentName;
	list<cSceneNode*> m_listChildObject;
	cSceneNode*		m_pParent;
	cSceneRoot*		m_pRoot;
	

	vector<POSKEY*> m_arrayPOSKEY;
	vector<ROTKEY*> m_arrayROTKEY;
	vector<SCLKEY*> m_arraySCLKEY;
	vector<ANMKEY*> m_arrayANMKEY;
	vector<TRANSFORMANM*>  m_arrayTRANSFORMANM;

	
	UINT			m_TotalTransformAnm;
	UINT			m_IndexTransformAnm;
	DWORD			m_AccumTime;
	BOOL			m_bIsActiveAnimation;
	

	D3DXMATRIX		m_AnimationTM;			
	D3DXMATRIX		m_WorldInverseTM;	

	D3DXMATRIX		m_ScaleTM;
	D3DXMATRIX		m_TranslationTM;
	D3DXMATRIX		m_RotationTM;	
	D3DXMATRIX		m_NodeTM;		
	
protected:	
	D3DXMATRIX		m_WorldTM;
	D3DXMATRIX		m_LocalTM;
	

public:

public:

	BOOL			LoadANM(cASEParser* pParser);
	
	UINT			GetTotalTransformAnm() const { return m_TotalTransformAnm; }
	void			SetTotalTransformAnm(UINT val) { m_TotalTransformAnm = val; }
	float			GetInterpolateValue(int start_time,int end_time,int inter_time );

	UINT			InterpolateTransformAnm(DWORD interpolateTime);

	void			UpdateTransformAnm(DWORD elapseTime);
	D3DXMATRIX&		GetAnimationTM();
			

	//  Object list
	// 자식 오브젝트를 추가한다.
	void			InsertChildObject(cSceneNode* pItem);

	// 자신을 포함한 자식ASE오브젝트에서 ASE오브젝트를 찾는다.
	cSceneNode*		FindChildObject(wstring& nodename,int type=ANYTHING);

	//Get/Set
	void			SetNodeName( LPCWSTR nodename );
	wstring&		GetNodeName();

	void			SetParentName( LPCWSTR nodename );
	wstring&		GetParentName();
	BOOL			IsExistParentName();		
	BOOL			IsParentObject();
	
		
	D3DXMATRIX&		GetNodeTM() { return m_NodeTM; }
	void			SetNodeTM(D3DXMATRIX &val) { m_NodeTM=val; }

		
	D3DXMATRIX&		GetWorldTM() { return m_WorldTM; }
	void			SetWorldTM(D3DXMATRIX &val) {m_WorldTM=val; }

			
	D3DXMATRIX&		GetLocalTM() { return m_LocalTM; }
	void			SetLocalTM(D3DXMATRIX &val) { m_LocalTM=val; }

	D3DXMATRIX&		GetLocalInverseTM();
	D3DXMATRIX&		GetWorldInverseTM();

	UINT			GetNodeType() const { return m_NodeType; }
	void			SetNodeType(UINT val) { m_NodeType = val; }

	cSceneNode*		GetParentPtr() const { return m_pParent; }
	void			SetParentPtr(cSceneNode* val) { m_pParent = val; }

	BOOL			LinkToParent();
	BOOL			LinkToParent(D3DXMATRIX& nodeTM);

	cSceneRoot*		GetRoot() const { return m_pRoot; }
	void			SetRoot(cSceneRoot* val) { m_pRoot = val; }



	BOOL			GetIsActiveAnimation() const { return m_bIsActiveAnimation; }
	void			SetIsActiveAnimation(BOOL val) { m_bIsActiveAnimation = val; }

	void			GetPosition( D3DXVECTOR3& pos );
	void			SetPosition( D3DXVECTOR3& pos );
	
	D3DXMATRIX&		MovePositionByLocalAxis( float deltaX,float deltaY,float deltaZ );
	D3DXMATRIX&		RotateByLocalAxis( float angleX,float angleY,float angleZ );
	virtual void	Init();
	virtual void	Uninit();
	virtual void	Update(DWORD elapseTime);
	virtual void	Render();

		void		InitChildren();
		void		UninitChildren();
		void		UpdateChildren(DWORD elapseTime);
		void		RenderChildren();
		void		FreeChildren();


};

