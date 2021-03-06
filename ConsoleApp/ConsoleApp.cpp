// ConsoleApp.cpp : Defines the entry point for the console application.
//

/*
 * Author : J.M.PARK
 * Date : 2018.06.20
 * Feature : Sequence Manager Version 1.
 */

#include "stdafx.h"
#include <iostream>
#include <array>
#include <vector>

using namespace std;

enum SequenceIndex
{
	Loader,
	Main, 
	Unloader,
};

class SequenceItem
{
public:

};

class Sequence
{
public:
	Sequence()
	{
	}

	void setName(const std::string& name)
	{
		name_ = name;
	}

	const std::string& getName() const
	{
		return name_;
	}

private:
	std::string name_;
};

class SequenceManager
{
public:
	/*SequenceManager()
	{
		sequences_.push_back(Sequence());
	}*/
	
	static SequenceManager& getInstance()
	{
		static SequenceManager instance;
		return instance;
	}

	Sequence& operator[](int index)
	{
		if (index < 0 ||
			index >= sequences_.size())
		{
			throw std::out_of_range("Index out of range");
		}

		return sequences_[index];
	}
	
	const Sequence& operator[](int index) const
	{
		if (index < 0 ||
			index >= sequences_.size())
		{
			throw std::out_of_range("Index out of range");
		}

		return sequences_[index];
	}

	void addSequence(const Sequence& sequence)
	{
		sequences_.push_back(sequence);
	}

private:
	SequenceManager() = default;
	~SequenceManager() = default;

	vector<Sequence> sequences_;
};

int main()
{	
	try
	{	
		Sequence* seq = new Sequence();
		seq->setName("JMPARK");

		Sequence* seq1 = new Sequence();
		seq1->setName("SMLEE");

		SequenceManager::getInstance().addSequence(*seq);
		SequenceManager::getInstance().addSequence(*seq1);		

		std::cout << SequenceManager::getInstance()[0].getName().c_str() << endl;
		std::cout << SequenceManager::getInstance()[1].getName().c_str() << endl;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << endl;
	}

    return 0;
}

