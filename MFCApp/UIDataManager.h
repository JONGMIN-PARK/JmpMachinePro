#pragma once
#include "tinyxml2.h"

namespace jmp
{
//	using ID_MAP_LIST = std::map<std::string, unsigned int>;

	typedef struct _ST_ID_MAP_LIST
	{
		CWnd* item;
		std::string name;
		unsigned int id;
		double value;
	} *P_ST_ID_MAP_LIST, ST_ID_MAP_LIST;

	class CUIDataManager
	{
	public:
		CUIDataManager();
		virtual ~CUIDataManager();

		double& operator[] (const std::string& idName);

		int getIdByName(const std::string& name);	// Get name of the control.
		void add(CWnd* control, const std::string& idName, unsigned int id);
		void save(const std::string& fileName);
		void load(const std::string& fileName);
		void clear(const std::string& fileName);
		void copy(const std::string& sourceFileName, const std::string& destinationFileName);
		double getValue(const std::string& idName) const;

	protected:

	private:
		std::vector<ST_ID_MAP_LIST> mItems;

		/*std::vector<CWnd*> items;
		std::map<std::string, unsigned int> mIdMap;*/
		//std::map<CWnd*, ID_MAP_LIST> mIdMap;
	};
}