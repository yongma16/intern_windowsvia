#include<iostream>
#include<Windows.h>
//windows����Ӧ�ó���
int WINAPI _tWinMain(HINSTANCE hInstanceexe,HINSTANCE,PTSTR pszdCmdline,int)
{
	HANDLE hfile = CreateFile(TEXT(""), 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	return 0;
}