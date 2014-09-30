#pragma once

#include "RendererQueue.h"
#include "Vertex.h"
namespace Sophia
{

class cGUIFont;
class cView;
class World;

#define TECHNIQUE_SIZE 32

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

public:
	static Graphics* m_pInstance;
	static LPDIRECT3DDEVICE9	m_pDevice;
	// D3D 변수추가	
	D3DVIEWPORT9			m_viewPortInfo;
	
	D3DPRESENT_PARAMETERS	m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D9;
	LPD3DXFONT				m_pFont;
	LPD3DXEFFECT			m_pEffect;	
	LPD3DXBUFFER			m_pError;	
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclationNormal;
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclationBlend;
	D3DXHANDLE				m_hTLine;// 테크닉
	D3DXHANDLE				m_hTSkinningPhong;// 테크닉	
	D3DXHANDLE				m_hTSkinningPhongDiffuse;// 테크닉	
	D3DXHANDLE				m_hTerrain;// 테크닉
	D3DXHANDLE				m_hTPhong;// 테크닉
	D3DXHANDLE				m_hTPhongDiffuse;// 테크닉
	D3DXHANDLE				m_hTPhongDiffuseLight;
	D3DXHANDLE				m_hTPhongDiffuseBump;
	D3DXHANDLE				m_hTPhongDiffuseSpecular;// 테크닉
	D3DXHANDLE				m_hTPhongDiffuseOpacity;// 테크닉
	D3DXHANDLE				m_hTPhongDiffuseBumpSpecular;

	D3DXHANDLE				m_hTCreateShadowNormal;	// 쉐이더
	D3DXHANDLE				m_hTCreateShadowBlend;	// 쉐이더
	D3DXHANDLE				m_hTCreateShadowNormalAlphaTest;	// 쉐이더
	D3DXHANDLE				m_hTCreateShadowBlendAlphaTest;	// 쉐이더
	D3DXHANDLE				m_hTGUI;
	LPD3DXEFFECT			m_pApplyShadowShader;
	LPD3DXEFFECT			m_pCreateShadowShader;

	D3DXHANDLE				m_hmWVP;	// 월드~투영행렬
	D3DXHANDLE				m_hmProjection;	// 월드~투영행렬
	D3DXHANDLE				m_hmView;	// 월드~투영행렬
	D3DXHANDLE				m_hmWorld;	// 월드~투영행렬
	D3DXHANDLE				m_hmViewProjection;
	D3DXHANDLE				m_hvLightDir;
	D3DXHANDLE				m_hmPalette;
	D3DXHANDLE				m_hvWorldLightPosition;
	D3DXHANDLE				m_hvWorldCameraPosition;;

	D3DXHANDLE				m_hmLightView;
	D3DXHANDLE				m_hmLightProjection;
	cGUIFont*					m_pNewFont;
	std::string m_strHLSL;

	int						m_nTechniqueSize;
	D3DXHANDLE*				m_pTNormal;
	D3DXHANDLE*				m_pTBlend;


	// 그림자맵 렌더타깃
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
	bool					m_bDebugBound;
	HWND					m_hWndPresent;

	RECT				m_Rect;
	GUIVERTEX			 g_vertices[4];
public:	
	void SetHLSL(std::string val) { m_strHLSL = val; }
	void SetViewPortInfo(const D3DVIEWPORT9& val) { m_viewPortInfo = val; }
	bool Init(HWND hWndPresent,bool bWindowed,int width,int height);
	void Finalize();

	void LoadHLSL(const char* szFileName);

	void RenderDebugString(int x,int y,const char* szText);
	LPD3DXEFFECT GetEffect() const { return m_pEffect; }

	void Begin();
	void End();

	void SetEffectVector_WorldLightPosition(D3DXVECTOR4* pVec);
	void SetEffectMatirx_LightView(D3DXMATRIX* pMat);
	void SetEffectMatirx_LightProjection(D3DXMATRIX* pMat);

	void ArrangeTechnique(unsigned int index,D3DXHANDLE hTechnique);
	void SetPos( UINT x,UINT y );
};


}