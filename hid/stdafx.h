// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>

#define PROP_NATIVE_WINDOW _T("feelanet_native_ui_window_prop")
#define PROP_NATIVE_PANEL _T("feelanet_native_ui_panel_prop")

#define WM_SIGNAL  WM_USER+33
#define WM_HEARTBIT  WM_USER+34
#define WM_RUNAPP WM_USER+35
#define WM_INSTALLUI WM_USER+36
#define WM_RESIZENATIVEPAN WM_USER+37

extern "C" void _stdcall SetNativeWindowProc(HWND hWnd);
extern "C" WNDPROC _stdcall GetNativeWindowProc(HWND hWnd);
extern "C" void _stdcall SetNativePanelProc(HWND hWnd);
extern "C" WNDPROC _stdcall GetNativePanelProc(HWND hWnd);
extern "C" BOOL _stdcall GetHeartbitN();

static LRESULT CALLBACK NativeWindowProc(HWND hWnd, UINT_PTR message, WPARAM wParam, LPARAM lparam);
static LRESULT CALLBACK NativePanelProc(HWND hWnd, UINT_PTR message, WPARAM wParam, LPARAM lparam);

// TODO: reference additional headers your program requires here
