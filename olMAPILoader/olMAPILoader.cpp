// olMAPILoader.cpp : Defines the exported functions for the DLL application.
//



#include "stdafx.h"
#include "olMAPILoader.h"

namespace OlMAPILoader
{
	BOOL bInitialized = FALSE;

	LPFGETCOMPONENTPATH pfnFGetComponentPath = NULL;
	LPMAPIINITIALIZE pfnMAPIInitialize = NULL;
	LPMAPIUNINITIALIZE pfnMAPIUnInitialize = NULL;
	LPMAPILOGONEX pfnMAPILogon = NULL;
	LPMAPILOGOFF pfnMAPILogOff = NULL;
	LPMAPIFREEBUFFER pfnMAPIFreeBuffer = NULL;

	HMODULE h_mmapi32 = NULL;
	HMODULE h_molmapi = NULL;

	void PointerStrCat(char** dest, char* source)
	{
		if(*dest != NULL)
		{
			char* totalDest = (char*)malloc(1 + strlen(*dest) + strlen(source));

			if(totalDest != NULL)
			{
				strcpy(totalDest,*dest);
				strcat(totalDest,source);
			}

			free(*dest);
			(*dest) = totalDest;
		}
		else
		{
			char* totalDest = (char*)malloc(1+strlen(source));
			strcpy(totalDest,source);

			(*dest) = totalDest;
		}
	}

	char* GetMailBody(byte* bPrSearchKey, ULONG cbSearchKey)
	{
		ULONG ulNumChars;
		LPENTRYID lpEntryID = NULL;
		char* pszReturn = NULL;
		char* szBuffer = NULL;
		HRESULT hRes = S_OK;
		LPMAPISESSION lpSession = NULL;
		LPMDB lpMDB = NULL;
		LPMAPIFOLDER lpMAPIFolder = NULL;
		LPMAPITABLE lpContentsTable = NULL;
		LPMESSAGE lpMessage = NULL;
		LPSTREAM lpStream = NULL;

		if(OlMAPIFunctionInitialize())
		{
			if(OlMAPILogon(&lpSession))
			{
				if(OpenMessageStore(lpSession, &lpMDB))
				{			
					if(OpenInbox(lpMDB, PR_IPM_DRAFTS_ENTRYID, &lpMAPIFolder))
					{
						hRes = lpMAPIFolder->GetContentsTable(0, &lpContentsTable);

						if(hRes == S_OK)
						{
							lpMessage = SearchMailItem(lpMDB, lpContentsTable, bPrSearchKey, cbSearchKey);

							if(lpMessage != NULL)
							{
								hRes = lpMessage->OpenProperty(PR_BODY_HTML, &IID_IStream, STGM_READ, NULL, (LPUNKNOWN*)&lpStream);

								if(hRes == S_OK)
								{
									do
									{
										char szTempBuffer[MAX_PATH+1] = {0,};

										lpStream->Read(szTempBuffer, MAX_PATH, &ulNumChars);
										szTempBuffer[min(MAX_PATH,ulNumChars)]=0;

										PointerStrCat(&szBuffer,szTempBuffer);

									}while (ulNumChars >= MAX_PATH);

									ULONG ulSize = strlen(szBuffer) + sizeof(char);
	
									if(ulSize > 0)
									{
										pszReturn = (char*)::CoTaskMemAlloc(1 + ulSize);
										strcpy(pszReturn, szBuffer);
									}

									free(szBuffer);
								}

								lpStream->Release();
								UlRelease(lpMessage);
							}
						}
					}
				}
			}
		}

		UlRelease(lpContentsTable);
		UlRelease(lpMAPIFolder);
		UlRelease(lpMDB);
		UlRelease(lpSession);
		(*pfnMAPIUnInitialize)();

		return pszReturn;
	}
	
	LPMESSAGE SearchMailItem(LPMDB lpMDB, LPMAPITABLE lpMapitable, byte* bSearchKey, ULONG cbSearchKey)
	{		
		HRESULT hRes = S_OK;
		LPMESSAGE lpMessageReturn = NULL;
		LPSRowSet pRows = NULL;
		ULONG i = 0;
		ULONG ulObjType;

		enum {	
			ePR_ENTRYID,			
			ePR_SEARCH_KEY,
			NUM_COLS
		};

		SizedSPropTagArray(NUM_COLS, sptCols) = { NUM_COLS,
			 PR_ENTRYID, PR_SEARCH_KEY
		};

		hRes = HrQueryAllRows(lpMapitable,(LPSPropTagArray)&sptCols,NULL,NULL,0,&pRows);

		if(hRes == S_OK)
		{
			for (i = 0; i< pRows->cRows ; i++)
			{
				if(memcmp(bSearchKey, pRows->aRow[i].lpProps[ePR_SEARCH_KEY].Value.bin.lpb, pRows->aRow[i].lpProps[ePR_SEARCH_KEY].Value.bin.cb) == 0)
				{
					hRes = lpMDB->OpenEntry(pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.cb,
										(LPENTRYID)pRows->aRow[i].lpProps[ePR_ENTRYID].Value.bin.lpb,
										NULL,
										MAPI_BEST_ACCESS,
										&ulObjType,
										(LPUNKNOWN*)&lpMessageReturn);

					return lpMessageReturn;
				}
			}
			FreeProws(pRows);
		}
		return NULL;
	}

