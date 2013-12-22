#pragma once
#include "Framework/StaticHWND.h"
#include "D3D9Server/StaticD3DDevice9.h"
#include "Scene/RendererQueue.h"



class cGUIFont;

namespace D3D9
{

	
class Server:
		public StaticD3DDEVICE9,
		public StaticHWND
{
public:
	Server(void);
	~Server(void);

public:
	static Server* g_pServer;

	// D3D 변수추가	
	D3DVIEWPORT9			m_viewPortInfo;
	void SetViewPortInfo(const D3DVIEWPORT9& val) { m_viewPortInfo = val; }
	D3DPRESENT_PARAMETERS	m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D9;
	LPD3DXFONT				m_pFont;
	LPD3DXEFFECT			m_pEffect;	
	LPD3DXBUFFER			m_pError;	
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclationNormal;
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclationBlend;
	D3DXHANDLE				m_hTSkinningPhong;// 테크닉	
	D3DXHANDLE				m_hTSkinningPhongDiffuse;// 테크닉	
	D3DXHANDLE				m_hTPhong;// 테크닉
	D3DXHANDLE				m_hTPhongDiffuse;// 테크닉
	D3DXHANDLE				m_hTPhongDiffuseLight;
	D3DXHANDLE				m_hTPhongDiffuseBump;


	D3DXHANDLE				m_hmWVP;	// 월드~투영행렬
	D3DXHANDLE				m_hmProjection;	// 월드~투영행렬
	D3DXHANDLE				m_hmView;	// 월드~투영행렬
	D3DXHANDLE				m_hmWorld;	// 월드~투영행렬
	D3DXHANDLE				m_hmViewProjection;
	D3DXHANDLE				m_hvLightDir;
	D3DXHANDLE				m_hmPalette;
	D3DXHANDLE				m_hvWorldLightPosition;
	D3DXHANDLE				m_hvWorldCameraPosition;;

	cGUIFont*					m_pNewFont;
	std::string m_strHLSL;


	cRendererQueue			m_listRenderQueue[16];
	cRendererQueue			m_listRenderQueueSkinned[16];

	void SetHLSL(std::string val) { m_strHLSL = val; }
public:	

	bool Init();
	void Uninit();
	
	void LoadHLSL(const char* szFileName);
	
	void RenderDebugString(const char* szText);
	LPD3DXEFFECT GetEffect() const { return m_pEffect; }

	void Render();
	void Begin();
	void End();
};
	

}
