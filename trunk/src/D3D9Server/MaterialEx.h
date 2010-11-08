#pragma once

typedef WORD SUBMATINDEX;

/*
���� ���� �ƴϴ�.
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
	float					Multiply;				//power�� ��
	float					Transparency;			//�޽� ����
private:		
	cRscTexture*			m_pTexture;

public:
	cMaterialEx& operator=(const cMaterialEx &Other);


	cRscTexture*			GetRscTexture() const { return m_pTexture; }
	void					SetRscTexture(cRscTexture* val) { m_pTexture = val; }



};



