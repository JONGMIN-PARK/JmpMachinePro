#include "stdafx.h"
#include "UIDataManager.h"

namespace jmp
{
	CUIDataManager::CUIDataManager()
	{

	}


	CUIDataManager::~CUIDataManager()
	{

	}

	double& CUIDataManager::operator[] (const std::string& idName)
	{
		double value = 0.0;

		for (const auto& item : mItems)
		{
			if (item.name == idName)
			{
				value = item.value;
				break;
			}
		}		

		return value;
	}

	int CUIDataManager::getIdByName(const std::string& name)
	{
		int id = -1;
		for (const auto& item : mItems)
		{
			if (item.name == name)
			{
				id = item.id;
				break;
			}
		}

		return id;
	}

	void CUIDataManager::add(CWnd* control, const std::string& idName, unsigned int id)
	{		
		mItems.push_back({ control, idName, id });
	}

	void CUIDataManager::save(const std::string& fileName)
	{
		tinyxml2::XMLDocument doc;

		for (int n = 0; n < mItems.size(); n++)
		{
			if (mItems[n].item->IsKindOf(RUNTIME_CLASS(CThemeEdit)) != 0)
			{
				CThemeEdit* edit = dynamic_cast<CThemeEdit*>(mItems[n].item);

				edit->setId2String(mItems[n].name);
				edit->Save();
			}
			else if (mItems[n].item->IsKindOf(RUNTIME_CLASS(CThemeEdit)) != 0)
			{

			}
		}
	}

	void CUIDataManager::load(const std::string& fileName)
	{
		for (int n = 0; n < mItems.size(); n++)
		{
			if (mItems[n].item->IsKindOf(RUNTIME_CLASS(CThemeEdit)) != 0)
			{
				CThemeEdit* edit = dynamic_cast<CThemeEdit*>(mItems[n].item);

				edit->setId2String(mItems[n].name);
				edit->Load();
				mItems[n].value = edit->GetDouble();
			}
			else if (mItems[n].item->IsKindOf(RUNTIME_CLASS(CThemeEdit)) != 0)
			{

			}
		}
	}

	void CUIDataManager::clear(const std::string& fileName)
	{

	}

	void CUIDataManager::copy(const std::string& sourceFileName, const std::string& destinationFileName)
	{

	}

	double CUIDataManager::getValue(const std::string& idName) const
	{
		double value = 0.0;
		for (const auto& item : mItems)
		{
			if (item.name == idName)
			{
				value = item.value;
				break;
			}
		}

		return value;
	}
}