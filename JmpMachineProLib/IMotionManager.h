/*
  Author : J.M.PARK
  Date : 2018.06.29
  Description : Motion Manager is abstract base class.
    This class maintains the IMotionItem interface class.  
*/

#pragma once

#include "Runtime.h"
#include "IMotionItem.h"

namespace jmp
{
	//using MotionItemContainer = std::vector<std::unique_ptr<IMotionItem>>;
	
	class JMPDLL_API IMotionManager
	{
	public:		
		IMotionManager() = default; // ctor
		virtual ~IMotionManager() = default; // make dtor virtual

		IMotionManager(const IMotionManager&) = delete; // support copying
		IMotionManager& operator=(const IMotionManager&) = delete;
		IMotionManager(IMotionManager&&) = delete; // support moving
		IMotionManager& operator=(IMotionManager&&) = delete;

		virtual void addItems(std::vector<std::unique_ptr<IMotionItem>>& items);
		virtual const IMotionItem& getItem(int index) const;

		virtual int init();		
		virtual void dispose();
		friend void executeLoop(void* obj);
		
		virtual int loadParameter() = 0;
		virtual void getStatus() = 0;
	
	protected:
		std::vector<std::unique_ptr<IMotionItem>> items_;
		std::thread loopThread_;
		bool terminated_ = false;
		bool mIsInitialized = false;

	private:

	};
}