// version.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

HINSTANCE g_hVerDll;

extern "C" DWORD _stdcall GetFileVersionInfoSizeA(LPCTSTR lptstrFilename, LPDWORD lpdwHandle)
{
	DWORD version_info = 0;

	typedef DWORD(*GetFileVersionInfoSizeA)(LPCTSTR lptstrFilename, LPDWORD lpdwHandle);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoSizeA lpGetFileVersionInfoSizeA;   //Dll �ν��Ͻ� �ڵ� ����
				//Dll�� �ε��Ѵ�.

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoSizeA = (GetFileVersionInfoSizeA)GetProcAddress(g_hVerDll, "GetFileVersionInfoSizeA");  //�Լ��� ��´�.


	if (lpGetFileVersionInfoSizeA == NULL)
	{
		return 0;
	}
	version_info = lpGetFileVersionInfoSizeA(lptstrFilename, lpdwHandle);



	return version_info;
}

extern "C" DWORD _stdcall GetFileVersionInfoSizeW(LPCTSTR lptstrFilename, LPDWORD lpdwHandle)
{
	DWORD version_info = 0;


	typedef DWORD(*GetFileVersionInfoSizeW)(LPCTSTR lptstrFilename, LPDWORD lpdwHandle);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoSizeW lpGetFileVersionInfoSizeW;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoSizeW = (GetFileVersionInfoSizeW)GetProcAddress(g_hVerDll, "GetFileVersionInfoSizeW");  //�Լ��� ��´�.


	if (lpGetFileVersionInfoSizeW == NULL)
	{
		return 0;
	}
	version_info = lpGetFileVersionInfoSizeW(lptstrFilename, lpdwHandle);



	return version_info;
}

extern "C" DWORD _stdcall GetFileVersionInfoSizeExA(DWORD dwFlags, LPCTSTR lptstrFilename, LPDWORD lpdwHandle)
{
	DWORD version_info = 0;

	typedef DWORD(*GetFileVersionInfoSizeExA)(DWORD dwFlags, LPCTSTR lptstrFilename, LPDWORD lpdwHandle);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoSizeExA lpGetFileVersionInfoSizeExA;   //Dll �ν��Ͻ� �ڵ� ����
				//Dll�� �ε��Ѵ�.

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoSizeExA = (GetFileVersionInfoSizeExA)GetProcAddress(g_hVerDll, "GetFileVersionInfoSizeA");  //�Լ��� ��´�.


	if (lpGetFileVersionInfoSizeExA == NULL)
	{
		return 0;
	}
	version_info = lpGetFileVersionInfoSizeExA(dwFlags, lptstrFilename, lpdwHandle);



	return version_info;
}


extern "C" DWORD _stdcall GetFileVersionInfoSizeExW(DWORD dwFlags, LPCTSTR lptstrFilename, LPDWORD lpdwHandle)
{
	DWORD version_info = 0;


	typedef DWORD(*GetFileVersionInfoSizeExW)(DWORD dwFlags, LPCTSTR lptstrFilename, LPDWORD lpdwHandle);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoSizeExW lpGetFileVersionInfoSizeExW;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoSizeExW = (GetFileVersionInfoSizeExW)GetProcAddress(g_hVerDll, "GetFileVersionInfoSizeExW");  //�Լ��� ��´�.


	if (lpGetFileVersionInfoSizeExW == NULL)
	{
		return 0;
	}
	version_info = lpGetFileVersionInfoSizeExW(dwFlags, lptstrFilename, lpdwHandle);


	return version_info;
}


extern "C" BOOL _stdcall GetFileVersionInfoA(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	BOOL retVal = FALSE;


	typedef BOOL(*GetFileVersionInfoA)(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoA lpGetFileVersionInfoA;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoA = (GetFileVersionInfoA)GetProcAddress(g_hVerDll, "GetFileVersionInfoA");  //�Լ��� ��´�.


	if (lpGetFileVersionInfoA == NULL)
	{
		return 0;
	}

	retVal = lpGetFileVersionInfoA(lptstrFilename, dwHandle, dwLen, lpData);

	return retVal;
}

extern "C" BOOL _stdcall GetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	BOOL retVal = FALSE;

	typedef BOOL(*GetFileVersionInfoW)(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoW lpGetFileVersionInfoW;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoW = (GetFileVersionInfoW)GetProcAddress(g_hVerDll, "GetFileVersionInfoW");  //�Լ��� ��´�.

	if (lpGetFileVersionInfoW == NULL)
	{
		return 0;
	}

	retVal = lpGetFileVersionInfoW(lptstrFilename, dwHandle, dwLen, lpData);


	return retVal;
}

extern "C" BOOL _stdcall GetFileVersionInfoExA(DWORD dwFlags, LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	BOOL retVal = FALSE;


	typedef BOOL(*GetFileVersionInfoExA)(DWORD dwFlags, LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoExA lpGetFileVersionInfoExA;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoExA = (GetFileVersionInfoExA)GetProcAddress(g_hVerDll, "GetFileVersionInfoExA");  //�Լ��� ��´�.


	if (lpGetFileVersionInfoExA == NULL)
	{
		return 0;
	}

	retVal = lpGetFileVersionInfoExA(dwFlags, lptstrFilename, dwHandle, dwLen, lpData);

	return retVal;
}

extern "C" BOOL _stdcall GetFileVersionInfoExW(DWORD dwFlags, LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData)
{
	BOOL retVal = FALSE;

	typedef BOOL(*GetFileVersionInfoExW)(DWORD dwFlags, LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	GetFileVersionInfoExW lpGetFileVersionInfoExW;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpGetFileVersionInfoExW = (GetFileVersionInfoExW)GetProcAddress(g_hVerDll, "GetFileVersionInfoExW");  //�Լ��� ��´�.

	if (lpGetFileVersionInfoExW == NULL)
	{
		return 0;
	}

	retVal = lpGetFileVersionInfoExW(dwFlags, lptstrFilename, dwHandle, dwLen, lpData);

	return retVal;
}

extern "C" BOOL _stdcall VerQueryValueA(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen)
{
	BOOL retVal = FALSE;

	typedef BOOL(*VerQueryValueA)(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	VerQueryValueA lpVerQueryValueA;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpVerQueryValueA = (VerQueryValueA)GetProcAddress(g_hVerDll, "VerQueryValueA");  //�Լ��� ��´�.


	if (lpVerQueryValueA == NULL)
	{
		return 0;
	}

	retVal = lpVerQueryValueA(pBlock, lpSubBlock, lplpBuffer, puLen);

	return retVal;
}

extern "C" BOOL _stdcall VerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen)
{
	BOOL retVal = FALSE;

	typedef BOOL(*VerQueryValueW)(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen);

	//typederf�� ������ FactoryFunc�� ������ �����Ѵ�.

	VerQueryValueW lpVerQueryValueW;   //Dll �ν��Ͻ� �ڵ� ����

	if (g_hVerDll == NULL)
	{
		return 0;
	}

	lpVerQueryValueW = (VerQueryValueW)GetProcAddress(g_hVerDll, "VerQueryValueW");  //�Լ��� ��´�.

	if (lpVerQueryValueW == NULL)
	{
		return 0;
	}

	retVal = lpVerQueryValueW(pBlock, lpSubBlock, lplpBuffer, puLen);

	return retVal;
}