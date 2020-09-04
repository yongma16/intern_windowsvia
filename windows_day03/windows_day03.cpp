// windows_day03.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <iostream>
//using namespace std;


#include<windows.h>
#include<windowsx.h>
#include<Sddl.h>
#include<tchar.h>
#include<strsafe.h>//file


HWND g_hDlg;
HANDLE g_hSingleton = NULL, g_hBoundary = NULL, g_hNamespace = NULL;//句柄
BOOL g_bNamespaceOpened = FALSE;
PCTSTR g_szBoundary = TEXT("3-Boundary"), g_szNamespace = TEXT("3-NAmespace");

#define DETALLS_CTRL GetDlgItem(g_hDlg,IDC_EDIT_DETALLS)

void AddText(PCTSTR pszFormat)
{
	va_list argList;
	va_start(argList, pszFormat);
	TCHAR sz[20 * 1024];
	
	Edit_GetText(DETALLS_CTRL, sz, _countof(sz));//
	_vstprintf_s(_tcschr(sz, TEXT('\0')), _countof(sz) - _tcslen(sz), pszFormat, argList);
	Edit_SetText(DETALLS_CTRL, sz);
	va_end(argList);
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtrl, UINT codeNotify)
{
	switch (id)
	{
	case IDOK:
	case IDCANCEL:
		EndDialog(hwnd, id);
		break;
	}
}

void CheckInstances()
{
	g_hBoundary = CreateBoundaryDescriptor(g_szBoundary, 0);
	BYTE localAdminSID[SECURITY_MAX_SID_SIZE];
	PSID pLocalAdminSID = &localAdminSID;
	DWORD cbSID = sizeof(localAdminSID);
	//创建boundary描述
	if (!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,pLocalAdminSID,&cbSID))
	{
		AddText(TEXT("AddSIDToBoundaryDescriptor failed:%u\r\n"),GetLastError());
			return;
	}

	//联系用户sid
	if (!AddSIDToBoundaryDescriptor(&g_hBoundary, pLocalAdminSID))
	{
		AddText(TEXT("ADDSIDBOUNDARY failed:%u\r\n"),GetLastError);//错误提示
		return;
	}

	//为用户创建命名空间
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	if (!ConvertStringSecurityDescriptorToSecurityDescriptor(TEXT("D:(A;;GA;;BA)"), SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL))
	{
		AddText(TEXT("security failed:%u\r\n"), GetLastError);//报错提示
		return;
	}
	g_hNamespace = CreatePrivateNamespace(&sa, g_hBoundary, g_szNamespace);
	//创建对应释放
	LocalFree(sa.lpSecurityDescriptor);
	DWORD dwLastError = GetLastError();
	if (g_hNamespace == NULL)
	{
		if (dwLastError == ERROR_ACCESS_DENIED)
		{
			AddText(TEXT("denied access\r\n"));
			AddText(TEXT("running as Administrator\r\n\r\n"));
			return;
		}
		else {
			if (dwLastError == ERROR_ALREADY_EXISTS)
			{
				AddText(TEXT("failed：%u\r\n"), dwLastError);
				g_hNamespace = OpenPrivateNamespace(g_hBoundary, g_szNamespace);
				if (g_hNamespace == NULL)
				{
					AddText(TEXT("open failed：%u\r\n"), dwLastError);//打开失败
					return;
				}
				else {
					g_bNamespaceOpened = TRUE;
					AddText(TEXT("open ok!"));
				}
			}
			else {
				AddText(TEXT("error occured：%u\r\n"),dwLastError);//不是创建错误
				return;
			}
		}
	}
	//尝试创建
	TCHAR szMutexName[64];
	StringCchPrintf(szMutexName, _countof(szMutexName), TEXT("%s\\%s"), g_szNamespace, TEXT("singleton"));
	g_hSingleton = CreateMutex(NULL, FALSE, szMutexName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AddText(TEXT("another is running!\r\n"));
		AddText(TEXT("---> !!!"));
	}
	else
	{
		AddText(TEXT("instance of singon：\r\n"));
		AddText(TEXT("Access application fefatures now.\r\n"));
	}
}
//
BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	chSETDLGICONS(hwnd, IDI_SINGLETON);//错误2
	g_hDlg = hwnd;
	CheckInstances();
	return TRUE;
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		chHANDLE_DLGMSG(hwnd, WM_COMMAND, Dlg_OnCommand);
		chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);

	}
	return FALSE;
}



int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prehinstance,PSTR szCmdLine,int Cmdline)
{
	//缺少文件
	UNREFERENCED_PARAMETER(preinstance);
	UNREFERENCED_PARAMETER(szCmdLine);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_SINGLETON), NULL, Dlg_Proc);
	if (g_hSingleton != NULL)
	{
		CloseHandle(g_hSingleton);//关闭内核对象
	}
	if (g_hNamespace != NULL)
	{
		if (g_bNamespaceOpened)
		{
			ClosePrivateNamespace(g_hNamespace, 0);
		}
		else {
			ClosePrivateNamespace(g_hNamespace, PRIVATE_NAMESPACE_FLAG_DESTROY);
		}
	}
	if (g_hBoundary != NULL)
	{
		DeleteBoundaryDescriptor(g_hBoundary);//
	}
	//MessageBox(NULL, TEXT("测试\nyongma16.xyz"), TEXT("测试"),MB_OK);
	return 0;
}
