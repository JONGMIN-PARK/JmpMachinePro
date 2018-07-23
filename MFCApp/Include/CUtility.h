#pragma once

namespace jmp
{
	class CUtility
	{
	public:
		static CUtility& getInstance()
		{
			static CUtility instance;
			return instance;
		}

		std::wstring GetWStrFromMultiByteStr(const std::string& str);
		std::string GetMultiByteStrFromWStr(const std::wstring& wstr);

	protected:

	private:
		CUtility() = default;
		virtual ~CUtility() = default;
		CUtility(const CUtility& rhs) = delete;
		CUtility& operator=(const CUtility& rhs) = delete;
	};
}