	BOOL OpenInbox(LPMDB lpMDB, ULONG boxEntryID, LPMAPIFOLDER* lpInboxFolder)
	{
		HRESULT hRes = S_OK;
		ULONG cbInbox = 0;
		LPENTRYID lpbInbox = NULL;
		LPMAPIFOLDER lpReturnFolder = NULL;

		hRes = lpMDB->GetReceiveFolder(NULL, NULL, &cbInbox, &lpbInbox, NULL);

		if (hRes == S_OK)
		{
			ULONG ulObjType;
			LPMAPIFOLDER lpTempFolder = NULL;
			LPSPropValue rpgtValue = NULL;

			lpMDB->OpenEntry(cbInbox, lpbInbox, NULL, MAPI_BEST_ACCESS, &ulObjType, (LPUNKNOWN*)&lpTempFolder);

			ULONG cbRpgtValue = 0;
			SizedSPropTagArray(2, rpgt) = { 2, { boxEntryID, PR_OBJECT_TYPE} };

			hRes = lpTempFolder->GetProps((LPSPropTagArray)&rpgt, MAPI_UNICODE, &cbRpgtValue, &rpgtValue);

			if (hRes == S_OK)
			{
				hRes = lpTempFolder->OpenEntry(rpgtValue[0].Value.bin.cb, (LPENTRYID)rpgtValue[0].Value.bin.lpb, NULL, MAPI_BEST_ACCESS, &ulObjType, (LPUNKNOWN*)&lpReturnFolder);

				if (hRes == S_OK)
				{
					*lpInboxFolder = lpReturnFolder;
					(*pfnMAPIFreeBuffer)(rpgtValue);
					UlRelease(lpTempFolder);
					return true;
				}

			}
		}

		return false;
	}

	BOOL OpenMessageStore(LPMAPISESSION lpMAPISession, LPMDB* lpMDB)
	{
		LPMAPITABLE pStoreTbl = NULL;
		LPSRowSet pRow = NULL;
		static SRestriction sres;
		SPropValue spv;
		HRESULT hRes;
		LPMDB lpTempMDB = NULL;

		enum { EID, NAME, NUM_COLS };
		SizedSPropTagArray(NUM_COLS, sptCols) = { NUM_COLS, PR_ENTRYID, PR_DISPLAY_NAME };
		*lpMDB = NULL;

		hRes = lpMAPISession->GetMsgStoresTable(0, &pStoreTbl);

		if (hRes == S_OK)
		{
			sres.rt = RES_PROPERTY;
			sres.res.resProperty.relop = RELOP_EQ;
			sres.res.resProperty.ulPropTag = PR_DEFAULT_STORE;
			sres.res.resProperty.lpProp = &spv;

			spv.ulPropTag = PR_DEFAULT_STORE;
			spv.Value.b = TRUE;

			hRes = HrQueryAllRows(pStoreTbl, (LPSPropTagArray)&sptCols, &sres, NULL, 0, &pRow);

			if (hRes == S_OK)
			{
				hRes = lpMAPISession->OpenMsgStore(NULL,
					pRow->aRow[0].lpProps[EID].Value.bin.cb,
					(LPENTRYID)pRow->aRow[0].lpProps[EID].Value.bin.lpb,
					NULL, MAPI_BEST_ACCESS, &lpTempMDB);

				FreeProws(pRow);
				UlRelease(pStoreTbl);

				*lpMDB = lpTempMDB;
				return true;
			}
		}

		return false;
	}

