#pragma once

#include <d3dx9.h>
#include <list>
#include <vector>

using namespace std;

typedef WORD SUBMATINDEX;

/*
노드는 아직 아니다.
*/
class Texture;

class cMaterialEx:
	public D3DMATERIAL9
{
public:
	cMaterialEx(void);
	virtual ~cMaterialEx(void);	
	cMaterialEx(const cMaterialEx &Other);
public:
	float					Multiply;				//power의 곱
	float					Transparency;			//메쉬 투명도
private:		
	Texture*				m_pTexture;
	
	vector<cMaterialEx>		m_arrSubMaterial;
public:
	cMaterialEx& operator=(const cMaterialEx &Other);

	BOOL					IsEmptySubMaterial() { return m_arrSubMaterial.empty(); }
	Texture*				GetTexture() const { return m_pTexture; }
	void					SetTexture(Texture* val) { m_pTexture = val; }
	cMaterialEx*			GetSubMaterial(UINT index);
	void					InsertSubMaterial(cMaterialEx& SubMatrial);


};



