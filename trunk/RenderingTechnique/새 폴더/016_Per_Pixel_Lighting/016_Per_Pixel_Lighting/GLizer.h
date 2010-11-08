#ifndef _GLIZER_H_
#define _GLIZER_H_

// This file is part of work done by Emil Persson, aka Humus.
// It is little modify by zanir

#include "init.h"


class GLizer {
protected:
	vec currentNormal;
	vec4 currentColor;
	float currentTexCoords[32];

	DWORD fvf;
	D3DPRIMITIVETYPE type;

	unsigned char *fvfBuffer;
	unsigned int bufferSize;
	unsigned int nVertices;
	unsigned int vertexSize;

	bool normals;
	bool colors;
	int nTextures;
public:

	inline GLizer();
	inline ~GLizer();

	inline void glBegin(D3DPRIMITIVETYPE Type, bool Normals, bool Colors, int NTextures);
	inline void glEnd();

	inline void glVertex(vec &vertex);
	inline void glVertex(float x, float y, float z);
	inline void glNormal(vec &normal);
	inline void glColor(vec4 &color);
	inline void glTexCoord(float s, float t, float r, float q);
	inline void glMultiTexCoord(int textureUnit, float s, float t, float r, float q);
};

inline GLizer::GLizer(){
	currentNormal = vec(0,0,1);
	currentColor  = vec4(1,1,1,1);
	ZeroMemory(currentTexCoords, sizeof(currentTexCoords));

	fvfBuffer = (unsigned char *) malloc(bufferSize = 64);
}

inline GLizer::~GLizer(){
	if (fvfBuffer != NULL) free(fvfBuffer);
}

inline void GLizer::glBegin(D3DPRIMITIVETYPE Type, bool Normals, bool Colors, int NTextures){
	nVertices = 0;
	normals = Normals;
	colors = Colors;
	nTextures = NTextures;


	fvf = D3DFVF_XYZ;
	vertexSize = 12;
	if (normals){
		fvf |= D3DFVF_NORMAL;
		vertexSize += 12;
	}
	if (colors){
		fvf |= D3DFVF_DIFFUSE;
		vertexSize += 4;
	}

	for (int i = 0; i < nTextures; i++){
		fvf |= D3DFVF_TEXCOORDSIZE4(i);
	}

	fvf |= nTextures << D3DFVF_TEXCOUNT_SHIFT;

	vertexSize += nTextures * 4 * sizeof(float);

	type = Type;
}

inline void GLizer::glEnd(){
    pd->SetVertexShader(fvf);

	int nPrimitives;
	switch (type){
	case D3DPT_POINTLIST:
		nPrimitives = nVertices;
		break;
    case D3DPT_LINELIST:
		nPrimitives = nVertices / 2;
		break;
    case D3DPT_LINESTRIP:
		nPrimitives = nVertices - 1;
		break;
    case D3DPT_TRIANGLELIST:
		nPrimitives = nVertices / 3;
		break;
    case D3DPT_TRIANGLESTRIP:
    case D3DPT_TRIANGLEFAN:
		nPrimitives = nVertices - 2;
		break;
	default:
		nPrimitives = 0;
	}
	pd->DrawPrimitiveUP(type, nPrimitives, fvfBuffer, vertexSize);
}

inline void GLizer::glVertex(vec &vertex){
	if ((nVertices + 1) * vertexSize > bufferSize){
		fvfBuffer = (unsigned char *) realloc(fvfBuffer, bufferSize = (nVertices + 1) * vertexSize);
	}
	unsigned char *dest = fvfBuffer + nVertices * vertexSize;

	*((vec *) dest) = vertex;
	dest += 12;

	if (normals){
		*((vec *) dest) = currentNormal;
		dest += 12;
	}

	if (colors){
		*((unsigned int *) dest) = currentColor.getRGBA();
		dest += 4;
	}

	if (nTextures > 0){
		memcpy(dest, currentTexCoords, nTextures * 4 * sizeof(float));
		dest += nTextures * 4 * sizeof(float);
	}


	nVertices++;
}

inline void GLizer::glVertex(float x, float y = 0.0f, float z = 0.0f){
	glVertex(vec(x,y,z));
}


inline void GLizer::glNormal(vec &normal){
	currentNormal = normal;
}

inline void GLizer::glColor(vec4 &color){
	currentColor = color;
}

inline void GLizer::glTexCoord(float s, float t = 0, float r = 0, float q = 1){
	currentTexCoords[0] = s;
	currentTexCoords[1] = t;
	currentTexCoords[2] = r;
	currentTexCoords[3] = q;
}

inline void GLizer::glMultiTexCoord(int textureUnit, float s, float t = 0, float r = 0, float q = 1){
	currentTexCoords[textureUnit * 4    ] = s;
	currentTexCoords[textureUnit * 4 + 1] = t;
	currentTexCoords[textureUnit * 4 + 2] = r;
	currentTexCoords[textureUnit * 4 + 3] = q;
}

#endif // _GLIZER_H_