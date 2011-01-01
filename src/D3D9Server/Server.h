#pragma once
#include "Framework/StaticHWND.h"
#include "D3D9Server/StaticD3DDevice9.h"


#define USE_EFFECT

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
	D3DVIEWPORT9			m_BaseViewPort;
	D3DPRESENT_PARAMETERS	m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D9;
	LPD3DXFONT				m_pFont;
	LPD3DXEFFECT			m_pEffect;	
	LPD3DXBUFFER			m_pError;	
	D3DXHANDLE						m_hTechnique;// 테크닉
	D3DXHANDLE						m_hmWVP;	// 월드~투영행렬
	D3DXHANDLE						m_hmProjection;	// 월드~투영행렬
	D3DXHANDLE						m_hmView;	// 월드~투영행렬
	D3DXHANDLE						m_hmWorld;	// 월드~투영행렬
	D3DXHANDLE						m_hmViewProjection;
	D3DXHANDLE						m_hvLightDir;

	cGUIFont*					m_pNewFont;
	string m_strHLSL;

	void SetHLSL(std::string val) { m_strHLSL = val; }
public:	

	bool Init();
	void Uninit();
	
	void LoadHLSL(const char* szFileName);
	
	void RenderDebugString(const char* szText);
	LPD3DXEFFECT GetEffect() const { return m_pEffect; }
};
	

}
