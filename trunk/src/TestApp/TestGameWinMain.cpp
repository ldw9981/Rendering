#include "TestGameApp.h"
#include <windows.h>
#include <tchar.h>

#include <boost/shared_ptr.hpp>



int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{

	Application::TestGameApp TestGameApp(L"TestGameApp",FALSE,1024,768);
	if(TestGameApp.Open())
	{
		TestGameApp.Run();
	}	
	TestGameApp.Close();



	
	


	return 0;
}