#pragma once

#include <string>

#if defined(_WIN32) | defined(_WIN64)
#include <tchar.h>
#endif

// �����̃��C�u�����̖��O���
namespace yuu
{

	//UNICODE�Ƃ̌݊�
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
	// �|�C���^�L���X�g
	//------------------------------------------------------------------
	// �C�ӂ̌^�̃|�C���^�ւ̃L���X�g
	template <class T>
	inline T *pointer_cast(void *p)
	{
		return static_cast<T *>(p);
	}

	//------------------------------------------------------------------
	// string�ϊ�
	//------------------------------------------------------------------
	// c.f. http://d.hatena.ne.jp/kryozahiro/20080809/1218295912
	// ���P�[���ˑ��Bsetlocale(LC_ALL, "japanese"); ���v���O�����̍ŏ��ɌĂяo���Ă�邱��
	void StringToWString(std::wstring &dest, const std::string &src);
	void WStringToString(std::string &dest, const std::wstring &src);

	std::wstring StringToWString(const std::string &src);
	std::string WStringToString(const std::wstring &src);

}