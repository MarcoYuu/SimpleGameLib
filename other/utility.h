#pragma once

#include <string>

#if defined(_WIN32) | defined(_WIN64)
#include <tchar.h>
#endif

// 自分のライブラリの名前空間
namespace yuu
{

	//UNICODEとの互換
#if defined(_WIN32) | defined(_WIN64)
	typedef std::basic_string<TCHAR> tstring;
#else

#ifdef UNICODE
	typedef std::wstring tstring 
#elif
#define std::string tstring 
#endif

#endif

	//------------------------------------------------------------------
	// ポインタキャスト
	//------------------------------------------------------------------
	// 任意の型のポインタへのキャスト
	template <class T>
	inline T *pointer_cast(void *p)
	{
		return static_cast<T *>(p);
	}

	//------------------------------------------------------------------
	// string変換
	//------------------------------------------------------------------
	// c.f. http://d.hatena.ne.jp/kryozahiro/20080809/1218295912
	// ロケール依存。setlocale(LC_ALL, "japanese"); をプログラムの最初に呼び出してやること
	void StringToWString(std::wstring &dest, const std::string &src);
	void WStringToString(std::string &dest, const std::wstring &src);

	std::wstring StringToWString(const std::string &src);
	std::string WStringToString(const std::wstring &src);

}