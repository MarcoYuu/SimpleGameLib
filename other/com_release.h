
#pragma once
#include <Unknwn.h>

//COM�|�C���^�p
inline void intrusive_ptr_add_ref(IUnknown *p)
{
	p->AddRef();
}
inline void intrusive_ptr_release(IUnknown *p)
{
	p->Release();
}

// COM�������[�X
inline void SafeRelease(IUnknown *p)
{
	if(p)
	{
		(p)->Release();
		p = NULL;
	}
}