	BOOL OlMAPILogon(LPMAPISESSION* OlMAPISession)
	{
		LPMAPISESSION OlTempSession;

		if (pfnMAPILogon != NULL)
		{
			HRESULT res = (*pfnMAPILogon)(0, NULL, NULL, MAPI_USE_DEFAULT, (LPMAPISESSION*)&OlTempSession);

			if (res == S_OK)
			{			
				*OlMAPISession = OlTempSession;
				return true;
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	VOID OlMAPILogoff(IMAPISession* pfnSession)
	{
		UlRelease(&pfnSession);
	}

	VOID OlMAPIFunctionUnInitialize()
	{
		if (pfnMAPIUnInitialize != NULL)
		{
			(*pfnMAPIUnInitialize)();
		}

		pfnMAPIInitialize = NULL;
		pfnMAPIUnInitialize = NULL;
		pfnFGetComponentPath = NULL;
		pfnMAPILogon = NULL;

		if (h_molmapi != NULL)
		{
			FreeLibrary(h_molmapi);
		}

		if (h_mmapi32)
		{
			FreeLibrary(h_mmapi32);
		}

		bInitialized = FALSE;
	}

	BOOL CheckOSBit()
	{
	#if _WIN64

	   return true;

	#elif _WIN32

		BOOL isWow64 = FALSE;


		typedef BOOL (WINAPI *LFN_ISWOW64PROCESS)(HANDLE,PBOOL);
		BOOL retVal = FALSE;

	
		HMODULE KERNEL = GetModuleHandle(L"kernel32");
		LFN_ISWOW64PROCESS fnIsWow64Process = (LFN_ISWOW64PROCESS)GetProcAddress(KERNEL,"IsWow64Process");


		if(fnIsWow64Process != NULL)
		{
			fnIsWow64Process(GetCurrentProcess(), &retVal);
		}

		FreeLibrary(KERNEL);

		return retVal;
	#endif
	}

	BOOL OlMAPIFunctionInitialize()
	{
		DWORD KEY_IS64KEY = 0x0000;
		BOOL bRetVal = false;
		TCHAR szSpecialPath[MAX_PATH] = { 0, };
		LONG lRet = 0;
		SHGetSpecialFolderPath(NULL, szSpecialPath, CSIDL_SYSTEM, FALSE);
		_tcscat_s(szSpecialPath, MAX_PATH, L"\\mapi32.dll");

		if(CheckOSBit())
		{
			KEY_IS64KEY = KEY_WOW64_64KEY;
		}


		if(!bInitialized)
		{
			HMODULE h_mmapi32 = LoadLibrary(szSpecialPath);

			if (h_mmapi32)
			{
				pfnFGetComponentPath = (LPFGETCOMPONENTPATH)GetProcAddress(h_mmapi32, "FGetComponentPath");

				if (pfnFGetComponentPath)
				{
					char szMAPIDir[MAX_PATH] = { 0, };
					char szComponentID[MAX_PATH] = { 0, };
					TCHAR szwMAPIDir[MAX_PATH] = { 0, };
					HKEY hMicrosoftOutlook = NULL;

					lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Clients\\Mail\\Microsoft Outlook"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hMicrosoftOutlook);

					if (lRet == ERROR_SUCCESS)
					{
						TCHAR szwComponentID[MAX_PATH] = { 0, };
						DWORD cb = MAX_PATH;
						DWORD dwKeyType = NULL;
						size_t convertNum = 0;

						RegQueryValueEx(hMicrosoftOutlook, L"MSIComponentID", 0, &dwKeyType, (LPBYTE)szwComponentID, &cb);
						wcstombs_s(&convertNum, szComponentID, szwComponentID, MAX_PATH);

						if (pfnFGetComponentPath((LPSTR)szComponentID, NULL, (LPSTR)szMAPIDir, MAX_PATH, true))
						{
							MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szMAPIDir, MAX_PATH, szwMAPIDir, MAX_PATH);

							h_molmapi = LoadLibrary(szwMAPIDir);

							if (h_molmapi)
							{
								pfnMAPIInitialize = (LPMAPIINITIALIZE)GetProcAddress(h_molmapi, "MAPIInitialize");
								pfnMAPIUnInitialize = (LPMAPIUNINITIALIZE)GetProcAddress(h_molmapi, "MAPIUninitialize");
								pfnMAPILogon = (LPMAPILOGONEX)GetProcAddress(h_molmapi, "MAPILogonEx");
								pfnMAPILogOff = (LPMAPILOGOFF)GetProcAddress(h_molmapi, "MAPILogoff");						
								pfnMAPIFreeBuffer = (LPMAPIFREEBUFFER)GetProcAddress(h_molmapi, "MAPIFreeBuffer");

								if (pfnMAPIInitialize &&
									pfnMAPIUnInitialize &&
									pfnMAPILogon &&
									pfnMAPILogOff &&
									pfnMAPIFreeBuffer
									)
								{
									HRESULT hr = (*pfnMAPIInitialize)(NULL);
												
									if (hr == S_OK)
									{							
										bInitialized = TRUE;
										bRetVal = true;
									}
									else
									{
										bRetVal = false;
									}
								}
							}
						}
					}

					if (hMicrosoftOutlook)
					{
						RegCloseKey(hMicrosoftOutlook);
					}
				}
			}
		}
		else
		{
			bRetVal = true;
		}

		return bRetVal;
	}
}