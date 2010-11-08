#pragma once
#include "Framework/StaticHWND.h"
#include "D3D9Server/StaticD3DDevice9.h"


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

	// D3D 변수추가	
	D3DVIEWPORT9			m_BaseViewPort;
	D3DPRESENT_PARAMETERS	m_D3DPP;         // Parameters for CreateDevice/Reset
	LPDIRECT3D9				m_pD3D9;
	LPD3DXFONT				m_pFont;
	LPD3DXEFFECT			m_pEffect;
	LPD3DXBUFFER			m_pError;

	cGUIFont*					m_pNewFont;
	string m_strHLSL;

	void SetHLSL(std::string val) { m_strHLSL = val; }
public:	

	bool Init();
	void Uninit();
	
	void LoadHLSL(const char* szFileName);
	
	void RenderDebugString(const char* szText);
};

}
