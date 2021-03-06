// SR_Client_01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <initguid.h>
#include "../SR_Server_01/IFN_01.h"
#include "../SR_Server_01/IVer.h"

int main(int argc, char *argv[])
{

	printf("Initializing COM \n");
	if (FAILED(CoInitialize(NULL)))
	{
		printf("Unable to initialize COM \n");
	}

	char progid[] = "FN_01.Component.1";

	char* szProgID = progid;
	WCHAR szWideProgID[128];
	CLSID clsid;
	long lLen = MultiByteToWideChar(CP_ACP, 0, szProgID,
		strlen(szProgID), szWideProgID, sizeof(szWideProgID));
	szWideProgID[lLen] = '\0';
	HRESULT hr = ::CLSIDFromProgID(szWideProgID, &clsid);
	if (FAILED(hr))
	{
		printf("Unable to get CLSID from ProgID. HR = %X \n", hr);
	}

	IClassFactory* pCF;
	// Получить фабрику класса для CMath 
	hr = CoGetClassObject(clsid, CLSCTX_INPROC, NULL,
		IID_IClassFactory, (void**)&pCF);
	if (FAILED(hr))
	{
		printf("Failed to GetClassObject server instance. HR = %X \n", hr);
	}

	// с помощью фабрики класса создать экземпляр 
	// компонента и получить интерфейс IUnknown. 

	IUnknown* pUnk;
	hr = pCF->CreateInstance(NULL, IID_IUnknown, (void**)&pUnk);

	if (SUCCEEDED(hr)) 
	{
		printf("Success Unknown \n");

		IFN_01* pFN_01 = NULL;
		hr = pUnk->QueryInterface(IID_IFN_01, (void**)&pFN_01);
		if (SUCCEEDED(hr))
		{
			printf("Success IFN_01 \n");
			
			int resInt;
			pFN_01->Fun11(10,10,&resInt);

			float resFloat;
			pFN_01->Fun12(10, 10,34, &resFloat);

			double resDouble;
			pFN_01->Fun13(5,&resDouble);

			pFN_01->Release(); 

			printf("Result Fun11 10-10 = %d\n", resInt);
			printf("Result Fun12 10+10+34 = %lf\n", resFloat);
			printf("Result Fun13 5*5 = %lf\n", resDouble);
		}  
		else
		{
			printf("Failed to get IFN_01. HR =%X \n", hr);
		}

		IVer* pVer = NULL;
		hr = pUnk->QueryInterface(IID_IVer, (void**)&pVer);
		if (SUCCEEDED(hr))
		{
			printf("Success IVer \n");
			
			wchar_t* author = new wchar_t[100];
			pVer->GetAutor(&author);

			printf("Author: %ls\n",author);

			pVer->Release();
		}
		else 
		{
			printf("Failed to get IVer. HR =%X \n", hr);
		}
		
		pUnk->Release();
	}
	else 
	{ 
		printf("Failed to create component instance. HR =%X \n", hr);
	}

	printf("Shuting down COM\n");
	CoUninitialize();

	system("pause");
	return 0;
}

