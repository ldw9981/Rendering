#pragma once


#include "ASELexer.h"
#include "Graphics/RscIndexBuffer.h"
#include "Graphics/RscVertexBuffer.h"
#include "Graphics/Vertex.h"
#include "Resource/ResourceMng.h"
#include "Math/Sphere.h"
#include "Graphics/Animation.h"

namespace Sophia
{


#define DEF_INTERPOLATE_TIME 16

class cSceneNode;
class cMeshNode;
class SkinnedMeshNode;
class cHelperNode;
class Entity;
class cSphere;
class SceneAnimation;
class Skeleton;
struct BONEREFINFO;

struct SCENENODEINFO
{
	std::string strNodeName;
	std::string strParentName;
	D3DXMATRIX tmNode;
	D3DXMATRIX tmLocal;
	cSceneNode* pParent;

	SCENENODEINFO()
	{
		D3DXMatrixIsIdentity(&tmNode);
		D3DXMatrixIsIdentity(&tmLocal);
		pParent=NULL;
	}
};




struct SCENETIME
{
	std::string			FILENAME;		// ���� �����̸�
	DWORD			FIRSTFRAME;		// ���� ������
	DWORD			LASTFRAME;		// ������ ������
	DWORD			FRAMESPEED;		// �ʴ� �����Ӽ�
	DWORD			TICKSPERFRAME;	// �����Ӵ� ƽ��
	DWORD			MESHFRAMESTEP;
	DWORD			KEYFRAMESTEP;
	D3DXVECTOR3		BACKGROUND_STATIC;
	D3DXVECTOR3		AMBIENT_STATIC;
	float			EX_MSPERFRAME;		// ���� �ΰ�����. �����Ӵ� ms
	float			EX_TICKSPERMS;		// ���� �ΰ�����. ms�� ƽ��	
	float			EX_LASTFRAMEMS;		// ���� �ΰ�����. ������ ������ ms
};





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
	SCENETIME						m_SceneTime;	// ���� �ð����� , �ִϸ��̼� ���ҽ��� ����� ���� ����ȴ�.
	//std::vector<Material>						m_vecMaterial;	// ������ ���Ǵ� ��Ʈ���� ����, �޽��� ���� �����Ѵ�.
	//std::vector<std::vector<Material>>		m_vecMaterial;
	D3DXVECTOR3			m_tempAxisMin;
	D3DXVECTOR3			m_tempAxisMax;	

	Entity*				m_pSceneRoot;
	LONG				m_Token;
	CHAR				m_TokenString[MAX_PARSER_BUFFERSIZE];
	cSceneNode*			m_pLastObject;
	int					m_CNTOBJECT;
	int					m_repeat;
	EntityAnimation*			m_pEntityAnimation;
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
	SceneAnimation*		GetSceneAnimation(const char* meshName,const D3DXMATRIX& localTM);
		
		D3DXMATRIX&		GetNodeTM();

		Entity*			GetRootNode() const { return m_pSceneRoot; }
		void			SetRootNode(Entity* val) { m_pSceneRoot = val; }	

		BOOL			Load(const char* strFileName,Entity* pOutput);


		BOOL			Parsing_HelperObject();
		BOOL			Parsing_GeoObject();
		bool			GetTextureVertexList(std::vector<TEXCOORD>& out);	
		bool			GetTextureFaceList(std::vector<TRIANGLE>& out);

		BOOL			Parsing_MaterialList();
		bool			GetSubMaterial(Material& SubMatrial);// return class
		cRscTexture*	GetTexture();

		BOOL			Parsing_ShapeObject();
		BOOL			Parsing_LightObject();
		BOOL			Parsing_CameraObject();
		BOOL			Parsing_Group();
		BOOL			Parsing_Scene();

		void			SetNodeInfo(cSceneNode* pNode,SCENENODEINFO& stInfo);
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
		cSceneNode*  CreateMeshNode(SCENENODEINFO& stInfo,cRscVertexBuffer* pVetexBuffer,cRscIndexBuffer* pIndexBuffer,std::map<SUBMATINDEX,WORD>& mapIndexCount,int nMaterialRef);
		cSceneNode* CreateSkinnedMeshNode(SCENENODEINFO& stInfo,cRscVertexBuffer* pVetexBuffer,cRscIndexBuffer* pIndexBuffer,std::map<SUBMATINDEX,WORD>& mapIndexCount,int nMaterialRef,std::vector<BONEREFINFO>& boneRef);
		cSceneNode* CreateSkeleton(SCENENODEINFO& stInfo);

		cSceneNode* CreateSceneNode(SCENENODEINFO& stInfo);

		template <typename T>
		cRscVertexBuffer* CreateRscVertexBuffer(const char* meshName,std::vector<T>& arrVertex);

		
		cRscIndexBuffer* CreateRscIndexBuffer(const char* meshName,std::vector<TRIANGLE_SUBMATERIAL>& arrIndex);
		
};

}