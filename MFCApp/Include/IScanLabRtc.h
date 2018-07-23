#pragma once

namespace jmp
{
	class IScanLabRtc
	{	
	public:
		IScanLabRtc() = default;
		virtual ~IScanLabRtc() = default;
		
		IScanLabRtc(const IScanLabRtc& rhs) = delete;
		IScanLabRtc& operator=(const IScanLabRtc& rhs) = delete;
		IScanLabRtc(IScanLabRtc&& rhs) = delete;
		IScanLabRtc& operator=(IScanLabRtc&& rhs) = delete;

		virtual UINT initialize() = 0;
		virtual UINT selectCardNumber(UINT cardNumber) = 0;
		virtual UINT loadCorrectionFile(const TCHAR* name, UINT num, UINT dim) = 0;
		virtual UINT loadProgramFile(const TCHAR* name = nullptr) = 0;
		virtual void configList(UINT mem1, UINT mem2) = 0;
		virtual void setLaserMode(UINT mode) = 0;
		virtual void setFirstPulseKiller(UINT length) = 0;
		virtual void setLaserControl(UINT control) = 0;
		virtual void setStartList(UINT listNumber) = 0;
		virtual void setStartList(UINT cardNumber, UINT listNumber) = 0;
		virtual void delay(UINT delay) = 0;
		virtual void setLaserPulses(UINT halfPeriod, UINT pulseLength) = 0;
		virtual void setScannerDelays(UINT jump, UINT mark, UINT polygon) = 0;
		virtual void setLaserDelays(long laserOnDelay, UINT laserOffDelay) = 0;
		virtual void setJumpSpeed(double speed) = 0;
		virtual void setMarkSpeed(double speed) = 0;
		virtual void setEndOfList() = 0;
		virtual void setEndOfList(UINT cardNumber) = 0;
		virtual void executeList(UINT listNumber) = 0;
		virtual void executeList(UINT cardNumber, UINT listNumber) = 0;
		virtual void enableLaser(UINT cardNumber) = 0;
		virtual void disableLaser(UINT cardNumber) = 0;
		virtual void stopExecution(UINT cardNumber) = 0;
		virtual void jumpAbs(UINT cardNumber, long x, long y) = 0;
		virtual void markAbs(UINT cardNumber, long x, long y) = 0;
		virtual void arcAbs(UINT cardNumber, long x, int long, double angle) = 0;
		virtual void setWobbel(UINT transversal, UINT longitudinal, double frequency) = 0;
		virtual void getStatus(UINT boardNumber, UINT* status, UINT* position) = 0;
	};

	JMPDLL_API IScanLabRtc* CreateRtc3();
	JMPDLL_API IScanLabRtc* CreateRtc4();
	JMPDLL_API IScanLabRtc* CreateRtc5();
	JMPDLL_API IScanLabRtc* CreateRtc6();
	JMPDLL_API void DestroyRtc(IScanLabRtc** ppRtc);
}
