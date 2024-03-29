// hid.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

typedef struct _HIDP_PREPARSED_DATA * PHIDP_PREPARSED_DATA;


typedef struct _HIDP_UNKNOWN_TOKEN
{
	UCHAR  Token;
	UCHAR  Reserved[3];
	ULONG  BitField;
} HIDP_UNKNOWN_TOKEN, *PHIDP_UNKNOWN_TOKEN;

typedef struct _HIDP_EXTENDED_ATTRIBUTES
{
	UCHAR   NumGlobalUnknowns;
	UCHAR   Reserved[3];
	PHIDP_UNKNOWN_TOKEN  GlobalUnknowns;
	// ... Additional attributes
	ULONG   Data[1]; // variableLength  DO NOT ACCESS THIS FIELD
} HIDP_EXTENDED_ATTRIBUTES, *PHIDP_EXTENDED_ATTRIBUTES;

typedef struct _HIDP_DATA
{
	USHORT  DataIndex;
	USHORT  Reserved;
	union {
		ULONG   RawValue; // for values
		BOOLEAN On; // for buttons MUST BE TRUE for buttons.
	};
} HIDP_DATA, *PHIDP_DATA;

typedef struct _HIDD_ATTRIBUTES {
	ULONG   Size; // = sizeof (struct _HIDD_ATTRIBUTES)

				  //
				  // Vendor ids of this hid device
				  //
	USHORT  VendorID;
	USHORT  ProductID;
	USHORT  VersionNumber;

	//
	// Additional fields will be added to the end of this structure.
	//
} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;

typedef struct _HIDD_CONFIGURATION {
	PVOID    cookie;
	ULONG    size;
	ULONG    RingBufferSize;
} HIDD_CONFIGURATION, *PHIDD_CONFIGURATION;

typedef enum _HIDP_REPORT_TYPE
{
	HidP_Input,
	HidP_Output,
	HidP_Feature
} HIDP_REPORT_TYPE;

typedef USHORT USAGE, *PUSAGE;

typedef struct _USAGE_AND_PAGE
{
	USAGE Usage;
	USAGE UsagePage;
} USAGE_AND_PAGE, *PUSAGE_AND_PAGE;

typedef struct _HIDP_BUTTON_CAPS
{
	USAGE    UsagePage;
	UCHAR    ReportID;
	BOOLEAN  IsAlias;

	USHORT   BitField;
	USHORT   LinkCollection;   // A unique internal index pointer

	USAGE    LinkUsage;
	USAGE    LinkUsagePage;

	BOOLEAN  IsRange;
	BOOLEAN  IsStringRange;
	BOOLEAN  IsDesignatorRange;
	BOOLEAN  IsAbsolute;

	ULONG    Reserved[10];
	union {
		struct {
			USAGE    UsageMin, UsageMax;
			USHORT   StringMin, StringMax;
			USHORT   DesignatorMin, DesignatorMax;
			USHORT   DataIndexMin, DataIndexMax;
		} Range;
		struct {
			USAGE    Usage, Reserved1;
			USHORT   StringIndex, Reserved2;
			USHORT   DesignatorIndex, Reserved3;
			USHORT   DataIndex, Reserved4;
		} NotRange;
	};

} HIDP_BUTTON_CAPS, *PHIDP_BUTTON_CAPS;

typedef struct _HIDP_LINK_COLLECTION_NODE
{
	USAGE    LinkUsage;
	USAGE    LinkUsagePage;
	USHORT   Parent;
	USHORT   NumberOfChildren;
	USHORT   NextSibling;
	USHORT   FirstChild;
	ULONG    CollectionType : 8;  // As defined in 6.2.2.6 of HID spec
	ULONG    IsAlias : 1; // This link node is an allias of the next link node.
	ULONG    Reserved : 23;
	PVOID    UserContext; // The user can hang his coat here.
} HIDP_LINK_COLLECTION_NODE, *PHIDP_LINK_COLLECTION_NODE;

typedef struct _HIDP_CAPS
{
	USAGE    Usage;
	USAGE    UsagePage;
	USHORT   InputReportByteLength;
	USHORT   OutputReportByteLength;
	USHORT   FeatureReportByteLength;
	USHORT   Reserved[17];

	USHORT   NumberLinkCollectionNodes;

	USHORT   NumberInputButtonCaps;
	USHORT   NumberInputValueCaps;
	USHORT   NumberInputDataIndices;

	USHORT   NumberOutputButtonCaps;
	USHORT   NumberOutputValueCaps;
	USHORT   NumberOutputDataIndices;

	USHORT   NumberFeatureButtonCaps;
	USHORT   NumberFeatureValueCaps;
	USHORT   NumberFeatureDataIndices;
} HIDP_CAPS, *PHIDP_CAPS;

