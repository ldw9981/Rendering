#pragma once


#include "ASELexer.h"
#include "Graphics/RscIndexBuffer.h"
#include "Graphics/RscVertexBuffer.h"
#include "Graphics/RscTransformAnm.h"
#include "Graphics/Vertex.h"
#include "Resource/ResourceMng.h"
#include "Math/Sphere.h"

#define DEF_INTERPOLATE_TIME 16

class cSceneNode;
class cMeshNode;
class SkinnedMeshNode;
class cHelperNode;
class Entity;
class cSphere;
struct BONEREFINFO;

struct SCENENODEINFO
{
	std::string strNodeName;
	std::string strParentName;
	D3DXMATRIX tmNode;
	D3DXMATRIX tmInvNode;
	D3DXMATRIX tmWorld;
	D3DXMATRIX tmLocal;
	cSceneNode* pParent;

	SCENENODEINFO()
	{
		D3DXMatrixIsIdentity(&tmNode);
		D3DXMatrixIsIdentity(&tmInvNode);
		D3DXMatrixIsIdentity(&tmWorld);
		D3DXMatrixIsIdentity(&tmLocal);
		pParent=NULL;
	}
};




struct SCENETIME
{
	std::string			FILENAME;		// 모델의 파일이름
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
	SCENETIME						m_SceneTime;	// 씬의 시간정보 , 애니메이션 리소스를 만들기 위해 참고된다.
	//std::vector<Material>				m_vecMaterial;	// 씬에서 사용되는 매트리얼 정보, 메쉬에 직접 복사한다.
	std::vector<std::vector<Material>>		m_vecMaterial;
	D3DXVECTOR3			m_tempAxisMin;
	D3DXVECTOR3			m_tempAxisMax;	

	Entity*				m_pSceneRoot;
	LONG				m_Token;
	CHAR				m_TokenString[MAX_PARSER_BUFFERSIZE];
	cSceneNode*			m_pLastObject;
	int					m_CNTOBJECT;
	int					m_repeat;
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


		std::string		GetString(); 
	
		BOOL			GetString(LPSTR pOutput);
		BOOL			Getstring(LPWSTR pOutput);
		BOOL			GetIdentifier(LPSTR pOutput);
	cRscTransformAnm*	GetRscTransformAnm(const char* rootName,const char* meshName,const D3DXMATRIX& localTM);
	
		
		D3DXMATRIX&		GetNodeTM();

		Entity*		GetRootNode() const { return m_pSceneRoot; }
		void			SetRootNode(Entity* val) { m_pSceneRoot = val; }	

		BOOL			Load(const char* strFileName,Entity* pOutput);


		BOOL			Parsing_HelperObject();
		BOOL			Parsing_GeoObject();
		bool			GetTextureVertexList(std::vector<TEXCOORD>& out);	
		bool			GetTextureFaceList(std::vector<TRIANGLE>& out);

		BOOL			Parsing_MaterialList();
		bool			GetSubMaterial(Material& SubMatrial);// return class
		BOOL			Parsing_ShapeObject();
		BOOL			Parsing_LightObject();
		BOOL			Parsing_CameraObject();
		BOOL			Parsing_Group();
		BOOL			Parsing_Scene();


		template <typename T>
		void			MergeNormalListIntoVertexList(std::vector<T>& arrVertex,
			std::vector<TRIANGLE_SUBMATERIAL>& arrFaceIndex,
			const std::vector<VNORMAL>& arrVNormal);

		void			OptimizeTexCoordAndFace(std::vector<TEXCOORD>& arrTexCoordOut,
			const std::vector<TEXCOORD>& arrTexCoordIn,
			std::vector<TRIANGLE>& arrTFaceIndexInOut);

		template <typename T>
		void			MergeTexCoordListIntoVertexList(bool bBaseMapChannel,std::vector<T>& arrVertexInOut,
			std::vector<TRIANGLE_SUBMATERIAL>& arrVFaceIndexInOut,
			const std::vector<TEXCOORD>& arrTexCoordIn,
			const std::vector<TRIANGLE>& arrTFaceIndexIn);

		void			CalculateSphere(D3DXVECTOR3& tempAxisMin,D3DXVECTOR3& tempAxisMax,cSphere& out);

	
		
		void  ConvertAccQuaternion(std::vector<ROTKEY>& arrayROTKEY,const D3DXMATRIX& localTM);
		void  ConvertAccQuaternionEX(std::vector<std::pair<DWORD,D3DXQUATERNION>>& inArrayROTKEY,const D3DXMATRIX& inLocalTM,std::map<DWORD,ANMKEY> outRefMapAnmKey);
		cMeshNode*  CreateMeshNode(SCENENODEINFO& stInfo,cRscVertexBuffer* pVetexBuffer,cRscIndexBuffer* pIndexBuffer,std::map<SUBMATINDEX,WORD>& mapIndexCount,int nMaterialRef);
		SkinnedMeshNode* CreateSkinnedMeshNode(SCENENODEINFO& stInfo,cRscVertexBuffer* pVetexBuffer,cRscIndexBuffer* pIndexBuffer,std::map<SUBMATINDEX,WORD>& mapIndexCount,int nMaterialRef,std::vector<BONEREFINFO>& boneRef);
		cHelperNode* CreateHelperNode(SCENENODEINFO& stInfo);

		template <typename T>
		cRscVertexBuffer* CreateRscVertexBuffer(const char* meshName,std::vector<T>& arrVertex);

		
		cRscIndexBuffer* CreateRscIndexBuffer(const char* meshName,std::vector<TRIANGLE_SUBMATERIAL>& arrIndex);
		
};

