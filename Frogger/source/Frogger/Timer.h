#pragma once

namespace Frogger
{
	class Timer
	{
	public :
		Timer();
		void Start();
		void Stop();
		size_t GetTimeInMilliseconds();

	private:
		size_t _startTimeInMilliseconds;
		size_t _endTimeInMilliseconds;
		bool _isActive;
	};
}
