#pragma once
#include <Windows.h>
//
// Registry.h
//   - Helper functions registering and unregistering a component.
//

// This function will register a component in the Registry.
// The component calls this function from its DllRegisterServer function.
HRESULT RegisterServer(HMODULE hModule,
	const CLSID& clsid,
	const wchar_t* szFriendlyName,
	const wchar_t* szVerIndProgID,
	const wchar_t* szProgID);

// This function will unregister a component.  Components
// call this function from their DllUnregisterServer function.
HRESULT UnregisterServer(const CLSID& clsid,
	const wchar_t* szVerIndProgID,
	const wchar_t* szProgID);