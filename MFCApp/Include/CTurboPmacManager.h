#pragma once
#include "IMotionManager.h"

namespace jmp
{
	typedef union _PMAC_DPRTYPE_ {
		float fData;
		DWORD dwData;
	} PMAC_DPRTYPE, *PPMAC_DPRTYPE;

	class JMPDLL_API CTurboPmacManager : public IMotionManager
	{
	public:
		CTurboPmacManager();
		virtual ~CTurboPmacManager();

		virtual int init(std::map<int, std::vector<int>>& channelMap);
		virtual void dispose() override;

		virtual int loadParameter() override;
		virtual void getStatus() override;

		// Rotary Buffer
		bool loadProgramFile(const std::string& fileName);
		bool initRotaryBuffer();
		bool startRotaryBuffer();
		bool stopRotaryBuffer();
		bool abortRotaryBuffer();
		friend void executeRotaryBufferLoop(void* obj);

	protected:

	private:
		std::map<int, std::vector<int>> mChannelMap;
		std::vector<int> mDeviceNumbers;
		static unsigned int kDprOffset;
		PMAC_DPRTYPE mDprData[10];

		// Rotary Buffer
		FILE* mInFile = nullptr;
		fpos_t mInFilePos;
		int mLineCount = 0;
		const int mRotateSize = 100;
		std::thread mRotaryBufferLoopThread;
		bool terminated_ = false;
		int mSequenceNumber = -1;
		int mLineDownloadCount = 0;
		bool mIsDprError = false;
	};
}
