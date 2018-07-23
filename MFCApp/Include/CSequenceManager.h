#pragma 
#include <iostream>
#include <vector>
namespace jmp
{
	class CSequence
	{

	};

	class CSequenceManager
	{
	public:
		static CSequenceManager& getInstance()
		{
			static CSequenceManager instance;
			return instance;
		}

		CSequence& operator[](int index);
		CSequence operator[](int index) const;

		void addSequence(const CSequence& sequence);

	protected:

	private:
		CSequenceManager() = default;
		virtual ~CSequenceManager() = default;
		CSequenceManager(const CSequenceManager& rhs) = delete;
		CSequenceManager& operator=(const CSequenceManager& rhs) = delete;

		std::vector<CSequence> sequences_;
	};
}
