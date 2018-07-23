#pragma once
#include "IMotionItem.h"

namespace jmp
{
	class JMPDLL_API CTurboPmacItem : public IMotionItem
	{
	public:
		CTurboPmacItem();
		virtual ~CTurboPmacItem();

		virtual int getCommandCount() const override;
		virtual int getEncoderCount() const = 0;
		virtual double getCommandPosition() const = 0;
		virtual double getEncoderPosition() const = 0;
		virtual bool checkMovingStop() = 0;
		virtual bool checkMoveDone() = 0;
		virtual bool checkMoving() = 0;
		virtual void clearCount() = 0;
		virtual void clearAlarm() = 0;
		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual void stop() = 0;
		virtual void estop() = 0;
		CheckMethodDelegate checkMethod;
		virtual void moveJog(double speedValue) = 0;
		virtual void setSpeed(double velocity, double acceleration, double deceleration) = 0;
		virtual void setSpeedLevel(double percentage) = 0;
		virtual void moveRelative(double position) = 0;
		virtual void moveAbsolute(double position) = 0;
	};
}