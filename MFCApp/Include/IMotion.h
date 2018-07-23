#pragma once
#include <cstdio>
#include <thread>

namespace jmp
{
	using CheckMethodDelegate = bool(*)(int index); //typedef void(*CheckMethodDelegate)();
	
	class IMotion
	{
	public:
		IMotion() = default;
		virtual ~IMotion() = default;
		IMotion(const IMotion& rhs) = default;
		IMotion& operator=(const IMotion& rhs) = default;
		IMotion(IMotion&& rhs) = default;
		IMotion& operator=(IMotion&& rhs) = default;

		static int initialize();
		static int dispose();
		virtual double getCommandPosition() = 0;
		virtual double getEncoderPosition() = 0;
		virtual bool checkMovingStop() = 0;
		virtual bool checkMoveDone() = 0;
		virtual bool checkMoving() = 0;
		virtual void clearCount() = 0;
		virtual void clearAlarm() = 0;
		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual void stop() = 0;
		virtual void stopEmergency() = 0;
		static CheckMethodDelegate checkMethod;		
		virtual void moveJog(double speedValue) = 0;
		virtual void setSpeed(double velocity, double acceleration, double deceleration) = 0;
		virtual void setSpeedLevel(double percentage) = 0;
		virtual void moveRelative(double position) = 0;
		virtual void moveAbsolute(double position) = 0;
		static void moveLinear(
			int coordinateNumber, 
			int axesCounts, 
			int* axesNumber, 
			double* position, 
			double* velocity, 
			double* acceleration, 
			double* deceleration);
		static void moveCircleAngle(
			int coordinateNumber, 
			int axesCounts, 
			int* axesNumber, 
			double* center, 
			double angle, 
			bool clockwiseDirection, 
			double* velocity, 
			double* acceleration, 
			double* deceleration);
		static void moveCircleEndPoint(
			int coordinateNumber, 
			int axesCounts, 
			int* axesNumber,
			double* center,
			double* end,
			bool clockwiseDirection,
			double* velocity,
			double* acceleration,
			double* deceleration);

	protected:
		static void executeLoop();
		static std::thread loopThread_;

	private:
	};
}
