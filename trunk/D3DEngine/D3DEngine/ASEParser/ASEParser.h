#pragma once


#include "ASELexer.h"
#include "../Resource/RscIndexBuffer.h"
#include "../Resource/RscVertexBuffer.h"
#include "../Resource/RscTransformAnm.h"


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


struct SCENETIME
{
	string			FILENAME;		// ���� �����̸�
	DWORD			FIRSTFRAME;		// ���� ������
	DWORD			LASTFRAME;		// ������ ������
	DWORD			LASTFRAMEMS;	// ������ �������� ms
	DWORD			FRAMESPEED;		// �ʴ� �����Ӽ�
	DWORD			TICKSPERFRAME;	// �����Ӵ� ƽ��
	DWORD			MESHFRAMESTEP;
	DWORD			KEYFRAMESTEP;
	D3DXVECTOR3		BACKGROUND_STATIC;
	D3DXVECTOR3		AMBIENT_STATIC;
	DWORD			EX_MSPERFRAME;		// ���� �ΰ�����. �����Ӵ� ms
	DWORD			EX_TICKSPERMS;		// ���� �ΰ�����. ms�� ƽ��	
	DWORD			EX_LASTFRAMEMS;		// ���� �ΰ�����. ������ ������ ms
};


class cSceneGraphNode;
class cMeshNode;
class cSceneRoot;


#define MAX_PARSER_BUFFERSIZE	256

struct VNORMAL
{
	WORD iRefFace;
	WORD iRefVertex;
	D3DXVECTOR3 normal;
};

class cASEParser:
	public cASELexer
{
public:
	cASEParser();
	virtual ~cASEParser(void);
private:	
	SCENETIME			m_SceneTime;	// ���� �ð����� , �ִϸ��̼� ���ҽ��� ����� ���� ����ȴ�.
	vector<cMaterialEx>	m_arrMatrial;	// ������ ���Ǵ� ��Ʈ���� ����, �޽��� ���� �����Ѵ�.

	cSceneGraphNode*	m_pSceneRoot;
	LONG				m_Token;
	CHAR				m_TokenString[MAX_PARSER_BUFFERSIZE];
	cSceneGraphNode*	m_pLastObject;
	int					m_CNTOBJECT;
public:	
	BOOL				SkipBlock();
	BOOL				FindToken(LONG token);
	LONG				GetNextASEToken();
	float				GetFloat();	
	int					GetInt();
	long				GetLong();
	BOOL				GetVector3(D3DXVECTOR3* output);
	D3DXMATRIX&			GetMatrix();


	string&				GetString(); 
	wstring&			GetWString();
	BOOL				GetString(LPSTR pOutput);
	BOOL				GetWString(LPWSTR pOutput);
	BOOL				GetIdentifier(LPSTR pOutput);
	cRscTransformAnm*	GetRscTransformAnm(const D3DXMATRIX& localTM);
		
	D3DXMATRIX&			GetNodeTM();

	cSceneGraphNode*	GetRootNode() const { return m_pSceneRoot; }
	void				SetRootNode(cSceneGraphNode* val) { m_pSceneRoot = val; }	

	BOOL				Load(LPCTSTR strFileName,cSceneGraphNode* pOutput);


	BOOL				Parsing_HelperObject();
	BOOL				Parsing_GeoObject();
	BOOL				Parsing_MaterialList();
	BOOL				Parsing_ShapeObject();
	BOOL				Parsing_LightObject();
	BOOL				Parsing_CameraObject();
	BOOL				Parsing_Group();
	BOOL				Parsing_Scene();


	template <typename T>
	void				MergeNormalListIntoVertexList(vector<T>& arrVertex,
	vector<INDEX_FACE_SUBMATERIAL>& arrFaceIndex,
	const vector<VNORMAL>& arrVNormal);

	void				OptimizeTexCoordAndFace(vector<TEXCOORD>& arrTexCoordOut,
	const vector<TEXCOORD>& arrTexCoordIn,
		vector<FACEINDEX16>& arrTFaceIndexInOut);

	template <typename T>
	void				MergeTexCoordListIntoVertexList(vector<T>& arrVertexInOut,
		vector<INDEX_FACE_SUBMATERIAL>& arrVFaceIndexInOut,
		const vector<TEXCOORD>& arrTexCoordIn,
		const vector<FACEINDEX16>& arrTFaceIndexIn);

	void				InterpolateTransformAnm(vector<TRANSFORMANM*>& refArrayTRANSFORMANM,
		const vector<SCLKEY*>& arraySCLKEY,			
		const vector<ROTKEY*>& arrayROTKEY,
		const vector<POSKEY*>& arrayPOSKEY,		
		const D3DXMATRIX& localTM,
		const DWORD interpolateTime);

	float GetInterpolateValue( int start_time,int end_time,int inter_time );
};
