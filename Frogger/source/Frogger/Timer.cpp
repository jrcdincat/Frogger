#include "pch.h"
#include "Timer.h"

namespace Frogger
{
	Timer::Timer(): _startTimeInMilliseconds{ 0 }, _endTimeInMilliseconds{ 0 }, _isActive { false }
	{

	}

	void Timer::Start()
	{
		_isActive = true;
		_startTimeInMilliseconds = SDL_GetTicks();
	}

	void Timer::Stop()
	{
		_isActive = false;
	}

	size_t Timer::GetTimeInMilliseconds()
	{
		return SDL_GetTicks() - _startTimeInMilliseconds;
	}
}