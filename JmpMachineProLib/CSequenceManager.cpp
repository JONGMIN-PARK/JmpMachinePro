#include "stdafx.h"
#include "CSequenceManager.h"

namespace jmp
{
	CSequence& CSequenceManager::operator[](int index)
	{
		if (sequences_.size() <= 0 ||
			index >= sequences_.size())
		{
			throw std::invalid_argument("Index is over range.");
		}

		return sequences_[index];
	}

	CSequence CSequenceManager::operator[](int index) const
	{
		if (sequences_.size() <= 0 ||
			index >= sequences_.size())
		{
			throw std::invalid_argument("Index is over range.");
		}

		return sequences_[index];
	}

	void CSequenceManager::addSequence(const CSequence& sequence)
	{
		sequences_.push_back(sequence);
	}
}