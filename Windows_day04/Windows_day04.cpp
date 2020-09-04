#include<Windows.h>
#include<tchar.h>
int WINAPI WinMain(HINSTANCE hInstanceExer, HINSTANCE prHinstance, PSTR pszCmdLine, int nCmdShow)
{
	STARTUPINFO si = { sizeof(si) };
	SECURITY_ATTRIBUTES saProcess, saThread;
	PROCESS_INFORMATION piProcessB, piProcessC;
	TCHAR szPath[MAX_PATH];

	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;

	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = FALSE;

	wcscpy_s(szPath,_countof(szPath),TEXT("ProcessB"));
	CreateProcess(NULL, szPath, &saProcess, &saThread, FALSE, 0, NULL, NULL, &si, &piProcessB);
	wcscpy_s(szPath, _countof(szPath), TEXT("ProcessC"));//´´½¨C
	CreateProcess(NULL, szPath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &piProcessC);

	return 0;

}