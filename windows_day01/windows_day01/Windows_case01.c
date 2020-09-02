#include<windows.h>
//windows应用程序
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hpreinstance, PSTR szCmdline, int iCmdShow)
{
	MessageBox(NULL, TEXT("我的第一个demo"), TEXT("测试\n"), MB_OK);
	//HANDLE hfile = CreateFile(TEXT("D:\windowsdemo_file"), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	return 0;
}
