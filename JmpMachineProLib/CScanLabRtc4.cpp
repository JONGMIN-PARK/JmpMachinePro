#include "stdafx.h"
#include "CScanLabRtc4.h"

#include "RTC4impl.h"

namespace jmp
{
	CScanLabRtc4::CScanLabRtc4()
	{

	}

	CScanLabRtc4::~CScanLabRtc4()
	{
		
	}

	UINT CScanLabRtc4::initialize()
	{
		UINT errorCode = 0;
		stop_execution();

		errorCode = load_correction_file("cor_1to1.ctb",
			1,          // table; #1 is used by default
			1.0, 1.0,   // scale factor
			0.0,        // rotation in degrees, countervclockwise
			0.0, 0.0);  // offset in bits
		if (errorCode)
		{
			return 0;
		}

		errorCode = load_program_file("RTC4D2.hex");
		if (errorCode)
		{
			return 0;
		}

		return 1;
	}

	UINT CScanLabRtc4::selectCardNumber(UINT cardNumber)
	{
		select_rtc(static_cast<unsigned short>(cardNumber));

		return 1;
	}

	UINT CScanLabRtc4::loadCorrectionFile(const TCHAR* name, UINT num, UINT dim)
	{
		short errorCode;
		errorCode = load_correction_file(
			CT2A(name),
			1,			// table; #1 is used by default
			1.0, 1.0,	// scale factor
			0.0,		// rotation in degrees, countervclockwise
			0.0, 0.0);	// offset in bits

		return errorCode;
	}

	UINT CScanLabRtc4::loadProgramFile(const TCHAR* name /*= nullptr*/)
	{
		short ret = load_program_file(CT2A(name));
		return ret;		
	}

	void CScanLabRtc4::configList(UINT mem1, UINT mem2)
	{
		
	}

	void CScanLabRtc4::setLaserMode(UINT mode)
	{
		set_laser_mode(mode);
	}

	void CScanLabRtc4::setFirstPulseKiller(UINT length)
	{
		set_firstpulse_killer(length);
	}

	void CScanLabRtc4::setLaserControl(UINT control)
	{
		
	}

	void CScanLabRtc4::setStartList(UINT listNumber)
	{
		set_start_list(listNumber);
	}

	void CScanLabRtc4::setStartList(UINT cardNumber, UINT listNumber)
	{
		n_set_start_list(cardNumber, listNumber);
	}
	
	void CScanLabRtc4::delay(UINT delay)
	{
		long_delay(delay);
	}

	void CScanLabRtc4::setLaserPulses(UINT halfPeriod, UINT pulseLength)
	{
		set_laser_timing(halfPeriod, pulseLength, pulseLength, 1);
	}

	void CScanLabRtc4::setScannerDelays(UINT jump, UINT mark, UINT polygon)
	{
		set_scanner_delays(jump, mark, polygon);
	}

	void CScanLabRtc4::setLaserDelays(long laserOnDelay, UINT laserOffDelay)
	{
		set_laser_delays(laserOnDelay, laserOffDelay);
	}

	void CScanLabRtc4::setJumpSpeed(double speed)
	{
		set_jump_speed(speed);
	}

	void CScanLabRtc4::setMarkSpeed(double speed)
	{
		set_mark_speed(speed);
	}

	void CScanLabRtc4::setEndOfList()
	{
		set_end_of_list();
	}

	void CScanLabRtc4::setEndOfList(UINT cardNumber)
	{
		n_set_end_of_list(cardNumber);
	}

	void CScanLabRtc4::executeList(UINT listNumber)
	{
		executeList(listNumber);
	}

	void CScanLabRtc4::executeList(UINT cardNumber, UINT listNumber)
	{
		n_execute_list(cardNumber, listNumber);
	}

	void CScanLabRtc4::enableLaser(UINT cardNumber)
	{
		n_enable_laser(cardNumber);
	}

	void CScanLabRtc4::disableLaser(UINT cardNumber)
	{
		n_disable_laser(cardNumber);
	}

	void CScanLabRtc4::stopExecution(UINT cardNumber)
	{
		n_stop_execution(cardNumber);
	}

	void CScanLabRtc4::jumpAbs(UINT cardNumber, long x, long y)
	{
		n_jump_abs(cardNumber, x, y);
	}

	void CScanLabRtc4::markAbs(UINT cardNumber, long x, long y)
	{
		n_mark_abs(cardNumber, x, y);
	}

	void CScanLabRtc4::arcAbs(UINT cardNumber, long x, long y, double angle)
	{
		n_arc_abs(cardNumber, x, y, angle);
	}

	void CScanLabRtc4::setWobbel(UINT transversal, UINT longitudinal, double frequency)
	{
		set_wobbel(transversal, frequency);
	}

	void CScanLabRtc4::getStatus(UINT boardNumber, UINT* status, UINT* position)
	{
		USHORT statusResult = 0;
		USHORT positionResult = 0;
		n_get_status(boardNumber, &statusResult, &positionResult);
		*status = statusResult;
		*position = positionResult;
	}
}


