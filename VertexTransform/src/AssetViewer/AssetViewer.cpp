// AssetViewer.cpp : �⺻ ������Ʈ �����Դϴ�.

#include "stdafx.h"
#include "MainForm.h"
#include "Form1.h"
using namespace AssetViewer;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// ��Ʈ���� ��������� ���� Windows XP �ð� ȿ���� Ȱ��ȭ�մϴ�.
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �� â�� ����� �����մϴ�.
	Application::Run(gcnew MainForm());
	return 0;
}
