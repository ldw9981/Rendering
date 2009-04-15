#pragma once

typedef WORD SUBMATINDEX;

/*
노드는 아직 아니다.
*/

// D3DCOLORVALUE   Diffuse;        /* Diffuse color RGBA */
// D3DCOLORVALUE   Ambient;        /* Ambient color RGB */
// D3DCOLORVALUE   Specular;       /* Specular 'shininess' */
// D3DCOLORVALUE   Emissive;       /* Emissive color RGB */	<- 3DMAX에서의 Data는 없음
// float           Power;          /* Sharpness if specular highlight */
class cRscTexture;
class cMaterialEx:
	public D3DMATERIAL9
{
public:
	cMaterialEx(void);
	virtual ~cMaterialEx(void);	
	cMaterialEx(const cMaterialEx &Other);
public:	
	float					SpecularMultiplier;
	float					Alpha;			//메쉬 투명도
private:		
	cRscTexture*			m_pDiffuseRscTexture;
	cRscTexture*			m_pBumpRscTexture;
	cRscTexture*			m_pOpacityRscTexture;

	
	vector<cMaterialEx>		m_arrSubMaterial;
public:
	cMaterialEx& operator=(const cMaterialEx &Other);

	BOOL					IsEmptySubMaterial() { return m_arrSubMaterial.empty(); }
	cMaterialEx*			GetSubMaterial(UINT index);
	void					InsertSubMaterial(cMaterialEx& SubMatrial);



	cRscTexture*			GetDiffuseRscTexture() const { return m_pDiffuseRscTexture; }
	void					SetDiffuseRscTexture(cRscTexture* val) { m_pDiffuseRscTexture = val; }

	cRscTexture*			GetBumpRscTexture() const { return m_pBumpRscTexture; }
	void					SetBumpRscTexture(cRscTexture* val) { m_pBumpRscTexture = val; }

	cRscTexture*			GetOpacityRscTexture() const { return m_pOpacityRscTexture; }
	void					SetOpacityRscTexture(cRscTexture* val) { m_pOpacityRscTexture = val; }
};

class cMaterialEx2:
	public D3DMATERIAL9
{
public:
	cMaterialEx2(void);
	virtual ~cMaterialEx2(void);		
public:	
	float					SpecularMultiplier;
	float					Alpha;			//메쉬 투명도
private:		
	cRscTexture*			m_pDiffuseRscTexture;
	cRscTexture*			m_pBumpRscTexture;
	cRscTexture*			m_pOpacityRscTexture;
	vector<cMaterialEx2*>	m_arrSubMaterial;
public:
	BOOL					IsEmptySubMaterial() { return m_arrSubMaterial.empty(); }
	cMaterialEx2*			GetSubMaterial(UINT index);
	void					InsertSubMaterial(cMaterialEx2* SubMatrial);



	cRscTexture*			GetDiffuseRscTexture() const { return m_pDiffuseRscTexture; }
	void					SetDiffuseRscTexture(cRscTexture* val) { m_pDiffuseRscTexture = val; }

	cRscTexture*			GetBumpRscTexture() const { return m_pBumpRscTexture; }
	void					SetBumpRscTexture(cRscTexture* val) { m_pBumpRscTexture = val; }

	cRscTexture*			GetOpacityRscTexture() const { return m_pOpacityRscTexture; }
	void					SetOpacityRscTexture(cRscTexture* val) { m_pOpacityRscTexture = val; }
};




