#include <windows.h>
#include "CCustomFun.h"

CCustomFun::CCustomFun()
{
	m_lRef = 0;

	// Увеличить значение внешнего счетчика объектов
	InterlockedIncrement(&g_lObjs);
}


CCustomFun::~CCustomFun()
{
	// Уменьшить значение внешнего счетчика объектов
	InterlockedDecrement(&g_lObjs);


}


STDMETHODIMP CCustomFun::Fun11(int iOp1, int iOp2, int* pResult)
{
	*pResult = iOp1 - iOp2;
	return S_OK;
}

STDMETHODIMP CCustomFun::Fun12(int iOp1, int iOp2, int iOp3, float* pResult)
{
	*pResult = (float)iOp1 + (float)iOp2 + (float)iOp3;
	return S_OK;
}

STDMETHODIMP CCustomFun::Fun13(double in, double *out)
{
	*out = in * in;
	return S_OK;
}

STDMETHODIMP_(ULONG) CCustomFun::Release()
{
	if (InterlockedDecrement(&m_lRef) == 0)
	{
		delete this;
		return 0;
	}

	return m_lRef;
}

STDMETHODIMP_(ULONG) CCustomFun::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

STDMETHODIMP CCustomFun::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = 0;

	if (riid == IID_IUnknown || riid == IID_ICustomFun)
		*ppv = this;

	if (*ppv)
	{
		AddRef();
		return(S_OK);
	}
	return (E_NOINTERFACE);
}


/* MathClasFactory  реализация */

CustomFunClassFactory::CustomFunClassFactory()
{
	m_lRef = 0;
}

CustomFunClassFactory::~CustomFunClassFactory()
{
}

STDMETHODIMP CustomFunClassFactory::QueryInterface(REFIID riid, void** ppv)
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

STDMETHODIMP_(ULONG) CustomFunClassFactory::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

STDMETHODIMP_(ULONG) CustomFunClassFactory::Release()
{
	if (InterlockedDecrement(&m_lRef) == 0)
	{
		delete this;
		return 0;
	}

	return m_lRef;
}

STDMETHODIMP CustomFunClassFactory::CreateInstance
(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
{
	CCustomFun*      pCFun;
	HRESULT    hr;

	*ppvObj = 0;

	pCFun = new CCustomFun;

	if (pCFun == 0)
		return(E_OUTOFMEMORY);

	hr = pCFun->QueryInterface(riid, ppvObj);

	if (FAILED(hr))
		delete pCFun;

	return hr;
}

STDMETHODIMP CustomFunClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
		InterlockedIncrement(&g_lLocks);
	else
		InterlockedDecrement(&g_lLocks);

	return S_OK;
}