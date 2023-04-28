#pragma once
#include <array>
#include <vector>
#include "Coordinates2D.h"
#include "Timer.h"
#include "Player.h"
#include "Home.h"

namespace Frogger
{
	class Goal
	{
	public:
		Goal();
		void Update();
		void Render(SDL_Renderer& renderer,SDL_Texture& texture, SDL_Texture& goalTexture);
		bool IsPlayerInGoal(Player& player);
		int GetGoalScore() const;
		void ResetTimer();
		bool DidPlayerWin() const;
		void Reset();
		bool IsPlayerOutOfTime() const;
		void ResetOutOfTime();

	private:
		int GetRandomPositionIndex();
		void RenderTimer(SDL_Renderer& renderer);

		std::array<Home, 5> _homes;
		std::vector<Home*> _bugHomes;
		int _currentPosition;
		Timer _spawnTimer;
		size_t _timeInSeconds;
		bool _isActive;
		bool _didPlayerWin;
		bool _isOutOfTime;
	};
}

