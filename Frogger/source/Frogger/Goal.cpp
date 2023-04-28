#include "pch.h"
#include "Goal.h"

namespace Frogger
{
	Goal::Goal() : _currentPosition { 0 }, _timeInSeconds { 0 }, _isActive { false },
		_didPlayerWin { false }, _isOutOfTime { false }
	{
		srand(static_cast<unsigned int>(time(0)));
		
		// Set bug goal positions
		_homes[0]._positions.x = 26;
		_homes[0]._positions.y = 60;
		_homes[1]._positions.x = 201;
		_homes[1]._positions.y = 60;
		_homes[2]._positions.x = 381;
		_homes[2]._positions.y = 60;
		_homes[3]._positions.x = 561;
		_homes[3]._positions.y = 60;
		_homes[4]._positions.x = 741;
		_homes[4]._positions.y = 60;
		_bugHomes.reserve(5);
		_bugHomes.emplace_back(&_homes[0]);
		_bugHomes.emplace_back(&_homes[1]);
		_bugHomes.emplace_back(&_homes[2]);
		_bugHomes.emplace_back(&_homes[3]);
		_bugHomes.emplace_back(&_homes[4]);
	}

	void Goal::Update()
	{
		// Return if player won
		if (_bugHomes.size() == 0)
		{
			return;
		}

		if (_spawnTimer.GetTimeInMilliseconds() == 0)
		{
			_spawnTimer.Start();
		}

		_timeInSeconds = _spawnTimer.GetTimeInMilliseconds() / 1000;

		// Activate goal bug at specified time intervals in seconds
		if (_timeInSeconds > 2 && _timeInSeconds < 7)
		{			
			if (_isActive == false)
			{
				_currentPosition = GetRandomPositionIndex();
				_isActive = true;
			}
		}
		else if (_timeInSeconds > 10 && _timeInSeconds < 15)
		{
			if (_isActive == false)
			{
				_currentPosition = GetRandomPositionIndex();
				_isActive = true;
			}
		}
		else if (_timeInSeconds > 18 && _timeInSeconds < 23)
		{
			if (_isActive == false)
			{
				_currentPosition = GetRandomPositionIndex();
				_isActive = true;
			}
		}
		else if (_timeInSeconds > 26 && _timeInSeconds < 31)
		{
			if (_isActive == false)
			{
				_currentPosition = GetRandomPositionIndex();
				_isActive = true;
			}
		}
		else if (_timeInSeconds > 30)
		{
			_isOutOfTime = true;
			_spawnTimer.Start();
			_isActive = false;
		}
		else
		{
			_isActive = false;
		}
	}

	void Goal::Render(SDL_Renderer& renderer, SDL_Texture& bugTexture, SDL_Texture& goalTexture)
	{
		// Render bug at current random home position
		if (_isActive && _currentPosition < _bugHomes.size())
		{
			SDL_Rect srcRect{ 0, 0, 60, 60 };
			SDL_Rect dstRect{ _bugHomes[_currentPosition]->_positions.x, _bugHomes[_currentPosition]->_positions.y, 60, 60 };
			SDL_RenderCopyEx(&renderer, &bugTexture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
		}

		// Render frog at all goals that player has successfully reached
		for (auto it = _homes.begin(); it < _homes.end(); ++it)
		{
			if (it->_successfulGoal)
			{
				SDL_Rect goalSrcRect{ 0, 0, 60, 60 };
				SDL_Rect goalDstRect{ it->_positions.x, it->_positions.y, 60, 60 };
				SDL_RenderCopyEx(&renderer, &goalTexture, &goalSrcRect, &goalDstRect, 0, nullptr, SDL_FLIP_NONE);
			}
		}

		RenderTimer(renderer);
	}

	bool Goal::IsPlayerInGoal(Player& player)
	{
		// Player is not in goal
		if (_currentPosition >= _bugHomes.size())
		{
			return false;
		}

		if (player._position.y != _bugHomes[_currentPosition]->_positions.y ||
			player._position.x < (_bugHomes[_currentPosition]->_positions.x - 20) ||
			player._position.x > (_bugHomes[_currentPosition]->_positions.x + 70)
			)
		{
			return false;
		}

		// Player is in goal
		std::vector<Home*>::iterator homeIterator = _bugHomes.begin() + _currentPosition;
		_bugHomes[_currentPosition]->_successfulGoal = true;
		_bugHomes.erase(homeIterator);

		if (_bugHomes.size() == 0)
		{
			_didPlayerWin = true;
		}

		return true;
	}

	int Goal::GetGoalScore() const
	{
		// Goal = 50 points + 10 x half of the seconds remaing
		return ((30 - static_cast<int>(_timeInSeconds)) / 2) * 10 + 50;
	}

	void Goal::ResetTimer()
	{
		_spawnTimer.Start();
		_timeInSeconds = 0;
	}

	bool Goal::DidPlayerWin() const
	{
		return _didPlayerWin;
	}
	
	void Goal::Reset()
	{
		_bugHomes.emplace_back(&_homes[0]);
		_bugHomes.emplace_back(&_homes[1]);
		_bugHomes.emplace_back(&_homes[2]);
		_bugHomes.emplace_back(&_homes[3]);
		_bugHomes.emplace_back(&_homes[4]);

		for (auto it = _homes.begin(); it < _homes.end(); ++it)
		{
			it->_successfulGoal = false;
		}
		_didPlayerWin = false;
	}
	
	bool Goal::IsPlayerOutOfTime() const
	{
		return _isOutOfTime;
	}
	
	void Goal::ResetOutOfTime()
	{
		_isOutOfTime = false;
	}
	
	int Goal::GetRandomPositionIndex()
	{
		if (_bugHomes.size() == 1)
			return 0;
		return rand() % _bugHomes.size();
	}

	void Goal::RenderTimer(SDL_Renderer& renderer)
	{
		// Render red timer bar
		SDL_Rect timerRegionRect{ 400, 860, 300, 30 };
		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(&renderer, &timerRegionRect);

		// Render black rectangle over section of time that has passed
		int timeDifference = 0 + static_cast<int>(_timeInSeconds) * 10;
		SDL_Rect timerRect { 400, 860, timeDifference ,30};
		SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(&renderer, &timerRect);
	}
}