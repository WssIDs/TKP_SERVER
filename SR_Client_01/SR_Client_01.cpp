// SR_Client_01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#include <initguid.h>
#include "../SR_Server_01/IСustomFun.h"

int main(int argc, char *argv[])
{
	printf("Initializing COM \n");
	if (FAILED(CoInitialize(NULL)))
	{
		printf("Unable to initialize COM \n"); return -1;
	}

	char progid[] = "CustomFun.Component.1";

	char* szProgID = progid;
	WCHAR szWideProgID[128];
	CLSID clsid;
	long lLen = MultiByteToWideChar(CP_ACP, 0, szProgID,
		strlen(szProgID), szWideProgID, sizeof(szWideProgID));
	szWideProgID[lLen] = '\0';
	HRESULT hr = ::CLSIDFromProgID(szWideProgID, &clsid);
	if (FAILED(hr))
	{
		printf("Unable to get CLSID from ProgID. HR = %X \n", hr); return -1;
	}

	IClassFactory* pCF;
	// Получить фабрику класса для CMath 
	hr = CoGetClassObject(clsid, CLSCTX_INPROC, NULL,
		IID_IClassFactory, (void**)&pCF);
	if (FAILED(hr))
	{
		printf("Failed to GetClassObject server instance. HR = %X \n", hr); return -1;
	}

	// с помощью фабрики класса создать экземпляр 
	// компонента и получить интерфейс IUnknown. 

	IUnknown* pUnk; hr = pCF->CreateInstance(NULL, IID_IUnknown, (void**)&pUnk);

	// Release the class factory pCF->Release(); 
	if (FAILED(hr))
	{
		printf("Failed to create server instance. HR =%X \n", hr); return -1;
	}
	printf("Instance created \n");
	ICustomFun* pCFun = NULL;
	hr = pUnk->QueryInterface(IID_ICustomFun, (LPVOID*)&pCFun);
	pUnk->Release();
	if (FAILED(hr))
	{
		printf("QueryInterface() for IMath failed \n"); return -1;
	}

	int iResult;
	pCFun->Fun11(15, 5, &iResult);
	printf("15 - 5 is %d \n", iResult);

	float fResult;
	pCFun->Fun12(100, 50, 200, &fResult);
	printf("100 + 50 + 200 is %lf \n", fResult);
	printf("Releasing instance \n");

	double dResult;
	pCFun->Fun13(10, &dResult);
	printf("10 *10 is %lf \n", dResult);
	printf("Releasing instance \n");

	pCFun->Release();
	printf("Shuting down COM\n");
	CoUninitialize();

	system("pause");
	return 0;
}

