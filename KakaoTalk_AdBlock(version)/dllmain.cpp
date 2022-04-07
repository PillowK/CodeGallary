// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern HINSTANCE g_hVerDll;

TCHAR szSpecialPath[MAX_PATH] = { 0 };
TCHAR szCurrentModulePath[MAX_PATH] = { 0, };
TCHAR szModuleFullPath[MAX_PATH] = { 0, };
TCHAR szModuleFileName[MAX_PATH] = { 0, };
TCHAR szWindowText[MAX_PATH] = { 0, };
TCHAR szLogBuffer[MAX_PATH] = { 0, };

RECT rtMainWindow;
HWND hMainWindow;

BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam)
{
	HWND hParent = GetParent(hwnd);
	TCHAR wndClassName[MAX_PATH] = { 0, };
	TCHAR wndText[MAX_PATH] = { 0, };

	GetClassName(hwnd, wndClassName, MAX_PATH);
	GetWindowText(hwnd, wndText, MAX_PATH);

	if (_tcscmp(L"EVA_Window_Dblclk", wndClassName) == 0) {
		if (_tcslen(wndText) == 0 || wndText == NULL)
		{
			ShowWindow(hwnd, 0);
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE);
		}
	}

	if (_tcscmp(L"EVA_Window", wndClassName) == 0) {
		TCHAR wndParentText[MAX_PATH] = { 0, };				
		GetWindowText(hParent, wndParentText, MAX_PATH);

		if (hParent == hMainWindow || _tcsstr(wndParentText, L"LockModeView") != NULL)
		{
			ShowWindow(hwnd, 0);
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE);
		}
	}

	if (_tcsstr(wndText, L"OnlineMainView") != NULL && hParent == hMainWindow) {
		int width = rtMainWindow.right - rtMainWindow.left;
		int height = (rtMainWindow.bottom - rtMainWindow.top) - 38;

		UpdateWindow(hMainWindow);
		SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOMOVE);
	}

	if (_tcsstr(wndText, L"LockModeView") != NULL && hParent == hMainWindow)
	{
		int width = rtMainWindow.right - rtMainWindow.left;
		int height = rtMainWindow.bottom - rtMainWindow.top;

		UpdateWindow(hMainWindow);
		SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOMOVE);
	}

	return true;
}

BOOL CALLBACK EnumAllWindowProc(HWND hwnd, LPARAM lParam)
{
	EnumAllWindows(hwnd);
	return true;
}

VOID EnumAllWindows(HWND hwnd)
{	
	EnumWindowProc(hwnd, NULL);
	if (!EnumChildWindows(hwnd, EnumAllWindowProc, NULL))
	{
		return;
	}	
}

DWORD ThreadProc(LPVOID lpdwThreadParam)
{
	while (true) {			
		hMainWindow = FindWindow(L"EVA_Window_Dblclk", L"Ä«Ä«¿ÀÅå");

		if (hMainWindow == NULL)
		{
			hMainWindow = FindWindow(L"EVA_Window_Dblclk", L"KakaoTalk");
		}

		if (hMainWindow != NULL)
		{
			GetWindowRect(hMainWindow, &rtMainWindow);

			if (hMainWindow != NULL) {
				EnumAllWindows(hMainWindow);
			}
		}

		Sleep(500);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	DWORD dwThreadId;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

		GetModuleFileName(NULL, szModuleFullPath, MAX_PATH);
		_tsplitpath_s(szModuleFullPath, NULL, 0, szCurrentModulePath, MAX_PATH, szModuleFileName, MAX_PATH, NULL, 0);

		SHGetSpecialFolderPath(NULL, szSpecialPath, CSIDL_SYSTEM, FALSE);
		_tcscat_s(szSpecialPath, MAX_PATH, L"\\version.dll");
		g_hVerDll = LoadLibrary(szSpecialPath);

		if (_tcsicmp(szModuleFileName, L"KakaoTalk") == 0)
		{
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, (LPVOID)GetCurrentThreadId(), 0, &dwThreadId);
		}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

