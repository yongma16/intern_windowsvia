#include<windows.h>
//windowsӦ�ó���
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hpreinstance, PSTR szCmdline, int iCmdShow)
{
	MessageBox(NULL, TEXT("�ҵĵ�һ��demo"), TEXT("����\n"), MB_OK);
	//HANDLE hfile = CreateFile(TEXT("D:\windowsdemo_file"), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	return 0;
}
