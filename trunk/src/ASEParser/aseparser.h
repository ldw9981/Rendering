#pragma once


#include "ASELexer.h"
#include "../D3D9/IndexBuffer.h"
#include "../D3D9/VertexBuffer.h"
#include "../D3D9/TransformAnm.h"
#include "../Framework/App/App.h"
#include "../Foundation/ResourceMng.h"
#include "../Scene/MaterialNode.h"

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
	string			FILENAME;		// 모델의 파일이름
	DWORD			FIRSTFRAME;		// 시작 프레임
	DWORD			LASTFRAME;		// 마지막 프레임
	DWORD			LASTFRAMEMS;	// 마지막 프레임의 ms
	DWORD			FRAMESPEED;		// 초당 프레임수
	DWORD			TICKSPERFRAME;	// 프레임당 틱수
	DWORD			MESHFRAMESTEP;
	DWORD			KEYFRAMESTEP;
	D3DXVECTOR3		BACKGROUND_STATIC;
	D3DXVECTOR3		AMBIENT_STATIC;
	DWORD			EX_MSPERFRAME;		// 계산된 부가정보. 프레임당 ms
	DWORD			EX_TICKSPERMS;		// 계산된 부가정보. ms당 틱수	
	DWORD			EX_LASTFRAMEMS;		// 계산된 부가정보. 마지막 프레임 ms
};


class SceneGraphNode;
class cMeshNode;
class SceneRoot;


#define MAX_PARSER_BUFFERSIZE	256

struct VNORMAL
{
	WORD iRefFace;
	WORD iRefVertex;
	D3DXVECTOR3 normal;
};

class cASEParser:
	public cASELexer,
	public SharedResourceMng
{
public:
	cASEParser();
	virtual ~cASEParser(void);
private:	

	SCENETIME			m_SceneTime;	// 씬의 시간정보 , 애니메이션 리소스를 만들기 위해 참고된다.
	vector<cMaterialEx>	m_arrMatrial;	// 씬에서 사용되는 매트리얼 정보, 메쉬에 직접 복사한다.

	SceneGraphNode*			m_pSceneRoot;
	LONG				m_Token;
	CHAR				m_TokenString[MAX_PARSER_BUFFERSIZE];
	SceneGraphNode*			m_pLastObject;
	int					m_CNTOBJECT;
public:	
		BOOL			SkipBlock();
		BOOL			FindToken(LONG token);
		LONG			GetNextASEToken();
		float			GetFloat();	
		int				GetInt();
		long			GetLong();
		BOOL			GetVector3(D3DXVECTOR3* output);
		D3DXMATRIX&		GetMatrix();


	string&				GetString(); 
	wstring&			GetWString();
		BOOL			GetString(LPSTR pOutput);
		BOOL			GetWString(LPWSTR pOutput);
		BOOL			GetIdentifier(LPSTR pOutput);
	TransformAnm*	GetTransformAnm(const D3DXMATRIX& localTM);
		
		D3DXMATRIX&		GetNodeTM();

		SceneGraphNode*		GetRootNode() const { return m_pSceneRoot; }
		void			SetRootNode(SceneGraphNode* val) { m_pSceneRoot = val; }	

		BOOL			Load(LPCTSTR strFileName,SceneGraphNode* pOutput);


		BOOL			Parsing_HelperObject();
		BOOL			Parsing_GeoObject();
		BOOL			Parsing_MaterialList();
		BOOL			Parsing_ShapeObject();
		BOOL			Parsing_LightObject();
		BOOL			Parsing_CameraObject();
		BOOL			Parsing_Group();
		BOOL			Parsing_Scene();


		template <typename T>
		void			MergeNormalListIntoVertexList(vector<T>& arrVertex,
			vector<INDEX_FACE_SUBMATERIAL>& arrFaceIndex,
			const vector<VNORMAL>& arrVNormal);

		void			OptimizeTexCoordAndFace(vector<TEXCOORD>& arrTexCoordOut,
			const vector<TEXCOORD>& arrTexCoordIn,
			vector<FACEINDEX16>& arrTFaceIndexInOut);

		template <typename T>
		void			MergeTexCoordListIntoVertexList(vector<T>& arrVertexInOut,
			vector<INDEX_FACE_SUBMATERIAL>& arrVFaceIndexInOut,
			const vector<TEXCOORD>& arrTexCoordIn,
			const vector<FACEINDEX16>& arrTFaceIndexIn);


		void InterpolateTransformAnm(vector<TRANSFORMANM*>& refArrayTRANSFORMANM,
			const vector<SCLKEY*>& arraySCLKEY,			
			const vector<ROTKEY*>& arrayROTKEY,
			const vector<POSKEY*>& arrayPOSKEY,		
			const D3DXMATRIX& localTM,
			const DWORD interpolateTime
			);

		float GetInterpolateValue( int start_time,int end_time,int inter_time );
};
