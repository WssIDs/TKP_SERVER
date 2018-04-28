#pragma once


#include "IÑustomFun.h"

extern long g_lObjs;
extern long g_lLocks;


class CCustomFun : public ICustomFun
{
protected:

	// Reference count
	long          m_lRef;

public:
	CCustomFun();
	~CCustomFun();

public:

	// IUnknown
	STDMETHOD(QueryInterface(REFIID, void**));
	STDMETHOD_(ULONG, AddRef());
	STDMETHOD_(ULONG, Release());


	//ICustomFun
	STDMETHOD(Fun11(int, int, int*));
	STDMETHOD(Fun12(int, int, int, float*));
	STDMETHOD(Fun13(double in, double *out));

};


class CustomFunClassFactory : public IClassFactory
{
protected:
	long       m_lRef;

public:
	CustomFunClassFactory();
	~CustomFunClassFactory();

	// IUnknown
	STDMETHOD(QueryInterface(REFIID, void**));
	STDMETHOD_(ULONG, AddRef());
	STDMETHOD_(ULONG, Release());

	// IClassFactory
	STDMETHOD(CreateInstance(LPUNKNOWN, REFIID, void**));
	STDMETHOD(LockServer(BOOL));
};

