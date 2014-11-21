#pragma once

#include "RendererQueue.h"
#include "Vertex.h"
namespace Sophia
{
	
class cGUIFont;
class cView;
class World;
class cRscVertexBuffer;
class cRscTexture;

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

public:
	static Graphics* m_pInstance;
	static LPDIRECT3DDEVICE9	m_pDevice;

	int						m_width;
	int						m_height;
	// D3D 변수추가		
	D3DPRESENT_PARAMETERS	m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D9;
	LPD3DXFONT				m_pFont;
	LPD3DXEFFECT			m_pEffect;	
	LPD3DXBUFFER			m_pError;	
	LPDIRECT3DVERTEXDECLARATION9 m_pNormalVertexDeclaration;
	LPDIRECT3DVERTEXDECLARATION9 m_pSkinnedVertexDeclaration;
	LPDIRECT3DVERTEXDECLARATION9 m_pNormalInstancingVertexDeclaration;
	LPDIRECT3DVERTEXDECLARATION9 m_pSkinnedInstancingVertexDeclaration;
	D3DXHANDLE				m_hTLine;
	D3DXHANDLE				m_hTSceneSkinned;
	D3DXHANDLE				m_hTSceneSkinnedDiffuse;
	D3DXHANDLE				m_hTSceneSkinnedInstancingDiffuse;
	D3DXHANDLE				m_hTerrain;
	D3DXHANDLE				m_hTSceneNormal;
	D3DXHANDLE				m_hTSceneNormalDiffuse;
	D3DXHANDLE				m_hTSceneNormalDiffuseLight;
	D3DXHANDLE				m_hTSceneNormalDiffuseBump;
	D3DXHANDLE				m_hTSceneNormalDiffuseSpecular;
	D3DXHANDLE				m_hTSceneNormalDiffuseOpacity;
	D3DXHANDLE				m_hTSceneNormalDiffuseBumpSpecular;

	D3DXHANDLE				m_hTSceneNormalInstancingDiffuse;
	D3DXHANDLE				m_hTSceneNormalInstancingDiffuseOpacity;
	D3DXHANDLE				m_hTSceneNormalInstancingDiffuseLight;
	D3DXHANDLE				m_hTNormalVertexTransform;
	D3DXHANDLE				m_hTSkinnedVertexTransform;


	D3DXHANDLE				m_hTShadowNormal;	
	D3DXHANDLE				m_hTShadowNormalAlphaTest;	
	D3DXHANDLE				m_hTShadowNormalInstancing;	
	D3DXHANDLE				m_hTShadowNormalInstancingAlphaTest;	
	
	D3DXHANDLE				m_hTShadowSkinned;	
	D3DXHANDLE				m_hTShadowSkinnedAlphaTest;
	D3DXHANDLE				m_hTShadowSkinnedInstancing;	
	D3DXHANDLE				m_hTShadowSkinnedInstancingAlphaTest;	

	D3DXHANDLE				m_hTGUI;

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
	D3DXHANDLE				m_hfMatrixTextureSize;
	cGUIFont*					m_pNewFont;
	std::string m_strHLSL;

	int						m_nTechniqueSize;
	std::vector<D3DXHANDLE>	m_vecTechniqueNormal;
	std::vector<D3DXHANDLE>	m_vecTechniqueSkinned;
	std::vector<D3DXHANDLE>	m_vecTechniqueNormalInstancing;
	std::vector<D3DXHANDLE>	m_vecTechniqueSkinnedInstancing;

	bool					m_bDebugBound;
	HWND					m_hWndPresent;

	RECT				m_Rect;
	GUIVERTEX			 g_vertices[4];	
	D3DCAPS9			m_caps;
	std::vector<LPDIRECT3DSURFACE9> m_vecRenderTarget;
	LPDIRECT3DSURFACE9	m_depthStencilSurface;

	std::map<std::string,int>	m_mapInstancingMax;
	int							m_defaultInstancingMax;
public:	
	void SetHLSL(std::string val) { m_strHLSL = val; }
//	void SetViewPortInfo(const D3DVIEWPORT9& val) { m_viewPortInfo = val; }
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

	void BackupRenderTarget(unsigned int renderTargetIndex);
	void RestoreRenderTarget(unsigned int renderTargetIndex);

	void BackupDepthStencilSurface();
	void RestoreDepthStencilSurface();

	void SetEntityInstancingMax(const char* entityName,int max);
	int  GetEntityInstancingMax(const char* entityName);
};


}