#pragma once

typedef WORD SUBMATINDEX;

/*
���� ���� �ƴϴ�.
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
	float					Multiply;				//power�� ��
	float					Transparency;			//�޽� ����

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



