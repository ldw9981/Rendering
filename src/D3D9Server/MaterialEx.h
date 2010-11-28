#pragma once

typedef WORD SUBMATINDEX;

/*
노드는 아직 아니다.
*/



class cRscTexture;
class Material
{
public:
	Material(void);
	virtual ~Material(void);	
	Material(const Material &Other);

	Material&  operator=(const Material &Other);
public:
	D3DCOLORVALUE			Diffuse;        /* Diffuse color RGBA */
	D3DCOLORVALUE			Ambient;        /* Ambient color RGB */
	D3DCOLORVALUE			Specular;       /* Specular 'shininess' */
	D3DCOLORVALUE			Emissive;       /* Emissive color RGB */
	float					Power;          /* Sharpness if specular highlight */
	float					Multiply;				//power의 곱
	float					Transparency;			//메쉬 투명도
private:		
	cRscTexture*			m_pMapDiffuse;
	cRscTexture*			m_pMapBump;
	cRscTexture*			m_pMapRefract;

public:
	cRscTexture*			GetMapDiffuse() const;
	void					SetMapDiffuse(cRscTexture* val);
	cRscTexture*			GetMapBump() const;
	void					SetMapBump(cRscTexture* val);
	cRscTexture*			GetMapRefract() const;
	void					SetMapRefract(cRscTexture* val);

};



