#pragma once
#include "Foundation/interface.h"
#include "Resource/Resource.h"

namespace Sophia
{

typedef unsigned char SUBMATINDEX;

/*
노드는 아직 아니다.
*/

class cRscTexture;
class Material
	: ISerializable
{
public:
	enum MAP
	{
		DIFFUSE	=0x000,
		NORMAL	=0x001,		
		LIGHT	=0x002,
		OPACITY =0x003,
		SPECULAR=0x004,
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
	float					Multiply;				//power의 곱
	float					Transparency;			//메쉬 투명도
	bool					AlphaBlend;
	bool					AlphaSort;
	bool					AlphaTestEnable;
	unsigned char			AlphaTestRef;
	unsigned char			AlphaTestOp;
	std::bitset<MAX>			index_renderer_queue_;
private:		
	cRscTexture*			m_pMapDiffuse;
	cRscTexture*			m_pMapNormal;
//	cRscTexture*			m_pMapRefract;	
	cRscTexture*			m_pMapLight;
	cRscTexture*			m_pMapOpacity;
	cRscTexture*			m_pMapSpecular;
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
// 	cRscTexture*			GetMapRefract() const;
// 	void					SetMapRefract(cRscTexture* val);
	cRscTexture*			GetMapLight() const;
	void					SetMapLight(cRscTexture* val);
	cRscTexture*			GetMapOpacity() const;
	void					SetMapOpacity(cRscTexture* val);
	cRscTexture*			GetMapSpecular() const;
	void					SetMapSpecular(cRscTexture* val);

	virtual void SerializeIn(std::ifstream& stream);
	virtual void SerializeOut(std::ofstream& stream);
};

typedef std::vector<Material*> SubMaterial;

class  EntityMaterial
	: ISerializable
	,public cResource
{
public:
	EntityMaterial(void);
	~EntityMaterial(void);
public:
	std::vector<SubMaterial> m_ref;
public:
	virtual void SerializeIn(std::ifstream& stream);
	virtual void SerializeOut(std::ofstream& stream);

	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();
private:
};


}