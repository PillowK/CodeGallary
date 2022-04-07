// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern HINSTANCE g_hVerDll;

TCHAR szSpecialPath[MAX_PATH] = { 0 };
TCHAR szCurrentModulePath[MAX_PATH] = { 0, };
TCHAR szModuleFullPath[MAX_PATH] = { 0, };
TCHAR szModuleFileName[MAX_PATH] = { 0, };

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
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

		}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

