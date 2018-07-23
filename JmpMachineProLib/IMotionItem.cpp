#include "stdafx.h"
#include "IMotionItem.h"

namespace jmp
{
	IMotionItem::IMotionItem()
	{		
	}

	void IMotionItem::setMoveUnitType(MoveUnitType moveUnitType)
	{
		moveUnitType_ = moveUnitType;
	}

	MoveUnitType IMotionItem::getMoveUnitType() const
	{
		return moveUnitType_;
	}
}