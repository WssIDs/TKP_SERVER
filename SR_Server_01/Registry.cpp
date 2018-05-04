//
// Registry.cpp
//

#include <objbase.h>
#include <assert.h>
#include <tchar.h>
#include <stdio.h>
#include "Registry.h"

////////////////////////////////////////////////////////
//
// Internal helper functions prototypes
//

// Set the given key and its value.
BOOL setKeyAndValue(const wchar_t* szKey,
	const wchar_t* szSubkey,
	const wchar_t* szValue);

// Delete szKeyChild and all of its descendents.
LONG recursiveDeleteKey(HKEY hKeyParent,const wchar_t* szKeyChild);

////////////////////////////////////////////////////////
//
// Constants
//

// Size of a CLSID as a string
const int CLSID_STRING_SIZE = 39;

/////////////////////////////////////////////////////////
//
// Public function implementation
//

//
// Register the component in the registry.
//
HRESULT RegisterServer(HMODULE hModule, const CLSID& clsid,const wchar_t* szFriendlyName,const wchar_t* szVerIndProgID,const wchar_t* szProgID)
{
	// Get server location.
	wchar_t* szModule = new wchar_t[512];

	DWORD dwResult =
		::GetModuleFileName(hModule,
			szModule,
			_tcslen(szModule) * sizeof(TCHAR));
	
	assert(dwResult != 0);

	wchar_t* szCLSID = new wchar_t[CLSID_STRING_SIZE];

	StringFromCLSID(clsid, &szCLSID);

	// Build the key CLSID\\{...}
	wchar_t* szKey = new wchar_t[1024];
	wcscpy(szKey, L"CLSID\\");

	wcscat(szKey, szCLSID);

	// Add the CLSID to the registry.
	setKeyAndValue(szKey, NULL, szFriendlyName);

	// Add the server filename subkey under the CLSID key.
	setKeyAndValue(szKey, L"InprocServer32", szModule);

	// Add the ProgID subkey under the CLSID key.
	setKeyAndValue(szKey, L"ProgID", szProgID);

	// Add the version-independent ProgID subkey under CLSID key.
	setKeyAndValue(szKey, L"VersionIndependentProgID", szVerIndProgID);

	// Add the version-independent ProgID subkey under HKEY_CLASSES_ROOT.
	setKeyAndValue(szVerIndProgID, NULL, szFriendlyName);

	setKeyAndValue(szVerIndProgID, L"CLSID", szCLSID);
	setKeyAndValue(szVerIndProgID, L"CurVer", szProgID);

	// Add the versioned ProgID subkey under HKEY_CLASSES_ROOT.
	setKeyAndValue(szProgID, NULL, szFriendlyName);
	setKeyAndValue(szProgID, L"CLSID", szCLSID);

	return S_OK;
}

HRESULT UnregisterServer(const CLSID& clsid, const wchar_t* szVerIndProgID,const wchar_t* szProgID)
{
	// Convert the CLSID into a char.
	wchar_t* szCLSID = new wchar_t[CLSID_STRING_SIZE];

	StringFromCLSID(clsid, &szCLSID);

	// Build the key CLSID\\{...}
	wchar_t* szKey = new wchar_t[64];
	wcscpy(szKey, L"CLSID\\");
	wcscat(szKey, szCLSID);

	// Delete the CLSID Key - CLSID\{...}
	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	assert((lResult == ERROR_SUCCESS) ||
		(lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

											// Delete the version-independent ProgID Key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	assert((lResult == ERROR_SUCCESS) ||
		(lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

											// Delete the ProgID key.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	assert((lResult == ERROR_SUCCESS) ||
		(lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	return S_OK;
}

//
// Delete a key and all of its descendents.
//
LONG recursiveDeleteKey(HKEY hKeyParent,           // Parent of key to delete
	const wchar_t* lpszKeyChild)  // Key to delete
{
	// Open the child.
	HKEY hKeyChild;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0,
		KEY_ALL_ACCESS, &hKeyChild);
	if (lRes != ERROR_SUCCESS)
	{
		return lRes;
	}

	// Enumerate all of the decendents of this child.
	FILETIME time;
	TCHAR szBuffer[256];
	DWORD dwSize = 256;

	while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL,
		NULL, NULL, &time) == S_OK)
	{
		// Delete the decendents of this child.
		lRes = recursiveDeleteKey(hKeyChild, szBuffer);
		if (lRes != ERROR_SUCCESS)
		{
			// Cleanup before exiting.
			RegCloseKey(hKeyChild);
			return lRes;
		}
		dwSize = 256;
	}

	// Close the child.
	RegCloseKey(hKeyChild);

	// Delete this child.
	return RegDeleteKey(hKeyParent, lpszKeyChild);
}

//
// Create a key and set its value.
//   - This helper function was borrowed and modifed from
//     Kraig Brockschmidt's book Inside OLE.
//
BOOL setKeyAndValue(const wchar_t* szKey,
	const wchar_t* szSubkey,
	const wchar_t* szValue)
{
	HKEY hKey;
	wchar_t* szKeyBuf= new wchar_t[1024];

	// Copy keyname into buffer.
	wcscpy(szKeyBuf, szKey);

	// Add subkey name to buffer.
	if (szSubkey != NULL)
	{
		wcscat(szKeyBuf, TEXT("\\"));
		wcscat(szKeyBuf, szSubkey);
	}

	// Create and open key and subkey.
	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT,
		szKeyBuf,
		0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL,
		&hKey, NULL);
	if (lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	// Set the Value.
	if (szValue != NULL)
	{
		RegSetValueEx(hKey, NULL, 0, REG_SZ,
			(BYTE*)szValue,
			_tcslen(szValue) * sizeof(TCHAR));
	}

	RegCloseKey(hKey);
	return TRUE;
}
