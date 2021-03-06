#pragma once
#include "Resource/Resource.h"
#include "Foundation/Interface.h"
namespace Sophia
{

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

// 순수 에니메이션 데이터
struct ANMKEY_DECOMPOSED
{
	DWORD AnmTick;
	D3DXVECTOR3 ScaleAccum;
	D3DXVECTOR3 TranslationAccum;
	D3DXQUATERNION RotationAccum;

	ANMKEY_DECOMPOSED()
		:AnmTick(0),ScaleAccum(0.0f,0.0f,0.0f),TranslationAccum(0.0f,0.0f,0.0f),RotationAccum(0.0f,0.0f,0.0f,0.0f)
	{

	}

	void GetTrasnform(D3DXMATRIX* p)
	{
		D3DXMATRIX tmSCL;
		D3DXMATRIX tmROT;
		D3DXMATRIX tmPOS;	

		// 각성분에대한  TM구하기
		D3DXMatrixScaling(&tmSCL,
			ScaleAccum.x,
			ScaleAccum.y,
			ScaleAccum.z);

		D3DXMatrixRotationQuaternion(&tmROT,
			&RotationAccum);					

		D3DXMatrixTranslation(&tmPOS,
			TranslationAccum.x,
			TranslationAccum.y,
			TranslationAccum.z);	

		*p = tmSCL * tmROT * tmPOS;		
	}
};

struct ANMKEY_MATRIX
{
	DWORD AnmTick;
	D3DXMATRIX mat;

	ANMKEY_MATRIX()
		:AnmTick(0)
	{

	}
};

struct ENTITY_ANIMATION_DESCRIPTION
{
	int					playIndex;
	DWORD				playTime;
	DWORD				length;
	DWORD				skipStartTime;
	DWORD				earlyEndTime;
	bool				loop;
	DWORD				fadeInTime;
	DWORD				fadeTime;
	float				fadeWeight;

	ENTITY_ANIMATION_DESCRIPTION()
		:playIndex(-1),playTime(0),skipStartTime(0),earlyEndTime(0),loop(false),fadeInTime(0),fadeTime(0),fadeWeight(0.0f),length(0)
	{

	}
};

// 하나의 씬노드에 해당하는 시간을 포함한 연속적인 에니메이션 데이터 
class SceneAnimation:
	public ISerializable
{
public:
	//std::vector<ANMKEY> m_arrayANMKEY;
	std::vector<ANMKEY_MATRIX>	m_arrKey;
	float	m_partialWeight;
public:
	SceneAnimation(void);
	~SceneAnimation(void);

	inline float	GetInterpolateValue( DWORD start_time,DWORD end_time,DWORD inter_time );
	void	GetMatrix(D3DXMATRIX& out,DWORD animationTime,size_t& index);
	void	SerializeIn(std::ifstream& stream);
	void	SerializeOut(std::ofstream& stream);

	void	Cut(DWORD timeStart,DWORD timeEnd,SceneAnimation* pOut);
	static  void BlendMatrix(D3DXMATRIX& out,D3DXMATRIX& in1,D3DXMATRIX& in2,float v1,float v2);
};

// 엔티티 즉 전체 씬을 에니메이션하는 데이터
class EntityAnimation:
	public cResource
	,public ISerializable
{
public:
	EntityAnimation(void);
	~EntityAnimation(void);
public:
	// SceneNodeName/ArrayAnmKey
	std::map<std::string,SceneAnimation*> m_container;
	DWORD	m_dwTimeLength;
public:
	SceneAnimation* GetSceneAnimtion(std::string& nodeNAme);
	SceneAnimation* CreateSceneAnimation(std::string& nodeName);
	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();

	virtual void SerializeIn(std::ifstream& stream);
	virtual void SerializeOut(std::ofstream& stream);

	void	Cut(DWORD start,DWORD end,EntityAnimation* pOut);
};

}

