
#pragma once

#include <MAPIUtil.h>
#include <tchar.h>
#include <MAPI.h>
#include <MAPIX.h>
#include <shlobj.h>

#define PR_IPM_APPOINTMENT_ENTRYID (PROP_TAG(PT_BINARY, 0x36D0))
#define PR_IPM_CONTACT_ENTRYID (PROP_TAG(PT_BINARY, 0x36D1))
#define PR_IPM_JOURNAL_ENTRYID (PROP_TAG(PT_BINARY, 0x36D2))
#define PR_IPM_NOTE_ENTRYID (PROP_TAG(PT_BINARY, 0x36D3))
#define PR_IPM_TASK_ENTRYID (PROP_TAG(PT_BINARY, 0x36D4))
#define PR_IPM_DRAFTS_ENTRYID (PROP_TAG(PT_BINARY, 0x36D7))
#define	PR_BODY_HTML (PROP_TAG(PT_STRING8, 0x1013))


using namespace std;

namespace OlMAPILoader
{
	extern "C" { __declspec(dllexport) char* GetMailBody(byte* bPrSearchKey, ULONG cbSearchKey);}
	LPMESSAGE SearchMailItem(LPMDB lpMDB, LPMAPITABLE lpMapitable, byte* bSearchKey, ULONG cbSearchKey);
	VOID ListMessages(LPMDB lpMDB, LPMAPIFOLDER lpInboxFolder);
	BOOL OpenInbox(LPMDB lpMDB, ULONG boxEntryID, LPMAPIFOLDER* lpInboxFolder);
	BOOL OpenMessageStore(LPMAPISESSION lpMAPISession, LPMDB* lpMDB);
	BOOL OlMAPILogon(LPMAPISESSION* OlMAPISession);
	VOID OlMAPILogoff(IMAPISession* pfnSession);
	VOID OlMAPIFunctionUnInitialize();
	BOOL OlMAPIFunctionInitialize();

	typedef BOOL(STDAPICALLTYPE FGETCOMPONENTPATH) (
		LPSTR szComponent,
		LPSTR szQualifier,
		LPSTR szDllPath,
		DWORD cchBufferSize,
		BOOL fInstall);
	typedef FGETCOMPONENTPATH FAR * LPFGETCOMPONENTPATH;

}