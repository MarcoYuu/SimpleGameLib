#include <cstdlib>
#include <string>

// Ž©•ª‚Ìƒ‰ƒCƒuƒ‰ƒŠ‚Ì–¼‘O‹óŠÔ
namespace yuu
{
	void StringToWString(std::wstring &dest, const std::string &src)
	{
		std::wstring out(src.size() + 1, '0');
		mbstowcs_s(0, &out[0], out.size() + 1, src.c_str(), src.size() + 1);
		dest = out.c_str();
	}
	void WStringToString(std::string &dest, const std::wstring &src)
	{
		std::string out(src.size()*sizeof(wchar_t) + 1, '0');
		wcstombs_s(0, &out[0], out.size() + 1, src.c_str(), src.size()*sizeof(wchar_t) + 1);
		dest = out.c_str();
	}

	std::wstring StringToWString(const std::string &src)
	{
		std::wstring out(src.size() + 1, '0');
		mbstowcs_s(0, &out[0], out.size() + 1, src.c_str(), src.size() + 1);
		return out = out.c_str();
	}
	std::string WStringToString(const std::wstring &src)
	{
		std::string out(src.size()*sizeof(wchar_t) + 1, '0');
		wcstombs_s(0, &out[0], out.size() + 1, src.c_str(), src.size()*sizeof(wchar_t) + 1);
		return out = out.c_str();
	}
}