
#pragma once
#include <Unknwn.h>

//COMポインタ用
inline void intrusive_ptr_add_ref(IUnknown *p)
{
	p->AddRef();
}
inline void intrusive_ptr_release(IUnknown *p)
{
	p->Release();
}

// COMをリリース
inline void SafeRelease(IUnknown *p)
{
	if(p)
	{
		(p)->Release();
		p = NULL;
	}
}