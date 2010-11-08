#pragma once

class cTransform
{
public:
	cTransform(void);
	~cTransform(void);
public:
	
	D3DXMATRIX				m_WorldTM;		//							Transform에 의해서만 갱신된다. 
	D3DXMATRIX				m_WorldTMOld;	//							Transform에 의해서만 갱신된다. 

	BOOL					m_bLocalTMModified;

	D3DXMATRIX				m_LocalTM;
	D3DXMATRIX				m_LocalTranslationTM;
	D3DXMATRIX				m_LocalRotationTM;
	D3DXMATRIX				m_LocalScaleTM;

private:

// 	void					SetWorldTM(D3DXMATRIX& val) { m_WorldTM = val; }
// 	void					SetWorldTMOld(D3DXMATRIX& val) { m_WorldTMOld = val; }
public:
	const D3DXMATRIX&		GetWorldTM() const { return m_WorldTM; }
	const D3DXMATRIX&		GetWorldTMOld() const { return m_WorldTMOld; }


	const D3DXMATRIX&		GetLocalTM() const { return m_LocalTM; }
	void					SetLocalTM(D3DXMATRIX& val) { m_bLocalTMModified=TRUE; m_LocalTM = val; }

	const D3DXMATRIX&		GetLocalSTM() const { return m_LocalScaleTM; }
	void					SetLocalSTM(D3DXMATRIX& val) { m_bLocalTMModified=TRUE; m_LocalScaleTM = val; }

	const D3DXMATRIX&		GetLocalRTM() const { return m_LocalRotationTM; }
	void					SetLocalRTM(D3DXMATRIX& val) {m_bLocalTMModified=TRUE; m_LocalRotationTM = val; }

	const D3DXMATRIX&		GetLocalTTM() const { return m_LocalTranslationTM; }
	void					SetLocalTTM(D3DXMATRIX& val) { m_bLocalTMModified=TRUE; m_LocalTranslationTM = val; }

	virtual D3DXMATRIX&		Transform();
};