typedef struct _HIDP_VALUE_CAPS
{
	USAGE    UsagePage;
	UCHAR    ReportID;
	BOOLEAN  IsAlias;

	USHORT   BitField;
	USHORT   LinkCollection;   // A unique internal index pointer

	USAGE    LinkUsage;
	USAGE    LinkUsagePage;

	BOOLEAN  IsRange;
	BOOLEAN  IsStringRange;
	BOOLEAN  IsDesignatorRange;
	BOOLEAN  IsAbsolute;

	BOOLEAN  HasNull;        // Does this channel have a null report   union
	UCHAR    Reserved;
	USHORT   BitSize;        // How many bits are devoted to this value?

	USHORT   ReportCount;    // See Note below.  Usually set to 1.
	USHORT   Reserved2[5];

	ULONG    UnitsExp;
	ULONG    Units;

	LONG     LogicalMin, LogicalMax;
	LONG     PhysicalMin, PhysicalMax;

	union {
		struct {
			USAGE    UsageMin, UsageMax;
			USHORT   StringMin, StringMax;
			USHORT   DesignatorMin, DesignatorMax;
			USHORT   DataIndexMin, DataIndexMax;
		} Range;

		struct {
			USAGE    Usage, Reserved1;
			USHORT   StringIndex, Reserved2;
			USHORT   DesignatorIndex, Reserved3;
			USHORT   DataIndex, Reserved4;
		} NotRange;
	};
} HIDP_VALUE_CAPS, *PHIDP_VALUE_CAPS;

typedef enum _HIDP_KEYBOARD_DIRECTION {
	HidP_Keyboard_Break,
	HidP_Keyboard_Make
} HIDP_KEYBOARD_DIRECTION;

typedef struct _HIDP_KEYBOARD_MODIFIER_STATE {
	union {
		struct {
			ULONG LeftControl : 1;
			ULONG LeftShift : 1;
			ULONG LeftAlt : 1;
			ULONG LeftGUI : 1;
			ULONG RightControl : 1;
			ULONG RightShift : 1;
			ULONG RightAlt : 1;
			ULONG RigthGUI : 1;
			ULONG CapsLock : 1;
			ULONG ScollLock : 1;
			ULONG NumLock : 1;
			ULONG Reserved : 21;
		};
		ULONG ul;
	};

} HIDP_KEYBOARD_MODIFIER_STATE, *PHIDP_KEYBOARD_MODIFIER_STATE;

typedef BOOLEAN(*PHIDP_INSERT_SCANCODES) (
	_In_opt_ PVOID Context,  // Some caller supplied context.
	_In_reads_bytes_(Length) PCHAR NewScanCodes, // A list of i8042 scan codes.
	_In_ ULONG Length // the length of the scan codes.
	);

#ifndef _NTDEF_
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
typedef NTSTATUS *PNTSTATUS;
#endif

extern HINSTANCE g_hHidDll;

extern TCHAR szLogBuffer[MAX_PATH];

