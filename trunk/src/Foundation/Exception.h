#pragma once





//����ó�����������޴�Ŭ���� ���ܳ���� throw�� ȣ��� �ٹ�ȣ�� �������ش�.

class Exception

{

	char m_szText[256];

	int                       m_line;

public:

	Exception(char* szText,int Line)
	{
		strncpy_s(m_szText, szText,strlen(szText));
		m_line =Line;
	}

	~Exception()
	{		

	}

	char* GetText(){return m_szText;}
	int GetLine(){return m_line;}
};
/*


void main() {

try          {

cout<<" ����ó���ϴ�Ŭ��������"    <<endl;

throw CException("main()",__LINE__);

cout<< "throw�������ؽ�������ʴ´�."<<endl;

}

catch(CException& e)

{

cout<<"Excepton "<<e.GetName()

<<"at line " <<e.GetLine()<<endl;

}



}


*/