#pragma once


#include "ASELexer.h"



class cSceneNode;
class cMeshNode;
class cScene;


#define MAX_PARSER_BUFFERSIZE	256


class cASEParser:
	public cASELexer
{
public:
	cASEParser();
	virtual ~cASEParser(void);

private:
	
	cScene* m_pScene;

	LONG	m_token;
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

		cScene*		GetScene() const { return m_pScene; }
		void		SetScene(cScene* val) { m_pScene = val; }	

		BOOL		Load(LPCTSTR strFileName,cScene* pOutput);


		BOOL		Parsing_HelperObject();
		BOOL		Parsing_GeoObject();
		BOOL		Parsing_MaterialList();
		BOOL		Parsing_ShapeObject();
		BOOL		Parsing_LightObject();
		BOOL		Parsing_CameraObject();
		BOOL		Parsing_Group();
		BOOL		Parsing_Scene();
};
