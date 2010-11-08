#pragma once

typedef WORD SUBMATINDEX;


class cRscTexture;
class cMaterialNode:
	public D3DMATERIAL9
{
public:
	cMaterialNode(void);
	virtual ~cMaterialNode(void);	
	cMaterialNode(const cMaterialNode &Other);
public:
	float					Multiply;				//power의 곱
	float					Transparency;			//메쉬 투명도
private:		
	cRscTexture*			m_pTexture;
	
	vector<cMaterialNode>	m_arrSubMaterial;
public:
	BOOL					IsEmptySubMaterial() { return m_arrSubMaterial.empty(); }
	cRscTexture*			GetRscTexture() const { return m_pTexture; }
	void					SetRscTexture(cRscTexture* val) { m_pTexture = val; }
	cMaterialNode*			GetSubMaterial(UINT index);
	void					InsertSubMaterial(cMaterialNode& SubMatrial);

	cMaterialNode &operator =(const cMaterialNode &Other);
};



