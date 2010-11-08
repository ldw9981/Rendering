#pragma once

typedef WORD SUBMATINDEX;

/*
노드는 아직 아니다.
*/



class cRscTexture;
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
	cRscTexture*			m_pTexture;

public:
	cMaterialEx& operator=(const cMaterialEx &Other);


	cRscTexture*			GetRscTexture() const { return m_pTexture; }
	void					SetRscTexture(cRscTexture* val) { m_pTexture = val; }



};



