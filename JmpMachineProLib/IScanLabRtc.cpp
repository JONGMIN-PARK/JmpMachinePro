#include "stdafx.h"
#include "IScanLabRtc.h"

#include "CScanLabRtc5.h"
#include "CScanLabRtc6.h"

namespace jmp
{
	IScanLabRtc* CreateRtc3()
	{
		return nullptr;
	}

	IScanLabRtc* CreateRtc4()
	{
		return nullptr;
	}

	IScanLabRtc* CreateRtc5()
	{
		return new CScanLabRtc5();
	}

	IScanLabRtc* CreateRtc6()
	{
		return new CScanLabRtc6();
	}

	void DestroyRtc(IScanLabRtc** ppRtc)
	{
		delete *ppRtc;
		*ppRtc = nullptr;
	}
}