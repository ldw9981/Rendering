#pragma once


#include "ASELexer.h"
#include "../Resource/RscIndexBuffer.h"
#include "../Resource/RscVertexBuffer.h"



class cSceneNode;
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
	
	cSceneRoot* m_pSceneRoot;

	LONG	m_Token;
	CHAR	m_TokenString[MAX_PARSER_BUFFERSIZE];
	cSceneNode* m_pLastObject;
	
	

	int		m_CNTOBJECT;
public:					
		BOOL		FindToken(LONG token);
		LONG		GetNextASEToken();
		float		GetFloat();	
		int			GetInt();
		long		GetLong();
		BOOL		GetVector3(D3DXVECTOR3* output);
		BOOL		GetMatrix(D3DXMATRIX* output);


	string&			GetString(); 
	wstring&		GetWString();
		BOOL		GetString(LPSTR pOutput);
		BOOL		GetWString(LPWSTR pOutput);
		BOOL		GetIdentifier(LPSTR pOutput);

		BOOL		SkipBlock();
		BOOL		GetNodeTM(D3DXMATRIX &output);

		cSceneRoot*	GetSceneRoot() const { return m_pSceneRoot; }
		void		SetSceneRoot(cSceneRoot* val) { m_pSceneRoot = val; }	

		BOOL		Load(LPCTSTR strFileName,cSceneRoot* pOutput);


		BOOL		Parsing_HelperObject();
		BOOL		Parsing_GeoObject();
		BOOL		Parsing_MaterialList();
		BOOL		Parsing_ShapeObject();
		BOOL		Parsing_LightObject();
		BOOL		Parsing_CameraObject();
		BOOL		Parsing_Group();
		BOOL		Parsing_Scene();


		template <typename T>
		void MergeNormalListIntoVertexList(vector<T>& arrVertex,
			vector<pair<FACEINDEX16,SUBMATINDEX>>& arrFaceIndex,
			const vector<VNORMAL>& arrVNormal);

		void OptimizeTexCoordAndFace(vector<TEXCOORD>& arrTexCoordOut,
			const vector<TEXCOORD>& arrTexCoordIn,
			vector<FACEINDEX16>& arrTFaceIndexInOut);

		template <typename T>
		void MergeTexCoordListIntoVertexList(vector<T>& arrVertexInOut,
			vector<pair<FACEINDEX16,SUBMATINDEX>>& arrVFaceIndexInOut,
			const vector<TEXCOORD>& arrTexCoordIn,
			const vector<FACEINDEX16>& arrTFaceIndexIn);
};
