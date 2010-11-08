#pragma once

extern LPDIRECT3DDEVICE9 g_pD3DDevice9; 
extern LPDIRECT3DVERTEXSHADER9 g_pVertexShader;
extern LPDIRECT3DPIXELSHADER9 g_pPixelShader;
extern D3DLIGHT9	g_Light;
extern D3DMATERIAL9	g_Matrial;

BOOL  LoadVertexShader(LPCTSTR p_VetexShaderFileName);
BOOL  LoadPixelShader(LPCTSTR p_PixelShaderFileName);
void SetupMatrialLight();