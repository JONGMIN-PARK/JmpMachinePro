#include "stdafx.h"
#include "CScanLabRtc5.h"

#include "RTC5impl.h"

namespace jmp
{
	CScanLabRtc5::CScanLabRtc5()
	{

	}

	CScanLabRtc5::~CScanLabRtc5()
	{
		free_rtc5_dll();
	}

	UINT CScanLabRtc5::initialize()
	{		
		UINT errorCode;
		errorCode = init_rtc5_dll();
		
		stop_execution();
		errorCode = load_program_file(nullptr);
		if (errorCode)
		{
			free_rtc5_dll();
			return 0;
		}

		reset_error(-1);
		config_list(4000, 4000);

		return 1;
	}

	UINT CScanLabRtc5::selectCardNumber(UINT cardNumber)
	{		
		return select_rtc(cardNumber);
	}

	UINT CScanLabRtc5::loadCorrectionFile(const TCHAR* name, UINT num, UINT dim)
	{		
		return load_correction_file(CT2A(name), num, dim);
	}

	UINT CScanLabRtc5::loadProgramFile(const TCHAR* name /*= nullptr*/)
	{
		return load_program_file(CT2A(name));
	}

	void CScanLabRtc5::configList(UINT mem1, UINT mem2)
	{
		config_list(mem1, mem2);
	}

	void CScanLabRtc5::setLaserMode(UINT mode)
	{
		set_laser_mode(mode);
	}

	void CScanLabRtc5::setFirstPulseKiller(UINT length)
	{
		set_firstpulse_killer(length);
	}

	void CScanLabRtc5::setLaserControl(UINT control)
	{
		set_laser_control(control);
	}

	void CScanLabRtc5::setStartList(UINT listNumber)
	{
		set_start_list(listNumber);
	}

	void CScanLabRtc5::setStartList(UINT cardNumber, UINT listNumber)
	{
		n_set_start_list(cardNumber, listNumber);
	}

	void CScanLabRtc5::delay(UINT delay)
	{
		long_delay(delay);
	}

	void CScanLabRtc5::setLaserPulses(UINT halfPeriod, UINT pulseLength)
	{
		set_laser_pulses(halfPeriod, pulseLength);
	}

	void CScanLabRtc5::setScannerDelays(UINT jump, UINT mark, UINT polygon)
	{
		set_scanner_delays(jump, mark, polygon);
	}

	void CScanLabRtc5::setLaserDelays(long laserOnDelay, UINT laserOffDelay)
	{
		set_laser_delays(laserOnDelay, laserOffDelay);
	}

	void CScanLabRtc5::setJumpSpeed(double speed)
	{
		set_jump_speed(speed);
	}

	void CScanLabRtc5::setMarkSpeed(double speed)
	{
		set_mark_speed(speed);
	}

	void CScanLabRtc5::setEndOfList()
	{
		set_end_of_list();
	}

	void CScanLabRtc5::setEndOfList(UINT cardNumber)
	{
		n_set_end_of_list(cardNumber);
	}

	void CScanLabRtc5::executeList(UINT listNumber)
	{
		execute_list(listNumber);
	}

	void CScanLabRtc5::executeList(UINT cardNumber, UINT listNumber)
	{
		n_execute_list(cardNumber, listNumber);
	}

	void CScanLabRtc5::enableLaser(UINT cardNumber)
	{
		n_enable_laser(cardNumber);
	}

	void CScanLabRtc5::disableLaser(UINT cardNumber)
	{
		n_disable_laser(cardNumber);
	}

	void CScanLabRtc5::stopExecution(UINT cardNumber)
	{
		n_stop_execution(cardNumber);
	}

	void CScanLabRtc5::jumpAbs(UINT cardNumber, long x, long y)
	{
		n_jump_abs(cardNumber, x, y);
	}

	void CScanLabRtc5::markAbs(UINT cardNumber, long x, long y)
	{
		n_mark_abs(cardNumber, x, y);
	}

	void CScanLabRtc5::arcAbs(UINT cardNumber, long x, long y, double angle)
	{
		n_arc_abs(cardNumber, x, y, angle);
	}

	void CScanLabRtc5::setWobbel(UINT transversal, UINT longitudinal, double frequency)
	{
		set_wobbel(transversal, longitudinal, frequency);
	}

	void CScanLabRtc5::getStatus(UINT boardNumber, UINT* status, UINT* position)
	{
		n_get_status(boardNumber, status, position);
	}

}