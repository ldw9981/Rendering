#pragma once





//예외처리시정보를받는클래스 예외내용과 throw의 호출된 줄번호를 저장해준다.

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

cout<<" 예외처리하는클래스연습"    <<endl;

throw CException("main()",__LINE__);

cout<< "throw의해의해실행되지않는다."<<endl;

}

catch(CException& e)

{

cout<<"Excepton "<<e.GetName()

<<"at line " <<e.GetLine()<<endl;

}



}


*/