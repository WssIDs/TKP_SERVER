#include <windows.h>
#include <tchar.h>
#include "CFN_01.h"

CFN_01::CFN_01()
{
	m_lRef = 0;

	// Увеличить значение внешнего счетчика объектов
	InterlockedIncrement(&g_lObjs);
}


CFN_01::~CFN_01()
{
	// Уменьшить значение внешнего счетчика объектов
	InterlockedDecrement(&g_lObjs);


}

STDMETHODIMP CFN_01::Fun11(int iOp1, int iOp2, int* pResult)
{
	*pResult = iOp1 - iOp2;
	return S_OK;
}

STDMETHODIMP CFN_01::Fun12(int iOp1, int iOp2, int iOp3, float* pResult)
{
	*pResult = (float)iOp1 + (float)iOp2 + (float)iOp3;
	return S_OK;
}

STDMETHODIMP CFN_01::Fun13(double in, double *out)
{
	*out = in * in;
	return S_OK;
}

STDMETHODIMP CFN_01::GetAutor(wchar_t** author)
{
	wchar_t text[] = L"Volodko 60331-1";
	lstrcpy(*author, text);

	return S_OK;
}

STDMETHODIMP_(ULONG) CFN_01::Release()
{
	if (InterlockedDecrement(&m_lRef) == 0)
	{
		delete this;
		return 0;
	}

	return m_lRef;
}

STDMETHODIMP_(ULONG) CFN_01::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

STDMETHODIMP CFN_01::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = 0;


	if (riid == IID_IUnknown || riid == IID_IFN_01)
	{
		// Клиент запрашивает интерфейс IFN_01
		*ppv = (IFN_01*)this;
	}
	else if (riid == IID_IVer)
	{
		// Клиент запрашивает интерфейс IVer
		*ppv = (IVer*)this;
	}   

	if (*ppv)
	{
		AddRef();
		return(S_OK);
	}
	return (E_NOINTERFACE);
}


/* FNClassFactory  реализация */

FN_01ClassFactory::FN_01ClassFactory()
{
	m_lRef = 0;
}

FN_01ClassFactory::~FN_01ClassFactory()
{
}

STDMETHODIMP FN_01ClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = 0;

	if (riid == IID_IUnknown || riid == IID_IClassFactory)
		*ppv = this;

	if (*ppv)
	{
		AddRef();
		return S_OK;
	}

	return(E_NOINTERFACE);
}

STDMETHODIMP_(ULONG) FN_01ClassFactory::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

STDMETHODIMP_(ULONG) FN_01ClassFactory::Release()
{
	if (InterlockedDecrement(&m_lRef) == 0)
	{
		delete this;
		return 0;
	}

	return m_lRef;
}

STDMETHODIMP FN_01ClassFactory::CreateInstance
(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
{
	CFN_01*      pCFun;
	HRESULT    hr;

	*ppvObj = 0;

	pCFun = new CFN_01;

	if (pCFun == 0)
		return(E_OUTOFMEMORY);

	hr = pCFun->QueryInterface(riid, ppvObj);

	if (FAILED(hr))
		delete pCFun;

	return hr;
}

STDMETHODIMP FN_01ClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&g_lLocks);
	else
		InterlockedDecrement(&g_lLocks);

	return S_OK;
}