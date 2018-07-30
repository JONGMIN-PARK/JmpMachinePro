#pragma once
#include "tinyxml2.h"

namespace jmp
{
	typedef struct tagST_ID_MAP_LIST
	{
		CWnd* item; // Control
		std::string name; // id (이름)
		unsigned int id; // id (숫자)
		double value; // 값
	} *P_ST_ID_MAP_LIST, ST_ID_MAP_LIST;

	class CUIDataManager
	{
	public:
		CUIDataManager();
		virtual ~CUIDataManager();
		CUIDataManager(const CUIDataManager& rhs) = delete;
		CUIDataManager& operator=(const CUIDataManager& rhs) = delete;
		CUIDataManager(CUIDataManager&& rhs) = delete;
		CUIDataManager& operator=(CUIDataManager&& rhs) = delete;

		double& operator[] (const std::string& idName);

		int getIdByName(const std::string& name);												// Get the name of the ui control.
		void add(CWnd* control, const std::string& idName, unsigned int id);					// add the ui control to the lists.
		void save(const std::string& fileName);													// save the data.
		void load(const std::string& fileName);													// load the data.
		void clear(const std::string& fileName);												// clear the data.
		void copy(const std::string& sourceFileName, const std::string& destinationFileName);	// copy from the source file to the destination file.
		double getValue(const std::string& idName) const;

	protected:

	private:
		std::vector<ST_ID_MAP_LIST> mItems;

		/*std::vector<CWnd*> items;
		std::map<std::string, unsigned int> mIdMap;*/
		//std::map<CWnd*, ID_MAP_LIST> mIdMap;
	};
}