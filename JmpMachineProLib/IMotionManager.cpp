#include "stdafx.h"
#include "IMotionManager.h"
#include "IMotionItem.h"

namespace jmp
{	
	void IMotionManager::addItems(std::vector<std::unique_ptr<IMotionItem>>& items)
	{
		items_ = std::move(items);
	}

	const IMotionItem& IMotionManager::getItem(int index) const
	{
		if (index < 0 ||
			index >= items_.size())
		{
			throw std::out_of_range("This parameter's value is out of range.");
		}

		return *items_[index];
	}

	void executeLoop(void* obj)
	{
		IMotionManager* motionManager = static_cast<IMotionManager*>(obj);

		while (!motionManager->terminated_)
		{
			if (motionManager->mIsInitialized)
			{
				motionManager->getStatus();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}		
	}

	int IMotionManager::init()
	{					
		loadParameter();

		loopThread_ = std::thread(&executeLoop, this); //std::unique_ptr<std::thread>(new std::thread(&IMotionManager::executeLoop));
		return 0;
	}	

	void IMotionManager::dispose()
	{		
		terminated_ = true;
		loopThread_.join();
	}
}