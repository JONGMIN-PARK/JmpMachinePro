#include "stdafx.h"
#include "CScanLabRtc6.h"

#include "RTC6impl.h"

namespace jmp
{
	CScanLabRtc6::CScanLabRtc6()
	{

	}

	CScanLabRtc6::~CScanLabRtc6()
	{
		free_rtc6_dll();
	}

	UINT CScanLabRtc6::initialize()
	{
		UINT errorCode;
		errorCode = init_rtc6_dll();
		set_rtc5_mode();
		stop_execution();
		errorCode = load_program_file(nullptr);
		if (errorCode)
		{
			free_rtc6_dll();
			return 0;
		}

		reset_error(-1);
		config_list(4000, 4000);

		return 1;
	}

	UINT CScanLabRtc6::selectCardNumber(UINT cardNumber)
	{
		return select_rtc(cardNumber);
	}

	UINT CScanLabRtc6::loadCorrectionFile(const TCHAR* name, UINT num, UINT dim)
	{
		return load_correction_file(CT2A(name), num, dim);
	}

	UINT CScanLabRtc6::loadProgramFile(const TCHAR* name /*= nullptr*/)
	{
		return load_program_file(CT2A(name));
	}

	void CScanLabRtc6::configList(UINT mem1, UINT mem2)
	{
		config_list(mem1, mem2);
	}

	void CScanLabRtc6::setLaserMode(UINT mode)
	{
		set_laser_mode(mode);
	}

	void CScanLabRtc6::setFirstPulseKiller(UINT length)
	{
		set_firstpulse_killer(length);
	}

	void CScanLabRtc6::setLaserControl(UINT control)
	{
		set_laser_control(control);
	}

	void CScanLabRtc6::setStartList(UINT listNumber)
	{
		set_start_list(listNumber);
	}

	void CScanLabRtc6::setStartList(UINT cardNumber, UINT listNumber)
	{
		n_set_start_list(cardNumber, listNumber);
	}

	void CScanLabRtc6::delay(UINT delay)
	{
		long_delay(delay);
	}

	void CScanLabRtc6::setLaserPulses(UINT halfPeriod, UINT pulseLength)
	{
		set_laser_pulses(halfPeriod, pulseLength);
	}

	void CScanLabRtc6::setScannerDelays(UINT jump, UINT mark, UINT polygon)
	{
		set_scanner_delays(jump, mark, polygon);
	}

	void CScanLabRtc6::setLaserDelays(long laserOnDelay, UINT laserOffDelay)
	{
		set_laser_delays(laserOnDelay, laserOffDelay);
	}

	void CScanLabRtc6::setJumpSpeed(double speed)
	{
		set_jump_speed(speed);
	}

	void CScanLabRtc6::setMarkSpeed(double speed)
	{
		set_mark_speed(speed);
	}

	void CScanLabRtc6::setEndOfList()
	{
		set_end_of_list();
	}

	void CScanLabRtc6::setEndOfList(UINT cardNumber)
	{
		n_set_end_of_list(cardNumber);
	}

	void CScanLabRtc6::executeList(UINT listNumber)
	{
		execute_list(listNumber);
	}

	void CScanLabRtc6::executeList(UINT cardNumber, UINT listNumber)
	{
		n_execute_list(cardNumber, listNumber);
	}

	void CScanLabRtc6::enableLaser(UINT cardNumber)
	{
		n_enable_laser(cardNumber);
	}

	void CScanLabRtc6::disableLaser(UINT cardNumber)
	{
		n_disable_laser(cardNumber);
	}

	void CScanLabRtc6::stopExecution(UINT cardNumber)
	{
		n_stop_execution(cardNumber);
	}

	void CScanLabRtc6::jumpAbs(UINT cardNumber, long x, long y)
	{
		n_jump_abs(cardNumber, x, y);
	}

	void CScanLabRtc6::markAbs(UINT cardNumber, long x, long y)
	{
		n_mark_abs(cardNumber, x, y);
	}

	void CScanLabRtc6::arcAbs(UINT cardNumber, long x, long y, double angle)
	{
		n_arc_abs(cardNumber, x, y, angle);
	}

	void CScanLabRtc6::setWobbel(UINT transversal, UINT longitudinal, double frequency)
	{
		set_wobbel(transversal, longitudinal, frequency);
	}

	void CScanLabRtc6::getStatus(UINT boardNumber, UINT* status, UINT* position)
	{
		n_get_status(boardNumber, status, position);
	}
}