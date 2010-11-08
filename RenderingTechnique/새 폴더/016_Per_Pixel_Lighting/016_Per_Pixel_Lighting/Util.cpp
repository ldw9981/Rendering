// Util.cpp: implementation of the Util class.
//
//////////////////////////////////////////////////////////////////////

#include "Util.h"

//////////////////////////////////////////////////////////////////////
// uVertexShader Class
//////////////////////////////////////////////////////////////////////

int uVertexShader::LoadVS(char *filename, DWORD *pDeclaration, char *verprog)
{
	int er=0;
	// Create the vertex shader.
	LPD3DXBUFFER pCode;                             // assembled shader code
	LPD3DXBUFFER pError;
	if(hVertexShader!=NULL)pd->DeleteVertexShader( hVertexShader);
	if(filename==NULL)
		D3DXAssembleShader( verprog, strlen(verprog), 0, NULL, &pCode, &pError );  // assemble shader code
	else
	{
		if( D3DXAssembleShaderFromFile( filename, 0, NULL, &pCode, &pError) != D3D_OK)er=1;  // assemble shader code
	}

	if(pError!=NULL)
	{
		char text[150];
		char* error_text = new char[pError->GetBufferSize()+1];
		memcpy(error_text,pError->GetBufferPointer(),pError->GetBufferSize());
		error_text[pError->GetBufferSize()]=NULL;
		pError->Release();
		
		if(filename!=NULL)
			sprintf( text, "ERROR in vertex shader: %s", filename);
		else
			sprintf( text, "ERROR in vertex shader: %s");

		MessageBox(hWnd, error_text, text, MB_OK);
		delete [] error_text;
		hVertexShader = NULL;
		return 0;
	}
	else if(er==1)
	{
		MessageBox(hWnd, filename, english ? "Couldn't find file for vertex shader":"Nenajdeny subor pre vertex shader", MB_OK);
		return 0;
	}

	if(	pd->CreateVertexShader( pDeclaration, (DWORD*)pCode->GetBufferPointer(), &hVertexShader, software_vertexprocessing )
		== D3D_OK )
	{
		pCode->Release();
		return 1;
	}
	pCode->Release();
	MessageBox(hWnd, filename, english ? "ERROR in vertex shader":"Chyba pri nacitavani vertex shader", MB_OK);
	return 0;
}

//////////////////////////////////////////////////////////////////////
// uPixelShader Class
//////////////////////////////////////////////////////////////////////

int uPixelShader::LoadPS(char *filename, char *pixelprog)
{
	int er=0;
	// Create the pixel shader.
	LPD3DXBUFFER pCode;                             // assembled shader code
	LPD3DXBUFFER pError;
	if(hPixelShader!=NULL) pd->DeletePixelShader( hPixelShader);
	if(filename==NULL)
		D3DXAssembleShader( pixelprog, strlen(pixelprog), 0, NULL, &pCode, &pError );  // assemble shader code
	else
	{
		if( D3DXAssembleShaderFromFile( filename, 0, NULL, &pCode, &pError) != D3D_OK)er=1;  // assemble shader code
	}

	if(pError!=NULL)
	{
		char text[150];
		char* error_text = new char[pError->GetBufferSize()+1];
		memcpy(error_text,pError->GetBufferPointer(),pError->GetBufferSize());
		error_text[pError->GetBufferSize()]=NULL;
		pError->Release();

		if(filename!=NULL)
			sprintf( text, "ERROR in pixel shader: %s", filename);
		else
			sprintf( text, "ERROR in pixel shader: %s");

		MessageBox(hWnd, error_text, text, MB_OK);
		delete [] error_text;
		hPixelShader = NULL;
		return 0;
	}
	else if(er==1)
	{
		MessageBox(hWnd, filename, english ? "Couldn't find file for pixel shader":"Nenajdeny subor pre pixel shader", MB_OK);
		return 0;
	}

	if(	pd->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &hPixelShader )
		== D3D_OK )
	{
		pCode->Release();
		return 1;
	}
	pCode->Release();
	MessageBox(hWnd, filename, english ? "ERROR in pixel shader":"Chyba pri nacitavani pixel shader", MB_OK);
	return 0;
}
