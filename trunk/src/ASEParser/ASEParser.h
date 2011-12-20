#pragma once


#include "ASELexer.h"
#include "D3D9Server/RscIndexBuffer.h"
#include "D3D9Server/RscVertexBuffer.h"
#include "D3D9Server/RscTransformAnm.h"
#include "Resource/ResourceMng.h"

#define DEF_INTERPOLATE_TIME 16

class cSceneNode;
class cMeshNode;
class SkinnedMeshNode;
class cHelperNode;
class cSceneRoot;
class cSphere;
struct BONEREFINFO;

struct SCENENODEINFO
{
	string strNodeName;
	string strParentName;
	D3DXMATRIX tmNode;
	D3DXMATRIX tmInvNode;
	D3DXMATRIX tmWorld;
	D3DXMATRIX tmLocal;

	cSceneNode* pParent;
	cRscTransformAnm* pRscTransform;

	SCENENODEINFO()
	{
		D3DXMatrixIsIdentity(&tmNode);
		D3DXMatrixIsIdentity(&tmInvNode);
		D3DXMatrixIsIdentity(&tmWorld);
		D3DXMatrixIsIdentity(&tmLocal);
		pParent=NULL;
		pRscTransform=NULL;
	}
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





#define MAX_PARSER_BUFFERSIZE	256

struct VNORMAL
{
	WORD iRefFace;
	WORD iRefVertex;
	D3DXVECTOR3 normal;
};


class cASEParser:
	public cASELexer,
	private cStaticResourceMng
{
public:
	cASEParser();
	virtual ~cASEParser(void);
private:	
	SCENETIME						m_SceneTime;	// ���� �ð����� , �ִϸ��̼� ���ҽ��� ����� ���� ����ȴ�.
	vector<Material>				m_vecMaterial;	// ������ ���Ǵ� ��Ʈ���� ����, �޽��� ���� �����Ѵ�.
	vector<vector<Material>>		m_vecMultiSubMaterial;

	cSceneNode*			m_pSceneRoot;
	LONG				m_Token;
	CHAR				m_TokenString[MAX_PARSER_BUFFERSIZE];
	cSceneNode*			m_pLastObject;
	int					m_CNTOBJECT;
public:	
	virtual void		Close();
		BOOL			SkipBlock();
		BOOL			FindToken(LONG token);
		LONG			GetNextASEToken();
		float			GetFloat();	
		int				GetInt();
		long			GetLong();
		BOOL			GetVector3(D3DXVECTOR3* output);
		D3DXMATRIX&		GetMatrix();


		string			GetString(); 
	
		BOOL			GetString(LPSTR pOutput);
		BOOL			Getstring(LPWSTR pOutput);
		BOOL			GetIdentifier(LPSTR pOutput);
	cRscTransformAnm*	GetRscTransformAnm(const D3DXMATRIX& localTM);
	
		
		D3DXMATRIX&		GetNodeTM();

		cSceneNode*		GetRootNode() const { return m_pSceneRoot; }
		void			SetRootNode(cSceneNode* val) { m_pSceneRoot = val; }	

		BOOL			Load(const char* strFileName,cSceneNode* pOutput);


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

		template <typename T>
		void			CalculateSphere(D3DXVECTOR3& tempAxisMin,D3DXVECTOR3& tempAxisMax,vector<T>& arrVertex,cSphere& out);

	
		
		void  ConvertAccQuaternion(vector<ROTKEY>& arrayROTKEY,const D3DXMATRIX& localTM);
		void  ConvertAccQuaternionEX(vector<pair<DWORD,D3DXQUATERNION>>& inArrayROTKEY,const D3DXMATRIX& inLocalTM,map<DWORD,ANMKEY> outRefMapAnmKey);
		cMeshNode*  CreateMeshNode(SCENENODEINFO& stInfo,cRscVertexBuffer* pVetexBuffer,cRscIndexBuffer* pIndexBuffer,map<SUBMATINDEX,WORD>& mapIndexCount,int nMaterialRef);
		SkinnedMeshNode* CreateSkinnedMeshNode(SCENENODEINFO& stInfo,cRscVertexBuffer* pVetexBuffer,cRscIndexBuffer* pIndexBuffer,map<SUBMATINDEX,WORD>& mapIndexCount,int nMaterialRef,vector<BONEREFINFO>& boneRef);
		cHelperNode* CreateHelperNode(SCENENODEINFO& stInfo);

		template <typename T>
		cRscVertexBuffer* CreateRscVertexBuffer(vector<T>& arrVertex);

		
		cRscIndexBuffer* CreateRscIndexBuffer(vector<INDEX_FACE_SUBMATERIAL>& arrIndex);
		
};

