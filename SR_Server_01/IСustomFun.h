#pragma once

#include "windows.h"

//
// ICustomFun.h
//

// {1EED271A-CD92-4C92-9EA9-75A16AF866C9}
DEFINE_GUID(CLSID_CCustomFun,
	0x1eed271a, 0xcd92, 0x4c92, 0x9e, 0xa9, 0x75, 0xa1, 0x6a, 0xf8, 0x66, 0xc9);


// {7D0D94C1-E5AC-411D-83FF-ADE984265DD3}
DEFINE_GUID(IID_ICustomFun,
	0x7d0d94c1, 0xe5ac, 0x411d, 0x83, 0xff, 0xad, 0xe9, 0x84, 0x26, 0x5d, 0xd3);


class ICustomFun : public IUnknown
{
public:

	STDMETHOD(Fun11(int, int, int*)) PURE;
	STDMETHOD(Fun12(int, int, int, float*)) PURE;
	STDMETHOD(Fun13(double in, double *out)) PURE;
};