extern "C" BOOLEAN __stdcall HidD_FlushQueue(
	_In_  HANDLE HidDeviceObject
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_FlushQueue"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_FlushQueue)(_In_ HANDLE HidDeviceObject);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_FlushQueue lpHidD_FlushQueue;
	lpHidD_FlushQueue = (HidD_FlushQueue)GetProcAddress(g_hHidDll, "HidD_FlushQueue");

	if (lpHidD_FlushQueue == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_FlushQueue(HidDeviceObject);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_FreePreparsedData(
	_In_  PHIDP_PREPARSED_DATA PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_FreePreparsedData"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_FreePreparsedData)(_In_  PHIDP_PREPARSED_DATA PreparsedData);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_FreePreparsedData lpHidD_FreePreparsedData;
	lpHidD_FreePreparsedData = (HidD_FreePreparsedData)GetProcAddress(g_hHidDll, "HidD_FreePreparsedData");

	if (lpHidD_FreePreparsedData == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_FreePreparsedData(PreparsedData);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetAttributes(_In_ HANDLE HidDeviceObject, _Out_ PHIDD_ATTRIBUTES Attributes)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetAttributes"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_GetAttributes)(_In_ HANDLE HidDeviceObject, _Out_ PHIDD_ATTRIBUTES Attributes);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetAttributes lpHidD_GetAttributes;
	lpHidD_GetAttributes = (HidD_GetAttributes)GetProcAddress(g_hHidDll, "HidD_GetAttributes");

	if (lpHidD_GetAttributes == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetAttributes(HidDeviceObject, Attributes);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetConfiguration(IN HANDLE  HidDeviceObject, OUT PHIDD_CONFIGURATION  Configuration, IN ULONG  ConfigurationLength)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetConfiguration"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_GetConfiguration)(IN HANDLE  HidDeviceObject, OUT PHIDD_CONFIGURATION  Configuration, IN ULONG  ConfigurationLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetConfiguration lpHidD_GetConfiguration;
	lpHidD_GetConfiguration = (HidD_GetConfiguration)GetProcAddress(g_hHidDll, "HidD_GetConfiguration");

	if (lpHidD_GetConfiguration == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetConfiguration(HidDeviceObject, Configuration, ConfigurationLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetFeature(_In_ HANDLE HidDeviceObject, _Out_ PVOID  ReportBuffer, _In_ ULONG  ReportBufferLength)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetFeature"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_GetFeature)(_In_ HANDLE HidDeviceObject, _Out_ PVOID  ReportBuffer, _In_ ULONG  ReportBufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetFeature lpHidD_GetFeature;
	lpHidD_GetFeature = (HidD_GetFeature)GetProcAddress(g_hHidDll, "HidD_GetFeature");

	if (lpHidD_GetFeature == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetFeature(HidDeviceObject, ReportBuffer, ReportBufferLength);

	return retVal;
}

extern "C" void __stdcall HidD_GetHidGuid(_Out_ LPGUID HidGuid)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetHidGuid"));
	OutputDebugString(szLogBuffer);

	typedef BOOLEAN(*__stdcall HidD_GetHidGuid)(_Out_ LPGUID HidGuid);

	if (g_hHidDll == NULL)
	{
		return;
	}

	HidD_GetHidGuid lpHidD_GetHidGuid;
	lpHidD_GetHidGuid = (HidD_GetHidGuid)GetProcAddress(g_hHidDll, "HidD_GetHidGuid");

	if (lpHidD_GetHidGuid == NULL)
	{
		return;
	}

	lpHidD_GetHidGuid(HidGuid);
}

extern "C" BOOLEAN __stdcall HidD_GetIndexedString(
	_In_   HANDLE HidDeviceObject,
	_In_   ULONG  StringIndex,
	_Out_  PVOID  Buffer,
	_In_   ULONG  BufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetIndexedString"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_GetIndexedString)(_In_ HANDLE HidDeviceObject, _In_ ULONG  StringIndex, _Out_ PVOID  Buffer, _In_ ULONG  BufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetIndexedString lpHidD_GetIndexedString;
	lpHidD_GetIndexedString = (HidD_GetIndexedString)GetProcAddress(g_hHidDll, "HidD_GetIndexedString");

	if (lpHidD_GetIndexedString == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetIndexedString(HidDeviceObject, StringIndex, Buffer, BufferLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetInputReport(_In_ HANDLE HidDeviceObject, _Out_ PVOID  ReportBuffer, _In_ ULONG  ReportBufferLength)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetInputReport"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_GetInputReport)(_In_ HANDLE HidDeviceObject, _Out_ PVOID  ReportBuffer, _In_ ULONG  ReportBufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetInputReport lpHidD_GetInputReport;
	lpHidD_GetInputReport = (HidD_GetInputReport)GetProcAddress(g_hHidDll, "HidD_GetInputReport");

	if (lpHidD_GetInputReport == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetInputReport(HidDeviceObject, ReportBuffer, ReportBufferLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetManufacturerString(
	_In_   HANDLE HidDeviceObject,
	_Out_  PVOID  Buffer,
	_In_   ULONG  BufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetManufacturerString"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_GetManufacturerString)(_In_ HANDLE HidDeviceObject, _Out_ PVOID  Buffer, _In_ ULONG  BufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetManufacturerString lpHidD_GetManufacturerString;
	lpHidD_GetManufacturerString = (HidD_GetManufacturerString)GetProcAddress(g_hHidDll, "HidD_GetManufacturerString");

	if (lpHidD_GetManufacturerString == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetManufacturerString(HidDeviceObject, Buffer, BufferLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetMsGenreDescriptor(
	IN HANDLE  HidDeviceObject,
	OUT PVOID  Buffer,
	IN ULONG  BufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetMsGenreDescriptor"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_GetMsGenreDescriptor)(IN HANDLE HidDeviceObject, OUT PVOID Buffer, IN ULONG BufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetMsGenreDescriptor lpHidD_GetMsGenreDescriptor;
	lpHidD_GetMsGenreDescriptor = (HidD_GetMsGenreDescriptor)GetProcAddress(g_hHidDll, "HidD_GetMsGenreDescriptor");

	if (lpHidD_GetMsGenreDescriptor == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetMsGenreDescriptor(HidDeviceObject, Buffer, BufferLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetNumInputBuffers(
	_In_   HANDLE HidDeviceObject,
	_Out_  PULONG NumberBuffers
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetNumInputBuffers"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_GetNumInputBuffers)(_In_ HANDLE HidDeviceObject, _Out_ PULONG NumberBuffers);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetNumInputBuffers lpHidD_GetNumInputBuffers;
	lpHidD_GetNumInputBuffers = (HidD_GetNumInputBuffers)GetProcAddress(g_hHidDll, "HidD_GetNumInputBuffers");

	if (lpHidD_GetNumInputBuffers == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetNumInputBuffers(HidDeviceObject, NumberBuffers);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetPhysicalDescriptor(
	_In_   HANDLE HidDeviceObject,
	_Out_  PVOID  Buffer,
	_In_   ULONG  BufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetPhysicalDescriptor"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_GetPhysicalDescriptor)(_In_ HANDLE HidDeviceObject, _Out_ PVOID Buffer, _In_ ULONG BufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetPhysicalDescriptor lpHidD_GetPhysicalDescriptor;
	lpHidD_GetPhysicalDescriptor = (HidD_GetPhysicalDescriptor)GetProcAddress(g_hHidDll, "HidD_GetPhysicalDescriptor");

	if (lpHidD_GetPhysicalDescriptor == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetPhysicalDescriptor(HidDeviceObject, Buffer, BufferLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetPreparsedData(
	_In_   HANDLE               HidDeviceObject,
	_Out_  PHIDP_PREPARSED_DATA *PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetPreparsedData"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_GetPreparsedData)(_In_ HANDLE HidDeviceObject, _Out_ PHIDP_PREPARSED_DATA *PreparsedData);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetPreparsedData lpHidD_GetPreparsedData;
	lpHidD_GetPreparsedData = (HidD_GetPreparsedData)GetProcAddress(g_hHidDll, "HidD_GetPreparsedData");

	if (lpHidD_GetPreparsedData == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetPreparsedData(HidDeviceObject, PreparsedData);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetProductString(
	_In_   HANDLE HidDeviceObject,
	_Out_  PVOID  Buffer,
	_In_   ULONG  BufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetProductString"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_GetProductString)(_In_ HANDLE HidDeviceObject, _Out_ PVOID Buffer, _In_ ULONG  BufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetProductString lpHidD_GetProductString;
	lpHidD_GetProductString = (HidD_GetProductString)GetProcAddress(g_hHidDll, "HidD_GetProductString");

	if (lpHidD_GetProductString == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetProductString(HidDeviceObject, Buffer, BufferLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_GetSerialNumberString(
	_In_   HANDLE HidDeviceObject,
	_Out_  PVOID  Buffer,
	_In_   ULONG  BufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_GetSerialNumberString"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal = FALSE;

	typedef BOOLEAN(*__stdcall HidD_GetSerialNumberString)(_In_ HANDLE HidDeviceObject, _Out_ PVOID Buffer, _In_ ULONG BufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_GetSerialNumberString lpHidD_GetSerialNumberString;
	lpHidD_GetSerialNumberString = (HidD_GetSerialNumberString)GetProcAddress(g_hHidDll, "HidD_GetSerialNumberString");

	if (lpHidD_GetSerialNumberString == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_GetSerialNumberString(HidDeviceObject, Buffer, BufferLength);

	return retVal;
}
extern "C" BOOLEAN __stdcall HidD_Hello(_Out_ PVOID Buffer, _In_ ULONG BufferLength)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_Hello"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_Hello)(_Out_ PVOID Buffer, _In_ ULONG BufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_Hello lpHidD_Hello;
	lpHidD_Hello = (HidD_Hello)GetProcAddress(g_hHidDll, "HidD_Hello");

	if (lpHidD_Hello == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_Hello(Buffer, BufferLength);

	return retVal;
}

extern "C" BOOLEAN HidD_SetConfiguration(
	IN HANDLE  HidDeviceObject,
	IN PHIDD_CONFIGURATION  Configuration,
	IN ULONG  ConfigurationLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_SetConfiguration"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_SetConfiguration)(IN HANDLE  HidDeviceObject, IN PHIDD_CONFIGURATION  Configuration, IN ULONG  ConfigurationLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_SetConfiguration lpHidD_SetConfiguration;
	lpHidD_SetConfiguration = (HidD_SetConfiguration)GetProcAddress(g_hHidDll, "HidD_SetConfiguration");

	if (lpHidD_SetConfiguration == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_SetConfiguration(HidDeviceObject, Configuration, ConfigurationLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_SetFeature(
	_In_  HANDLE HidDeviceObject,
	_In_  PVOID  ReportBuffer,
	_In_  ULONG  ReportBufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_SetFeature"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_SetFeature)(_In_ HANDLE HidDeviceObject, _In_ PVOID  ReportBuffer, _In_ ULONG  ReportBufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_SetFeature lpHidD_SetFeature;
	lpHidD_SetFeature = (HidD_SetFeature)GetProcAddress(g_hHidDll, "HidD_SetFeature");

	if (lpHidD_SetFeature == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_SetFeature(HidDeviceObject, ReportBuffer, ReportBufferLength);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_SetNumInputBuffers(
	_In_  HANDLE HidDeviceObject,
	_In_  ULONG  NumberBuffers
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_SetNumInputBuffers"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_SetNumInputBuffers)(_In_ HANDLE HidDeviceObject, _In_ ULONG NumberBuffers);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_SetNumInputBuffers lpHidD_SetNumInputBuffers;
	lpHidD_SetNumInputBuffers = (HidD_SetNumInputBuffers)GetProcAddress(g_hHidDll, "HidD_SetNumInputBuffers");

	if (lpHidD_SetNumInputBuffers == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_SetNumInputBuffers(HidDeviceObject, NumberBuffers);

	return retVal;
}

extern "C" BOOLEAN __stdcall HidD_SetOutputReport(
	_In_  HANDLE HidDeviceObject,
	_In_  PVOID  ReportBuffer,
	_In_  ULONG  ReportBufferLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidD_SetOutputReport"));
	OutputDebugString(szLogBuffer);

	BOOLEAN retVal;

	typedef BOOLEAN(*__stdcall HidD_SetOutputReport)(_In_ HANDLE HidDeviceObject, _In_ PVOID  ReportBuffer, _In_ ULONG  ReportBufferLength);

	if (g_hHidDll == NULL)
	{
		return FALSE;
	}

	HidD_SetOutputReport lpHidD_SetOutputReport;
	lpHidD_SetOutputReport = (HidD_SetOutputReport)GetProcAddress(g_hHidDll, "HidD_SetOutputReport");

	if (lpHidD_SetOutputReport == NULL)
	{
		return FALSE;
	}

	retVal = lpHidD_SetOutputReport(HidDeviceObject, ReportBuffer, ReportBufferLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetButtonCaps(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_Out_    PHIDP_BUTTON_CAPS    ButtonCaps,
	_Inout_  PUSHORT              ButtonCapsLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetButtonCaps"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetButtonCaps)(_In_ HIDP_REPORT_TYPE ReportType, _Out_ PHIDP_BUTTON_CAPS ButtonCaps, _Inout_ PUSHORT ButtonCapsLength, _In_ PHIDP_PREPARSED_DATA PreparsedData);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetButtonCaps lpHidP_GetButtonCaps;
	lpHidP_GetButtonCaps = (HidP_GetButtonCaps)GetProcAddress(g_hHidDll, "HidP_GetButtonCaps");

	if (lpHidP_GetButtonCaps == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetButtonCaps(ReportType, ButtonCaps, ButtonCapsLength, PreparsedData);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetCaps(
	_In_   PHIDP_PREPARSED_DATA PreparsedData,
	_Out_  PHIDP_CAPS           Capabilities
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetCaps"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetCaps)(_In_ PHIDP_PREPARSED_DATA PreparsedData, _Out_ PHIDP_CAPS Capabilities);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetCaps lpHidP_GetCaps;
	lpHidP_GetCaps = (HidP_GetCaps)GetProcAddress(g_hHidDll, "HidP_GetCaps");

	if (lpHidP_GetCaps == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetCaps(PreparsedData, Capabilities);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetData(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_Out_    PHIDP_DATA           DataList,
	_Inout_  PULONG               DataLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_In_     PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetData"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal = NULL;

	typedef NTSTATUS(*__stdcall HidP_GetData)(_In_ HIDP_REPORT_TYPE ReportType, _Out_ PHIDP_DATA DataList, _Inout_ PULONG DataLength, _In_ PHIDP_PREPARSED_DATA PreparsedData, _In_ PCHAR Report, _In_ ULONG ReportLength);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetData lpHidP_GetData;
	lpHidP_GetData = (HidP_GetData)GetProcAddress(g_hHidDll, "HidP_GetData");

	if (lpHidP_GetData == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetData(ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);

	return retVal;

}

extern "C" NTSTATUS __stdcall HidP_GetExtendedAttributes(
	_In_     HIDP_REPORT_TYPE          ReportType,
	_In_     USHORT                    DataIndex,
	_In_     PHIDP_PREPARSED_DATA      PreparsedData,
	_Out_    PHIDP_EXTENDED_ATTRIBUTES Attributes,
	_Inout_  PULONG                    LengthAttributes
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetExtendedAttributes"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal = NULL;

	typedef NTSTATUS(*__stdcall HidP_GetExtendedAttributes)(_In_ HIDP_REPORT_TYPE ReportType, _In_ USHORT DataIndex, _In_ PHIDP_PREPARSED_DATA PreparsedData, _Out_ PHIDP_EXTENDED_ATTRIBUTES Attributes, _Inout_ PULONG LengthAttributes);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetExtendedAttributes lpHidP_GetExtendedAttributes;
	lpHidP_GetExtendedAttributes = (HidP_GetExtendedAttributes)GetProcAddress(g_hHidDll, "HidP_GetExtendedAttributes");

	if (lpHidP_GetExtendedAttributes == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetExtendedAttributes(ReportType, DataIndex, PreparsedData, Attributes, LengthAttributes);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetLinkCollectionNodes(
	_Out_    PHIDP_LINK_COLLECTION_NODE LinkCollectionNodes,
	_Inout_  PULONG                     LinkCollectionNodesLength,
	_In_     PHIDP_PREPARSED_DATA       PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetLinkCollectionNodes"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetLinkCollectionNodes)(_Out_ PHIDP_LINK_COLLECTION_NODE LinkCollectionNodes, _Inout_ PULONG LinkCollectionNodesLength, _In_ PHIDP_PREPARSED_DATA PreparsedData);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetLinkCollectionNodes lpHidP_GetLinkCollectionNodes;
	lpHidP_GetLinkCollectionNodes = (HidP_GetLinkCollectionNodes)GetProcAddress(g_hHidDll, "HidP_GetLinkCollectionNodes");

	if (lpHidP_GetLinkCollectionNodes == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetLinkCollectionNodes(LinkCollectionNodes, LinkCollectionNodesLength, PreparsedData);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetScaledUsageValue(
	_In_   HIDP_REPORT_TYPE     ReportType,
	_In_   USAGE                UsagePage,
	_In_   USHORT               LinkCollection,
	_In_   USAGE                Usage,
	_Out_  PLONG                UsageValue,
	_In_   PHIDP_PREPARSED_DATA PreparsedData,
	_In_   PCHAR                Report,
	_In_   ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetScaledUsageValue"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetScaledUsageValue)(_In_ HIDP_REPORT_TYPE ReportType, _In_ USAGE UsagePage, _In_ USHORT LinkCollection, _In_ USAGE Usage, _Out_ PLONG UsageValue, _In_ PHIDP_PREPARSED_DATA PreparsedData, _In_ PCHAR Report, _In_ ULONG ReportLength);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetScaledUsageValue lpHidP_GetScaledUsageValue;
	lpHidP_GetScaledUsageValue = (HidP_GetScaledUsageValue)GetProcAddress(g_hHidDll, "HidP_GetScaledUsageValue");

	if (lpHidP_GetScaledUsageValue == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetScaledUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetSpecificButtonCaps(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_In_     USAGE                Usage,
	_Out_    PHIDP_BUTTON_CAPS    ButtonCaps,
	_Inout_  PUSHORT              ButtonCapsLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetSpecificButtonCaps"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetSpecificButtonCaps)(_In_ HIDP_REPORT_TYPE ReportType, _In_ USAGE UsagePage, _In_ USHORT LinkCollection, _In_ USAGE Usage, _Out_ PHIDP_BUTTON_CAPS ButtonCaps, _Inout_ PUSHORT ButtonCapsLength, _In_ PHIDP_PREPARSED_DATA PreparsedData);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetSpecificButtonCaps lpHidP_GetSpecificButtonCaps;
	lpHidP_GetSpecificButtonCaps = (HidP_GetSpecificButtonCaps)GetProcAddress(g_hHidDll, "HidP_GetSpecificButtonCaps");

	if (lpHidP_GetSpecificButtonCaps == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetSpecificButtonCaps(ReportType, UsagePage, LinkCollection, Usage, ButtonCaps, ButtonCapsLength, PreparsedData);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetSpecificValueCaps(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_In_     USAGE                Usage,
	_Out_    PHIDP_VALUE_CAPS     ValueCaps,
	_Inout_  PUSHORT              ValueCapsLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetSpecificValueCaps"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetSpecificValueCaps)(_In_ HIDP_REPORT_TYPE ReportType, _In_ USAGE UsagePage, _In_ USHORT LinkCollection, _In_ USAGE Usage, _Out_ PHIDP_VALUE_CAPS ValueCaps, _Inout_ PUSHORT ValueCapsLength, _In_ PHIDP_PREPARSED_DATA PreparsedData);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetSpecificValueCaps lpHidP_GetSpecificValueCaps;
	lpHidP_GetSpecificValueCaps = (HidP_GetSpecificValueCaps)GetProcAddress(g_hHidDll, "HidP_GetSpecificValueCaps");

	if (lpHidP_GetSpecificValueCaps == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetSpecificValueCaps(ReportType, UsagePage, LinkCollection, Usage, ValueCaps, ValueCapsLength, PreparsedData);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetUsageValue(
	_In_   HIDP_REPORT_TYPE     ReportType,
	_In_   USAGE                UsagePage,
	_In_   USHORT               LinkCollection,
	_In_   USAGE                Usage,
	_Out_  PULONG               UsageValue,
	_In_   PHIDP_PREPARSED_DATA PreparsedData,
	_In_   PCHAR                Report,
	_In_   ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetUsageValue"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetUsageValue)(_In_   HIDP_REPORT_TYPE     ReportType,
		_In_   USAGE                UsagePage,
		_In_   USHORT               LinkCollection,
		_In_   USAGE                Usage,
		_Out_  PULONG               UsageValue,
		_In_   PHIDP_PREPARSED_DATA PreparsedData,
		_In_   PCHAR                Report,
		_In_   ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetUsageValue lpHidP_GetUsageValue;
	lpHidP_GetUsageValue = (HidP_GetUsageValue)GetProcAddress(g_hHidDll, "HidP_GetUsageValue");

	if (lpHidP_GetUsageValue == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetUsageValueArray(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_In_     USAGE                Usage,
	_Inout_  PCHAR                UsageValue,
	_In_     USHORT               UsageValueByteLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_In_     PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetUsageValueArray"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetUsageValueArray)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_     USAGE                UsagePage,
		_In_     USHORT               LinkCollection,
		_In_     USAGE                Usage,
		_Inout_  PCHAR                UsageValue,
		_In_     USHORT               UsageValueByteLength,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_In_     PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetUsageValueArray lpHidP_GetUsageValueArray;
	lpHidP_GetUsageValueArray = (HidP_GetUsageValueArray)GetProcAddress(g_hHidDll, "HidP_GetUsageValueArray");

	if (lpHidP_GetUsageValueArray == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetUsageValueArray(ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetUsages(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_Out_    PUSAGE               UsageList,
	_Inout_  PULONG               UsageLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_Out_    PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetUsages"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetUsages)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_     USAGE                UsagePage,
		_In_     USHORT               LinkCollection,
		_Out_    PUSAGE               UsageList,
		_Inout_  PULONG               UsageLength,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_Out_    PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetUsages lpHidP_GetUsages;
	lpHidP_GetUsages = (HidP_GetUsages)GetProcAddress(g_hHidDll, "HidP_GetUsages");

	if (lpHidP_GetUsages == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetUsages(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_GetUsagesEx(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USHORT               LinkCollection,
	_Inout_  PUSAGE_AND_PAGE      ButtonList,
	_Inout_  ULONG                *UsageLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_In_     PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetUsagesEx"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*_stdcall HidP_GetUsagesEx)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_     USHORT               LinkCollection,
		_Inout_  PUSAGE_AND_PAGE      ButtonList,
		_Inout_  ULONG                *UsageLength,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_In_     PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetUsagesEx lpHidP_GetUsagesEx;
	lpHidP_GetUsagesEx = (HidP_GetUsagesEx)GetProcAddress(g_hHidDll, "HidP_GetUsagesEx");

	if (lpHidP_GetUsagesEx == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetUsagesEx(ReportType, LinkCollection, ButtonList, UsageLength, PreparsedData, Report, ReportLength);

	return retVal;
}


extern "C" NTSTATUS __stdcall HidP_GetValueCaps(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_Out_    PHIDP_VALUE_CAPS     ValueCaps,
	_Inout_  PUSHORT              ValueCapsLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_GetValueCaps"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_GetValueCaps)(_In_ HIDP_REPORT_TYPE ReportType,
		_Out_    PHIDP_VALUE_CAPS     ValueCaps,
		_Inout_  PUSHORT              ValueCapsLength,
		_In_     PHIDP_PREPARSED_DATA PreparsedData
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_GetValueCaps lpHidP_GetValueCaps;
	lpHidP_GetValueCaps = (HidP_GetValueCaps)GetProcAddress(g_hHidDll, "HidP_GetValueCaps");

	if (lpHidP_GetValueCaps == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_GetValueCaps(ReportType, ValueCaps, ValueCapsLength, PreparsedData);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_InitializeReportForID(
	_In_   HIDP_REPORT_TYPE     ReportType,
	_In_   UCHAR                ReportID,
	_In_   PHIDP_PREPARSED_DATA PreparsedData,
	_Out_  PCHAR                Report,
	_In_   ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_InitializeReportForID"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_InitializeReportForID)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_   UCHAR                ReportID,
		_In_   PHIDP_PREPARSED_DATA PreparsedData,
		_Out_  PCHAR                Report,
		_In_   ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_InitializeReportForID lpHidP_InitializeReportForID;
	lpHidP_InitializeReportForID = (HidP_InitializeReportForID)GetProcAddress(g_hHidDll, "HidP_InitializeReportForID");

	if (lpHidP_InitializeReportForID == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_InitializeReportForID(ReportType, ReportID, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" ULONG __stdcall HidP_MaxDataListLength(
	_In_  HIDP_REPORT_TYPE     ReportType,
	_In_  PHIDP_PREPARSED_DATA PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_MaxDataListLength"));
	OutputDebugString(szLogBuffer);

	ULONG retVal;

	typedef ULONG(*__stdcall HidP_MaxDataListLength)(_In_ HIDP_REPORT_TYPE ReportType, _In_ PHIDP_PREPARSED_DATA PreparsedData);

	if (g_hHidDll == NULL)
	{
		return 0;
	}

	HidP_MaxDataListLength lpHidP_MaxDataListLength;
	lpHidP_MaxDataListLength = (HidP_MaxDataListLength)GetProcAddress(g_hHidDll, "HidP_MaxDataListLength");

	if (lpHidP_MaxDataListLength == NULL)
	{
		return 0;
	}

	retVal = lpHidP_MaxDataListLength(ReportType, PreparsedData);

	return retVal;
}

extern "C" ULONG __stdcall HidP_MaxUsageListLength(
	_In_  HIDP_REPORT_TYPE     ReportType,
	_In_  USAGE                UsagePage,
	_In_  PHIDP_PREPARSED_DATA PreparsedData
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_MaxUsageListLength"));
	OutputDebugString(szLogBuffer);

	ULONG retVal;

	typedef ULONG(*__stdcall HidP_MaxUsageListLength)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_  USAGE                UsagePage,
		_In_  PHIDP_PREPARSED_DATA PreparsedData
		);

	if (g_hHidDll == NULL)
	{
		return 0;
	}

	HidP_MaxUsageListLength lpHidP_MaxUsageListLength;
	lpHidP_MaxUsageListLength = (HidP_MaxUsageListLength)GetProcAddress(g_hHidDll, "HidP_MaxUsageListLength");

	if (lpHidP_MaxUsageListLength == NULL)
	{
		return 0;
	}

	retVal = lpHidP_MaxUsageListLength(ReportType, UsagePage, PreparsedData);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_SetData(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_Inout_  PHIDP_DATA           DataList,
	_Inout_  PULONG               DataLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_In_     PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_SetData"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_SetData)(_In_ HIDP_REPORT_TYPE ReportType,
		_Inout_  PHIDP_DATA           DataList,
		_Inout_  PULONG               DataLength,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_In_     PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_SetData lpHidP_SetData;
	lpHidP_SetData = (HidP_SetData)GetProcAddress(g_hHidDll, "HidP_SetData");

	if (lpHidP_SetData)
	{
		return NULL;
	}

	retVal = lpHidP_SetData(ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_SetScaledUsageValue(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_In_     USAGE                Usage,
	_In_     LONG                 UsageValue,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_Inout_  PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_SetScaledUsageValue"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_SetScaledUsageValue)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_     USAGE                UsagePage,
		_In_     USHORT               LinkCollection,
		_In_     USAGE                Usage,
		_In_     LONG                 UsageValue,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_Inout_  PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_SetScaledUsageValue lpHidP_SetScaledUsageValue;
	lpHidP_SetScaledUsageValue = (HidP_SetScaledUsageValue)GetProcAddress(g_hHidDll, "HidP_SetScaledUsageValue");

	if (lpHidP_SetScaledUsageValue == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_SetScaledUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_SetUsageValue(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_In_     USAGE                Usage,
	_In_     ULONG                UsageValue,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_Inout_  PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_SetUsageValue"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_SetUsageValue)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_     USAGE                UsagePage,
		_In_     USHORT               LinkCollection,
		_In_     USAGE                Usage,
		_In_     ULONG                UsageValue,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_Inout_  PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_SetUsageValue lpHidP_SetUsageValue;
	lpHidP_SetUsageValue = (HidP_SetUsageValue)GetProcAddress(g_hHidDll, "HidP_SetUsageValue");

	if (lpHidP_SetUsageValue == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_SetUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_SetUsageValueArray(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_In_     USAGE                Usage,
	_In_     PCHAR                UsageValue,
	_In_     USHORT               UsageValueByteLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_Inout_  PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_SetUsageValueArray"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_SetUsageValueArray)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_     USAGE                UsagePage,
		_In_     USHORT               LinkCollection,
		_In_     USAGE                Usage,
		_In_     PCHAR                UsageValue,
		_In_     USHORT               UsageValueByteLength,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_Inout_  PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_SetUsageValueArray lpHidP_SetUsageValueArray;
	lpHidP_SetUsageValueArray = (HidP_SetUsageValueArray)GetProcAddress(g_hHidDll, "HidP_SetUsageValueArray");

	if (lpHidP_SetUsageValueArray == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_SetUsageValueArray(ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_SetUsages(
	_In_     HIDP_REPORT_TYPE     ReportType,
	_In_     USAGE                UsagePage,
	_In_     USHORT               LinkCollection,
	_Inout_  PUSAGE               UsageList,
	_Inout_  PULONG               UsageLength,
	_In_     PHIDP_PREPARSED_DATA PreparsedData,
	_In_     PCHAR                Report,
	_In_     ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_SetUsages"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_SetUsages)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_     USAGE                UsagePage,
		_In_     USHORT               LinkCollection,
		_Inout_  PUSAGE               UsageList,
		_Inout_  PULONG               UsageLength,
		_In_     PHIDP_PREPARSED_DATA PreparsedData,
		_In_     PCHAR                Report,
		_In_     ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_SetUsages lpHidP_SetUsages;
	lpHidP_SetUsages = (HidP_SetUsages)GetProcAddress(g_hHidDll, "HidP_SetUsages");

	if (lpHidP_SetUsages == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_SetUsages(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_TranslateUsagesToI8042ScanCodes(
	_In_      PUSAGE                        ChangedUsageList,
	_In_      ULONG                         UsageListLength,
	_In_      HIDP_KEYBOARD_DIRECTION       KeyAction,
	_Inout_   PHIDP_KEYBOARD_MODIFIER_STATE ModifierState,
	_In_      PHIDP_INSERT_SCANCODES        InsertCodesProcedure,
	_In_opt_  PVOID                         InsertCodesContext
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_TranslateUsagesToI8042ScanCodes"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_TranslateUsagesToI8042ScanCodes)(_In_ PUSAGE ChangedUsageList,
		_In_      ULONG                         UsageListLength,
		_In_      HIDP_KEYBOARD_DIRECTION       KeyAction,
		_Inout_   PHIDP_KEYBOARD_MODIFIER_STATE ModifierState,
		_In_      PHIDP_INSERT_SCANCODES        InsertCodesProcedure,
		_In_opt_  PVOID                         InsertCodesContext
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_TranslateUsagesToI8042ScanCodes lpHidP_TranslateUsagesToI8042ScanCodes;
	lpHidP_TranslateUsagesToI8042ScanCodes = (HidP_TranslateUsagesToI8042ScanCodes)GetProcAddress(g_hHidDll, "HidP_TranslateUsagesToI8042ScanCodes");

	if (lpHidP_TranslateUsagesToI8042ScanCodes == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_TranslateUsagesToI8042ScanCodes(ChangedUsageList, UsageListLength, KeyAction, ModifierState, InsertCodesProcedure, InsertCodesContext);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_UnsetUsages(
	_In_      HIDP_REPORT_TYPE     ReportType,
	_In_      USAGE                UsagePage,
	_In_opt_  USHORT               LinkCollection,
	_Inout_   PUSAGE               UsageList,
	_Inout_   PULONG               UsageLength,
	_In_      PHIDP_PREPARSED_DATA PreparsedData,
	_In_      PCHAR                Report,
	_In_      ULONG                ReportLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_UnsetUsages"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_UnsetUsages)(_In_ HIDP_REPORT_TYPE ReportType,
		_In_      USAGE                UsagePage,
		_In_opt_  USHORT               LinkCollection,
		_Inout_   PUSAGE               UsageList,
		_Inout_   PULONG               UsageLength,
		_In_      PHIDP_PREPARSED_DATA PreparsedData,
		_In_      PCHAR                Report,
		_In_      ULONG                ReportLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_UnsetUsages lpHidP_UnsetUsages;
	lpHidP_UnsetUsages = (HidP_UnsetUsages)GetProcAddress(g_hHidDll, "HidP_UnsetUsages");

	if (lpHidP_UnsetUsages == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_UnsetUsages(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);

	return retVal;
}

extern "C" NTSTATUS __stdcall HidP_UsageListDifference(
	_In_   PUSAGE PreviousUsageList,
	_In_   PUSAGE CurrentUsageList,
	_Out_  PUSAGE BreakUsageList,
	_Out_  PUSAGE MakeUsageList,
	_In_   ULONG  UsageListLength
)
{
	_stprintf(szLogBuffer, _T("Feelanet HID : HidP_UsageListDifference"));
	OutputDebugString(szLogBuffer);

	NTSTATUS retVal;

	typedef NTSTATUS(*__stdcall HidP_UsageListDifference)(_In_ PUSAGE PreviousUsageList,
		_In_   PUSAGE CurrentUsageList,
		_Out_  PUSAGE BreakUsageList,
		_Out_  PUSAGE MakeUsageList,
		_In_   ULONG  UsageListLength
		);

	if (g_hHidDll == NULL)
	{
		return NULL;
	}

	HidP_UsageListDifference lpHidP_UsageListDifference;
	lpHidP_UsageListDifference = (HidP_UsageListDifference)GetProcAddress(g_hHidDll, "HidP_UsageListDifference");

	if (lpHidP_UsageListDifference == NULL)
	{
		return NULL;
	}

	retVal = lpHidP_UsageListDifference(PreviousUsageList, CurrentUsageList, BreakUsageList, MakeUsageList, UsageListLength);

	return retVal;
}























































