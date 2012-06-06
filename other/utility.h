#pragma once

#include <string>
#include <tchar.h>
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

// �����̃��C�u�����̖��O���
namespace yuu
{
//UNICODE�Ƃ̌݊�
typedef std::basic_string<TCHAR> tstring;

//c.f. http://d.hatena.ne.jp/kryozahiro/20080809/1218295912
//���P�[���ˑ��B
//setlocale(LC_ALL, "japanese");
//���v���O�����̍ŏ��ɌĂяo���Ă�邱��
void StringToWString(std::wstring &dest, const std::string &src);
void WStringToString(std::string &dest, const std::wstring &src);
std::wstring StringToWString(const std::string &src);
std::string WStringToString(const std::wstring &src);

// COM�������[�X
inline void SafeRelease(IUnknown *p)
{
	if(p)
	{
		(p)->Release();
		p = NULL;
	}
}
}