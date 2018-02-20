#pragma once

#include "windows.h"

//
// imath.h
//

// {E0D52819-87D5-49BB-867E-D4ACF3DB6B40}
DEFINE_GUID(CLSID_CMath,
	0xe0d52819, 0x87d5, 0x49bb, 0x86, 0x7e, 0xd4, 0xac, 0xf3, 0xdb, 0x6b, 0x40);

// {DDB51A3E-9D7E-42A3-904F-11B3B15256FE}
DEFINE_GUID(IID_IMath,
	0xddb51a3e, 0x9d7e, 0x42a3, 0x90, 0x4f, 0x11, 0xb3, 0xb1, 0x52, 0x56, 0xfe);


class IMath : public IUnknown
{
public:
	STDMETHOD(Add(long, long, long*))      PURE;
	STDMETHOD(Subtract(long, long, long*)) PURE;
	STDMETHOD(Multiply(long, long, long*)) PURE;
	STDMETHOD(Divide(long, long, long*))   PURE;
};
