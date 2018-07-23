#pragma once

namespace jmp
{
	class IMotionController
	{
	public:
		virtual ~IMotionController() = default; // make dtor virtual
		IMotionController(IMotionController&&) = default; // support moving
		IMotionController& operator=(IMotionController&&) = default;
		IMotionController(const IMotionController&) = default; // support copying
		IMotionController& operator=(const IMotionController&) = default;

		virtual UINT Initialize() = 0;

	protected:

	private:
	};
}
