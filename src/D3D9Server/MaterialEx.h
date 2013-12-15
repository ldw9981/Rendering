#pragma once

typedef WORD SUBMATINDEX;

/*
���� ���� �ƴϴ�.
*/

class cRscTexture;
class Material
{
public:
	enum MAP
	{
		DIFFUSE=0,
		NORMAL,		
		LIGHT,
		REFRACT,
		MAX
	};
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

	std::bitset<4>			index_renderer_queue_;
private:		
	cRscTexture*			m_pMapDiffuse;
	cRscTexture*			m_pMapNormal;
	cRscTexture*			m_pMapRefract;	
	cRscTexture*			m_pMapLight;
	//Light
	//Specular
	//Reflection
	//Self Illumination
public:
	unsigned long			index_renderer_queue() const { return index_renderer_queue_.to_ulong(); }
	cRscTexture*			GetMapDiffuse() const;
	void					SetMapDiffuse(cRscTexture* val);
	cRscTexture*			GetMapNormal() const;
	void					SetMapNormal(cRscTexture* val);
	cRscTexture*			GetMapRefract() const;
	void					SetMapRefract(cRscTexture* val);
	cRscTexture*			GetMapLight() const;
	void					SetMapLight(cRscTexture* val);
};



