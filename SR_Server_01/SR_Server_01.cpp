//
// sr_server_01.cpp : Defines the initialization routines for the DLL.
//

#include <windows.h>
#include <tchar.h>
#include <initguid.h>
#include "CFN_01.h"
#include "Registry.h"

long    g_lObjs = 0;
long    g_lLocks = 0;


///////////////////////////////////////////////////////////
//
// Global variables
//

// Static variables
static HMODULE g_hModule = NULL;  // DLL module handle
								  // Friendly name of component

LPCTSTR  g_szFriendlyName = TEXT("Lab1 FN_01 Component");
// Version-independent ProgID
LPCTSTR  g_szVerIndProgID = TEXT("FN_01.Component");
// ProgID
LPCTSTR  g_szProgID = TEXT("FN_01.Component.1");





STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	HRESULT             hr;
	FN_01ClassFactory    *pCF;

	pCF = 0;

	// Make sure the CLSID is for our Expression component
	if (rclsid != CLSID_CFN_01)
		return(E_FAIL);

	pCF = new FN_01ClassFactory;

	if (pCF == 0)
		return(E_OUTOFMEMORY);

	hr = pCF->QueryInterface(riid, ppv);

	// Check for failure of QueryInterface
	if (FAILED(hr))
	{
		delete pCF;
		pCF = 0;
	}

	return hr;
}

STDAPI DllCanUnloadNow(void)
{
	if (g_lObjs || g_lLocks)
		return(S_FALSE);
	else
		return(S_OK);
}

//
// Server registration
//
STDAPI DllRegisterServer()
{
	return RegisterServer(g_hModule,
		CLSID_CFN_01,
		g_szFriendlyName,
		g_szVerIndProgID,
		g_szProgID);
}


STDAPI DllUnregisterServer()
{
	return UnregisterServer(CLSID_CFN_01,
		g_szVerIndProgID,
		g_szProgID);
}

///////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD dwReason,
	void* lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		g_hModule = (HMODULE)hModule;
	}
	return TRUE;
}

