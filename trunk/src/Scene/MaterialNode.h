#pragma once

#include <d3dx9.h>
#include <list>
#include <vector>

using namespace std;

typedef WORD SUBMATINDEX;

/*
���� ���� �ƴϴ�.
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
	float					Multiply;				//power�� ��
	float					Transparency;			//�޽� ����
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



