#pragma once

namespace Sophia
{


#define	MAX_SIZE_OF_BUFFER					65536

// �⺻ Identifier
#define	TOKEND_NUMBER						1001
#define	TOKEND_STRING						1002
#define	TOKEND_BLOCK_START					1003
#define	TOKEND_BLOCK_END					1004

#define	TOKEND_END							65534
#define	TOKEND_IDENTIFIER					65535
#define	TOKEND_NOTDEFINED					65536

class cLexer
{
protected:
	// ������ �о �۾��ϱ� ���� ������
	HANDLE				m_File;																// ������ �ڵ�

	// Buffers~~
	DWORD				m_Pointer;															// ���� Buffer�� �а� �ִ� ���ڿ��� Pointer
	DWORD				m_NowLoaded;														// ���� �о� �鿩���� ũ��

	DWORD				m_SizeOfBuffer;														// Buffer�� ũ��
	CHAR*				m_Char;																// ������ ������ �о� ���� Buffer

	DWORD				m_TotalFileSize;													// ������ ��ü ũ��
	DWORD				m_NowProgress;														// ���� ����.

	std::list<std::string> m_tokenHistory;

public:
	cLexer(const char* p_FileName=NULL, DWORD p_SizeOfBuffer=MAX_SIZE_OF_BUFFER);
	virtual ~cLexer();
	
			CHAR		GetNextChar();

			BOOL		Open(const char* p_FileName, DWORD p_SizeOfBuffer=MAX_SIZE_OF_BUFFER);	// ������ ����.
	virtual	void		Close();															// �ݾƹ�����.
			BOOL		Reset();															// ó������ �ٽ� �����Ѵ�.

	virtual	LONG		GetToken(LPSTR p_TokenString);

	virtual void		AddTokenHistory(int identifier);	
};

}