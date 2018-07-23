#pragma once
#include "IScanLabRtc.h"

namespace jmp
{
	class JMPDLL_API CScanLabRtc4 : public IScanLabRtc
	{
	public:
		CScanLabRtc4();
		virtual ~CScanLabRtc4();

		virtual UINT initialize() override;
		virtual UINT selectCardNumber(UINT cardNumber) override;
		virtual UINT loadCorrectionFile(const TCHAR* name, UINT num, UINT dim) override;
		virtual UINT loadProgramFile(const TCHAR* name = nullptr) override;
		virtual void configList(UINT mem1, UINT mem2) override;
		virtual void setLaserMode(UINT mode) override;
		virtual void setFirstPulseKiller(UINT length) override;
		virtual void setLaserControl(UINT control) override;
		virtual void setStartList(UINT listNumber) override;
		virtual void setStartList(UINT cardNumber, UINT listNumber) override;
		virtual void delay(UINT delay) override;
		virtual void setLaserPulses(UINT halfPeriod, UINT pulseLength) override;
		virtual void setScannerDelays(UINT jump, UINT mark, UINT polygon) override;
		virtual void setLaserDelays(long laserOnDelay, UINT laserOffDelay) override;
		virtual void setJumpSpeed(double speed) override;
		virtual void setMarkSpeed(double speed) override;
		virtual void setEndOfList() override;
		virtual void setEndOfList(UINT cardNumber) override;
		virtual void executeList(UINT listNumber) override;
		virtual void executeList(UINT cardNumber, UINT listNumber) override;
		virtual void enableLaser(UINT cardNumber) override;
		virtual void disableLaser(UINT cardNumber) override;
		virtual void stopExecution(UINT cardNumber) override;
		virtual void jumpAbs(UINT cardNumber, long x, long y) override;
		virtual void markAbs(UINT cardNumber, long x, long y) override;
		virtual void arcAbs(UINT cardNumber, long x, long y, double angle) override;
		virtual void setWobbel(UINT transversal, UINT longitudinal, double frequency) override;
		virtual void getStatus(UINT boardNumber, UINT* status, UINT* position) override;
	};
}
