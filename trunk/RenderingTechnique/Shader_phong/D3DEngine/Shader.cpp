#include "stdafx.h"
#include "Shader.h"
LPDIRECT3DDEVICE9 g_pD3DDevice9=NULL; 
LPDIRECT3DVERTEXSHADER9 g_pVertexShader=NULL;
LPDIRECT3DPIXELSHADER9 g_pPixelShader=NULL;

D3DLIGHT9		g_Light;
D3DMATERIAL9	g_Matrial;

BOOL  LoadVertexShader(LPCTSTR p_VetexShaderFileName)
{
	LPD3DXBUFFER        pCode;
	LPD3DXBUFFER        pError;

	if(D3DXAssembleShaderFromFile(p_VetexShaderFileName, NULL, NULL, D3DXSHADER_DEBUG, &pCode, &pError) != D3D_OK)
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		MessageBoxA(NULL,(LPCSTR)pError->GetBufferPointer(),MB_OK,0);
		return	FALSE;
	}

	if(g_pD3DDevice9->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &g_pVertexShader) != D3D_OK)
	{
	
		return	FALSE;
	}
	g_pD3DDevice9->SetVertexShader(g_pVertexShader);
	return TRUE;
}

BOOL  LoadPixelShader(LPCTSTR p_PixelShaderFileName)
{
	LPD3DXBUFFER        pCode;
	LPD3DXBUFFER        pError;

	if(D3DXAssembleShaderFromFile(p_PixelShaderFileName, NULL, NULL, D3DXSHADER_DEBUG, &pCode, &pError) != D3D_OK)
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		MessageBoxA(NULL,(LPCSTR)pError->GetBufferPointer(),MB_OK,0);
		return	FALSE;
	}

	if(g_pD3DDevice9->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &g_pPixelShader) != D3D_OK)
	{
		return	FALSE;
	}
	g_pD3DDevice9->SetPixelShader(g_pPixelShader);
	return TRUE;
}

void SetupMatrialLight()
{

	ZeroMemory(&g_Matrial,sizeof(D3DMATERIAL9));
	g_Matrial.Ambient.r = 1.0f;
	g_Matrial.Ambient.g = 1.0f;
	g_Matrial.Ambient.b = 1.0f;
	g_Matrial.Ambient.a = 1.0f;

	g_Matrial.Diffuse.r = 1.0f;
	g_Matrial.Diffuse.g = 1.0f;
	g_Matrial.Diffuse.b = 1.0f;
	g_Matrial.Diffuse.a = 1.0f;

	g_Matrial.Specular.r = 1.0f;
	g_Matrial.Specular.g = 1.0f;
	g_Matrial.Specular.b = 1.0f;
	g_Matrial.Specular.a = 1.0f;

	g_Matrial.Power =4.0f;

	g_pD3DDevice9->SetMaterial(&g_Matrial);


	ZeroMemory(&g_Light,sizeof(D3DLIGHT9));
	g_Light.Type	=	D3DLIGHT_DIRECTIONAL;

	g_Light.Ambient.r = 0.3f;
	g_Light.Ambient.g = 0.3f;
	g_Light.Ambient.b = 0.3f;
	g_Light.Ambient.a = 1.0f;

	g_Light.Diffuse.r = 1.0f;
	g_Light.Diffuse.g = 1.0f;
	g_Light.Diffuse.b = 1.0f;
	g_Light.Diffuse.a = 1.0f;

	g_Light.Specular.r = 1.0f;
	g_Light.Specular.g = 1.0f;
	g_Light.Specular.b = 1.0f;
	g_Light.Specular.a = 1.0f;


	g_Light.Direction.x = 0.5f;
	g_Light.Direction.y = -0.5f;
	g_Light.Direction.z = 1.0f;


	g_pD3DDevice9->SetPixelShaderConstantF(0,(float*)&g_Matrial.Ambient,1);
	g_pD3DDevice9->SetPixelShaderConstantF(1,(float*)&g_Matrial.Diffuse,1);
	g_pD3DDevice9->SetPixelShaderConstantF(2,(float*)&g_Matrial.Specular,1);
	g_pD3DDevice9->SetPixelShaderConstantF(3,(float*)&g_Matrial.Power,1);

	g_pD3DDevice9->SetPixelShaderConstantF(4,(float*)&g_Light.Ambient,1);
	g_pD3DDevice9->SetPixelShaderConstantF(5,(float*)&g_Light.Diffuse,1);
	g_pD3DDevice9->SetPixelShaderConstantF(6,(float*)&g_Light.Specular,1);	
	
	g_pD3DDevice9->SetVertexShaderConstantF(12,(float*)&g_Light.Direction,1);
}