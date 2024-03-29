// stdafx.cpp : source file that includes just the standard includes
// hid.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

extern HWND g_hWndNativeWindow;//CommunicatorMainWindowClass
extern HWND	g_hWndNativePanel;//NetUINativeHWNDHost

extern BOOL	bHeartbitN;
extern DWORD g_LastError;
extern TCHAR szLogBuffer[MAX_PATH];

WNDPROC g_NativeWindowOldProc = NULL;//CommunicatorMainWindowClassOldProc
WNDPROC g_NativePanelOldProc = NULL;//NetUINativeHWNDHostOldProc
WNDPROC g_NUIHWNDOldProc = NULL;//NetUIHWNDOldproc

extern "C" BOOL _stdcall GetHeartbitN()
{
	return bHeartbitN;
}

extern "C" void _stdcall SetNativeWindowProc(HWND hWnd)
{
	HWND hDeskWnd = GetDesktopWindow();

	g_NativeWindowOldProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	if (g_NativeWindowOldProc != NULL)
	{
		// 원래의 윈도우 프로시저 저장
		SetProp(hDeskWnd, PROP_NATIVE_WINDOW, g_NativeWindowOldProc);

		// 신규 윈도우 프로시저를 설정한다.
		SetLastError(0);
		/*g_CMCOldProc = (WNDPROC)*/SetWindowLongPtr(hWnd, GWLP_WNDPROC, (UINT_PTR)NativeWindowProc);
		g_LastError = GetLastError();
		_stprintf_s(szLogBuffer, _T("[fnsccom]  - SetNativePanelProc - SetWindowLong : %d"), g_LastError);
		OutputDebugString(szLogBuffer);
	}
}

extern "C" WNDPROC _stdcall GetNativeWindowProc(HWND hWnd)
{
	HWND hDeskWnd = GetDesktopWindow();
	return (WNDPROC)GetProp(hDeskWnd, PROP_NATIVE_WINDOW);
}

extern "C" void _stdcall RemoveNativeWindowProc(HWND hWnd)
{
	HWND hDeskWnd;
	g_NativeWindowOldProc = GetNativeWindowProc(hWnd);
	if (g_NativeWindowOldProc != NULL)
	{
		// 원래의 윈도우 프로시저를 돌려준다.
		SetWindowLong(hWnd, GWLP_WNDPROC, (DWORD)g_NativeWindowOldProc);

		// 저장된 프로시저 정보 제거
		hDeskWnd = GetDesktopWindow();
		RemoveProp(hDeskWnd, PROP_NATIVE_WINDOW);
		g_NativeWindowOldProc = NULL;
	}
}

extern "C" void _stdcall SetNativePanelProc(HWND hWnd)
{
	HWND hDeskWnd = GetDesktopWindow();

	g_NativePanelOldProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	if (g_NativePanelOldProc != NULL)
	{
		// 원래의 윈도우 프로시저 저장
		SetProp(hDeskWnd, PROP_NATIVE_PANEL, g_NativePanelOldProc);

		// 신규 윈도우 프로시저를 설정한다.
		SetLastError(0);
		/*g_NNUIOldProc = (WNDPROC)*/SetWindowLongPtr(hWnd, GWLP_WNDPROC, (UINT_PTR)NativePanelProc);
		g_LastError = GetLastError();
		_stprintf_s(szLogBuffer, _T("[fnsccom]  - SetNativePanelProc - SetWindowLong : %d"), g_LastError);
		OutputDebugString(szLogBuffer);
	}
}

extern "C" WNDPROC _stdcall GetNativePanelProc(HWND hWnd)
{
	HWND hDeskWnd = GetDesktopWindow();
	return (WNDPROC)GetProp(hDeskWnd, PROP_NATIVE_PANEL);
}

extern "C" void _stdcall RemoveNativePanelProc(HWND hWnd)
{
	HWND hDeskWnd;

	g_NativePanelOldProc = GetNativePanelProc(hWnd);
	if (g_NativePanelOldProc != NULL)
	{
		// 원래의 윈도우 프로시저를 돌려준다.
		SetWindowLong(hWnd, GWLP_WNDPROC, (DWORD)g_NativePanelOldProc);

		// 저장된 프로시저 정보 제거
		hDeskWnd = GetDesktopWindow();
		RemoveProp(hDeskWnd, PROP_NATIVE_PANEL);
		g_NativePanelOldProc = NULL;
	}
}

static LRESULT CALLBACK NativeWindowProc(HWND hWnd, UINT_PTR message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		RemoveNativeWindowProc(hWnd);
	}
	}

	return CallWindowProc(g_NativeWindowOldProc, hWnd, message, wParam, lParam);
}

static LRESULT CALLBACK NativePanelProc(HWND hWnd, UINT_PTR message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RUNAPP:
	{
	///*	WpfApp::App^ app = gcnew WpfApp::App();
	//	
	//	Microsoft::Practices::Prism::UnityExtensions::UnityBootstrapper^ btr =
	//		gcnew WpfApp::Bootstrapper();*/

	//	btr->Run();
	}
	break;
	case  WM_DESTROY:
	{
		RemoveNativePanelProc(hWnd);
	}
	break;
	}

	return CallWindowProc(g_NativePanelOldProc, hWnd, message, wParam, lParam);
}