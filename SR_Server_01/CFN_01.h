#pragma once


#include "IFN_01.h"
#include "IVer.h"

extern long g_lObjs;
extern long g_lLocks;


class CFN_01 : public IFN_01, public IVer
{
protected:

	// Reference count
	long          m_lRef;

public:
	CFN_01();
	~CFN_01();

public:

	// IUnknown
	STDMETHOD(QueryInterface(REFIID, void**));
	STDMETHOD_(ULONG, AddRef());
	STDMETHOD_(ULONG, Release());

	// IFN_01
	STDMETHOD(Fun11(int, int, int*));
	STDMETHOD(Fun12(int, int, int, float*));
	STDMETHOD(Fun13(double in, double *out));

	// IVer
	STDMETHOD(GetAutor(wchar_t** author));

};


class FN_01ClassFactory : public IClassFactory
{
protected:
	long       m_lRef;

public:
	FN_01ClassFactory();
	~FN_01ClassFactory();

	// IUnknown
	STDMETHOD(QueryInterface(REFIID, void**));
	STDMETHOD_(ULONG, AddRef());
	STDMETHOD_(ULONG, Release());

	// IClassFactory
	STDMETHOD(CreateInstance(LPUNKNOWN, REFIID, void**));
	STDMETHOD(LockServer(BOOL));
};

