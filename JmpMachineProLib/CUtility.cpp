#include "stdafx.h"
#include "CUtility.h"

namespace jmp
{
	std::wstring CUtility::GetWStrFromMultiByteStr(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}

	std::string CUtility::GetMultiByteStrFromWStr(const std::wstring& wstr)
	{
		int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
		return strTo;
	}
}