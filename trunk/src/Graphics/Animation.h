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

// ���� ���ϸ��̼� ������
struct ANMKEY
{
	DWORD AnmTick;
	D3DXVECTOR3 ScaleAccum;
	D3DXVECTOR3 TranslationAccum;
	D3DXQUATERNION RotationAccum;

	ANMKEY()
		:AnmTick(0),ScaleAccum(0.0f,0.0f,0.0f),TranslationAccum(0.0f,0.0f,0.0f),RotationAccum(0.0f,0.0f,0.0f,0.0f)
	{

	}

};

struct ENTITY_ANIMATION_DESCRIPTION
{
	int					playIndex;
	DWORD				playTime;
	DWORD				skipStartTime;
	DWORD				earlyEndTime;
	bool				loop;
	DWORD				fadeInTime;
	DWORD				fadeTime;
	float				fadeWeight;

	ENTITY_ANIMATION_DESCRIPTION()
		:playIndex(-1),playTime(0),skipStartTime(0),earlyEndTime(0),loop(false),fadeInTime(0),fadeTime(0),fadeWeight(0.0f)
	{

	}
};

// �ϳ��� ����忡 �ش��ϴ� �ð��� ������ �������� ���ϸ��̼� ������ 
class SceneAnimation:
	public ISerializable
{
public:
	SceneAnimation(void);
	~SceneAnimation(void);
public:
	std::vector<ANMKEY> m_arrayANMKEY;
	DWORD*	m_pTimeLength;
	float	m_weight;

	float	GetInterpolateValue( int start_time,int end_time,int inter_time );
	void	GetTransform(D3DXMATRIX& out,DWORD animationTime );	
	void	GetAnimationKey(ANMKEY& out,DWORD animationTime);
	void	SerializeIn(std::ifstream& stream);
	void	SerializeOut(std::ofstream& stream);

	void	Cut(DWORD timeStart,DWORD timeEnd,SceneAnimation* pOut);
	static  void InterpolateAnimationnKey(ANMKEY& out,ANMKEY& in1,ANMKEY& in2,float v);
};

// ��ƼƼ �� ��ü ���� ���ϸ��̼��ϴ� ������
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

