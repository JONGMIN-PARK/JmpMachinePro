#pragma once
#include <cstdio>
#include <thread>
#include <typeinfo>

namespace jmp
{
	using CheckMethodDelegate = bool(*)(int index); //typedef void(*CheckMethodDelegate)();
	
	enum class MoveUnitType
	{
		MilliMeter = 0,
		MicroMeter,
		Degree,
	};

	enum class HomeExecutionMode
	{
		Plc = 0,
		HostPc,
	};

	class JMPDLL_API IMotionItem
	{
	public:
		IMotionItem();
		virtual ~IMotionItem() = default;

		IMotionItem(const IMotionItem& rhs) = default;
		IMotionItem& operator=(const IMotionItem& rhs) = default;
		IMotionItem(IMotionItem&& rhs) = default;
		IMotionItem& operator=(IMotionItem&& rhs) = default;

		virtual int getCommandCount() const = 0;
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
		virtual void setMoveUnitType(MoveUnitType moveUnitType);
		virtual MoveUnitType getMoveUnitType() const;

		// interpolation
		/*virtual void moveLinear(
			int coordinateNumber, 
			int axesCounts, 
			int* axesNumber, 
			double* position, 
			double* velocity, 
			double* acceleration, 
			double* deceleration) = 0;
		
		virtual void moveCircleAngle(
			int coordinateNumber, 
			int axesCounts, 
			int* axesNumber, 
			double* center, 
			double angle, 
			bool clockwiseDirection, 
			double* velocity, 
			double* acceleration, 
			double* deceleration) = 0;
		
		virtual void moveCircleEndPoint(
			int coordinateNumber, 
			int axesCounts, 
			int* axesNumber,
			double* center,
			double* end,
			bool clockwiseDirection,
			double* velocity,
			double* acceleration,
			double* deceleration) = 0;*/

	protected:		
		int index_;
		std::string name_;
		int deviceChannelNumber_;				// 여러개 접속해야 할 경우 사용함.
		double encoderCounterPerMilliMeter_;	// counts / 1 mm		
		MoveUnitType moveUnitType_;				// mm, um, degree
		int commandCount_;						// Command count
		int encoderCount_;						// Encoder count
		int commandPosition_;					// Command position
		int encoderPosition_;					// Encoder position

		HomeExecutionMode homeExecutionMode_;	// Plc, HostPc
		int homeStartPlcProgramNumber_;			// Home start plc program number
		int homeProgramProgressNumber_;			// Home program procress number
		int homeStep_;

		bool followingError_;
		bool busy_;
		bool enabled_;
		bool ampFault_;
		bool homeSensorDetected_;
		bool cwLimitSensorDetected_;
		bool ccwLimitSensorDetected_;
		bool internalHomeDone_;
		bool homeDone_;
		bool encoderNotUsing_;

	private:
	};
}
