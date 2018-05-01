#include "Common.h"
#include "MapleTime.h"

namespace MapleDrum
{
	MapleTime::MapleTime()
	{
		frequency = GetFrequency();
		lastClock = GetClock();
	}

	double MapleTime::Update()
	{
		double elapsedTime;
		LONGLONG currentClock = GetClock();
		elapsedTime = (currentClock - lastClock) / frequency;
		lastClock = currentClock;
		return elapsedTime;
	}
	void MapleTime::Reset()
	{
		lastClock = GetClock();
	}
	LONGLONG MapleTime::GetClock() const
	{
		LARGE_INTEGER result;
		QueryPerformanceCounter(&result);
		return result.QuadPart;
	}
	double MapleTime::GetFrequency() const
	{
		LARGE_INTEGER frequency;

		if (QueryPerformanceFrequency(&frequency) == false)
		{
			throw std::exception("QueryPerformanceFrequency() failed.");
		}

		return (double)frequency.QuadPart;
	}
	MapleTime::~MapleTime()
	{
	}
}

