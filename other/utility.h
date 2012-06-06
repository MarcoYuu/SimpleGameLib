#pragma once

#include <string>
#include <tchar.h>
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

// 自分のライブラリの名前空間
namespace yuu
{
//UNICODEとの互換
typedef std::basic_string<TCHAR> tstring;

//c.f. http://d.hatena.ne.jp/kryozahiro/20080809/1218295912
//ロケール依存。
//setlocale(LC_ALL, "japanese");
//をプログラムの最初に呼び出してやること
void StringToWString(std::wstring &dest, const std::string &src);
void WStringToString(std::string &dest, const std::wstring &src);
std::wstring StringToWString(const std::string &src);
std::string WStringToString(const std::wstring &src);

// COMをリリース
inline void SafeRelease(IUnknown *p)
{
	if(p)
	{
		(p)->Release();
		p = NULL;
	}
}
}