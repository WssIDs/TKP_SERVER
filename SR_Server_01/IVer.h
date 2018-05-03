#pragma once

#include "windows.h"

// {CF323E3F-B526-47BA-85B8-23BB43B5700C}
DEFINE_GUID(IID_IVer,
	0xcf323e3f, 0xb526, 0x47ba, 0x85, 0xb8, 0x23, 0xbb, 0x43, 0xb5, 0x70, 0xc);



interface IVer : public IUnknown
{
public:

	STDMETHOD(GetAutor(wchar_t** author)) PURE;
};