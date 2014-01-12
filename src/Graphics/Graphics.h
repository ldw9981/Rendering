#pragma once

#include "Graphics/StaticD3DDevice9.h"
#include "Scene/RendererQueue.h"

class cGUIFont;
class cView;

class Graphics:
	public StaticD3DDEVICE9
{
public:
	Graphics(void);
	~Graphics(void);

public:
	static Graphics* g_pGraphics;

	// D3D �����߰�	
	D3DVIEWPORT9			m_viewPortInfo;
	void SetViewPortInfo(const D3DVIEWPORT9& val) { m_viewPortInfo = val; }
	D3DPRESENT_PARAMETERS	m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D9;
	LPD3DXFONT				m_pFont;
	LPD3DXEFFECT			m_pEffect;	
	LPD3DXBUFFER			m_pError;	
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclationNormal;
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclationBlend;
	D3DXHANDLE				m_hTLine;// ��ũ��
	D3DXHANDLE				m_hTSkinningPhong;// ��ũ��	
	D3DXHANDLE				m_hTSkinningPhongDiffuse;// ��ũ��	
	D3DXHANDLE				m_hTerrain;// ��ũ��
	D3DXHANDLE				m_hTPhong;// ��ũ��
	D3DXHANDLE				m_hTPhongDiffuse;// ��ũ��
	D3DXHANDLE				m_hTPhongDiffuseLight;
	D3DXHANDLE				m_hTPhongDiffuseBump;
	D3DXHANDLE				m_hTCreateShadowNormal;	// ���̴�
	D3DXHANDLE				m_hTCreateShadowBlend;	// ���̴�
	LPD3DXEFFECT			m_pApplyShadowShader;
	LPD3DXEFFECT			m_pCreateShadowShader;

	D3DXHANDLE				m_hmWVP;	// ����~�������
	D3DXHANDLE				m_hmProjection;	// ����~�������
	D3DXHANDLE				m_hmView;	// ����~�������
	D3DXHANDLE				m_hmWorld;	// ����~�������
	D3DXHANDLE				m_hmViewProjection;
	D3DXHANDLE				m_hvLightDir;
	D3DXHANDLE				m_hmPalette;
	D3DXHANDLE				m_hvWorldLightPosition;
	D3DXHANDLE				m_hvWorldCameraPosition;;

	D3DXHANDLE				m_hmLightView;
	D3DXHANDLE				m_hmLightProjection;

	D3DXVECTOR4				m_WorldLightPosition;
	cGUIFont*					m_pNewFont;
	std::string m_strHLSL;

	D3DXHANDLE				m_hTNormal[16];
	D3DXHANDLE				m_hTBlend[16];
	// �׸��ڸ� ����Ÿ��
	LPDIRECT3DTEXTURE9		m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9		m_pShadowDepthStencil;
	bool					m_bDebugBound;

	void SetHLSL(std::string val) { m_strHLSL = val; }
public:	

	bool Init(bool bWindowed,int width,int height);
	void Finalize();

	void LoadHLSL(const char* szFileName);

	void RenderDebugString(int x,int y,const char* szText);
	LPD3DXEFFECT GetEffect() const { return m_pEffect; }

	void Render(cView* pView);
	void Begin();
	void End();
};
