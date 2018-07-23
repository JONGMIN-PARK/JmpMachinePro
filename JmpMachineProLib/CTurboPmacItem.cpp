#include "stdafx.h"
#include "CTurboPmacItem.h"

namespace jmp
{
	CTurboPmacItem::CTurboPmacItem() 
		: IMotionItem()
	{
	}

	CTurboPmacItem::~CTurboPmacItem()
	{
	}

	int CTurboPmacItem::getCommandCount() const
	{
		return commandCount_;
	}

	int CTurboPmacItem::getEncoderCount() const
	{
		return encoderCount_;
	}

	double CTurboPmacItem::getCommandPosition() const
	{
		return commandPosition_;
	}

	double CTurboPmacItem::getEncoderPosition() const
	{
		return encoderPosition_;
	}

	bool CTurboPmacItem::checkMovingStop()
	{
		return true;
	}

	bool CTurboPmacItem::checkMoveDone()
	{
		return true;
	}
	
	bool CTurboPmacItem::checkMoving()
	{
		return busy_;
	}
	
	void CTurboPmacItem::clearCount()
	{

	}
	
	void CTurboPmacItem::clearAlarm()
	{

	}
	
	void CTurboPmacItem::enable()
	{

	}
	
	void CTurboPmacItem::disable()
	{

	}
	
	void CTurboPmacItem::stop()
	{

	}
	
	void CTurboPmacItem::estop()
	{

	}
	
	void CTurboPmacItem::moveJog(double speedValue)
	{

	}

	void CTurboPmacItem::setSpeed(double velocity, double acceleration, double deceleration)
	{

	}

	void CTurboPmacItem::setSpeedLevel(double percentage)
	{

	}

	void CTurboPmacItem::moveRelative(double position)
	{

	}

	void CTurboPmacItem::moveAbsolute(double position)
	{

	}
}