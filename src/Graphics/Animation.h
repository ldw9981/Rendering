#pragma once
#include "Resource/Resource.h"
#include "Foundation/Interface.h"

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

// �ϳ��� ����忡 �ش��ϴ� �ð��� ������ �������� ���ϸ��̼� ������ 
class SceneAnimation:
	public ISerializable
{
public:
	SceneAnimation(void);
	~SceneAnimation(void);
public:
	std::vector<ANMKEY> m_arrayANMKEY;
	DWORD	m_dwTimeLength;

	float	GetInterpolateValue( int start_time,int end_time,int inter_time );
	void	GetTransform(D3DXMATRIX& out,DWORD& animationTime, DWORD dwTimeDelta);	
	void	SerializeIn(std::ifstream& stream);
	void	SerializeOut(std::ofstream& stream);
};

// ��ƼƼ �� ��ü ���� ���ϸ��̼��ϴ� ������
class EntityAnimation:
	public cResource
	,public ISerializable
{
public:
	EntityAnimation(void);
	~EntityAnimation(void);
protected:
	// SceneNodeName/ArrayAnmKey
	std::map<std::string,SceneAnimation*> m_container;

public:
	SceneAnimation* GetSceneAnimtion(std::string& nodeNAme);
	SceneAnimation* CreateSceneAnimation(std::string& nodeName);
	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();

	virtual void SerializeIn(std::ifstream& stream);
	virtual void SerializeOut(std::ofstream& stream);
};



