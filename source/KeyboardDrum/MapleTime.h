#pragma once
#include "TypeDef.h"

namespace MapleDrum
{
	class MapleTime
	{
	public:
		MapleTime();
		~MapleTime();
		double Update();
		void Reset();
	private:
		LONGLONG lastClock;
		double frequency;
		double GetFrequency() const;
		LONGLONG GetClock() const;


	};
}


