// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <ShlObj.h>

HINSTANCE g_hHidDll;
HANDLE g_hThread;

TCHAR szLogBuffer[MAX_PATH] = { 0, };

HWND g_hWndNativeWindow;//CommunicatorMainWindowClass
HWND g_hWndNativePanel;//NetUINativeHWNDHost
BOOL g_bIsAppRequired;

BOOL bHeartbitN;
DWORD g_LastError;

DWORD ThreadProc(LPVOID lpdwThreadParam)
{
	BOOL bWindowProcCreated = FALSE;
	BOOL isMsgSent = FALSE;

	do {
		/*
		** [ 메인 윈도우 핸들 조회 ]
		**/
		do {
			Sleep(10);
			g_hWndNativeWindow = FindWindowEx(NULL, NULL, _T("CommunicatorMainWindowClass"), NULL);
		} while (g_hWndNativeWindow == NULL || IsWindow(g_hWndNativeWindow) == FALSE);

		/*
		** [ Lync UI Native 핸들 조회 ]
		**/
		do {
			Sleep(10);
			SetLastError(0);
			g_hWndNativePanel = FindWindowEx(g_hWndNativeWindow, NULL, _T("NetUINativeHWNDHost"), NULL);
			g_LastError = GetLastError();

		} while (g_hWndNativePanel == NULL || IsWindow(g_hWndNativePanel) == FALSE);

		/*
		** [ 메인 윈도우 프로시저 설치 ]
		**/
		if (!bWindowProcCreated)
		{
			bWindowProcCreated = TRUE;
			SetNativeWindowProc(g_hWndNativeWindow);
		}

		/*
		** [ Lync UI Native 프로시저 설치 ]
		**/
		if (!GetHeartbitN()) {
			SetNativePanelProc(g_hWndNativePanel);
			Sleep(10);
		}

		/*
		** [ WPF 응용프로그램 구성 ]
		** WPF Application을 실행하게 합니다.
		** 애드인 관련 UI 스레드가 만들어집니다.
		**/
		g_bIsAppRequired = TRUE;
		SendMessage(g_hWndNativePanel, (UINT_PTR)WM_RUNAPP, 0, 0);

		// 응답 대기
		do {
			Sleep(10);
		} while (g_bIsAppRequired);
		break;
	} while (true);

	return TRUE;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		TCHAR szSpecialPath[MAX_PATH] = { 0, };

		SHGetSpecialFolderPath(NULL, szSpecialPath, CSIDL_SYSTEM, FALSE);
		_tcscat_s(szSpecialPath, MAX_PATH, L"\\hid.dll");

		g_hHidDll = LoadLibrary(szSpecialPath);
		OutputDebugString(_T("[DEBUG] HID LOADED"));

		DWORD dwThreadId = 0;

		g_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, (LPVOID)GetCurrentThreadId(), 0, &dwThreadId);

		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

