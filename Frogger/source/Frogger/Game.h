#pragma once
#include <array>
#include <sstream>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "Player.h"
#include "Log.h"
#include "Timer.h"
#include "Animation.h"
#include "Turtle.h"
#include "Goal.h"
#include "Vehicle.h"

namespace Frogger
{
	class Game
	{
	public:
		Game();
		~Game();
		void Run();
		bool IsRunning() const;
		void ProcessInput();
		void Update();
		void Render();
		inline int GetHighScore() const { return _highScore; }

	private: 
		void LoadTextures();
		void CheckWaterRegion(int const yPosition, int const lowerYWaterRegion);
		void CheckForVehicleHit(std::array<Vehicle, 12Ui64>::iterator vehicleIterator);
		void RenderScore();
		void RenderHighScore();
		void KillPlayer();
		void Restart();
		void PlayMusic();
		void PlayHopSfx();
		void PlaySquashSfx();

		int _highScore;
		bool _isRunning;
		bool _isTitleScreen;
		bool _isTitleScreenRendered;
		float _playerTimerInSeconds;

		SDL_Window* _window;
		int const _windowWidth;
		int const _windowHeight;

		
		Timer _fpsTimer;
		Timer _deltaTimer;
		int _numberOfFrames;
		size_t _deltaTime;

		TTF_Font* _font;
		SDL_Color _white;
		std::stringstream _stringStream;

		SDL_Renderer* _renderer;
		SDL_Texture* _backgroundTexture;
		SDL_Texture* _playerTexture;
		SDL_Texture* _leftPlayerTexture;
		SDL_Texture* _longLogTexture;
		SDL_Texture* _mediumLogTexture;
		SDL_Texture* _smallLogTexture;
		SDL_Texture* _turtleTexture;
		SDL_Texture* _bugGoalTexture;
		SDL_Texture* _goalTexture;
		SDL_Texture* _carsTexture;
		SDL_Texture* _truckTexture;
		SDL_Texture* _deathTexture;
		SDL_Texture* _titleTexture;
		SDL_Texture* _lifeTexture;

		Mix_Music* _gameMusic;
		Mix_Chunk* _hopSfx;
		Mix_Chunk* _squashSfx;

		std::array<Animation, 4> _playerAnimations;
		Animation _turtleAnimation;

		Player _player;
		Goal _goal;
		std::array<bool, 13> _hasBeenInRow;

		std::array<Log, 11> _logs;
		std::array<Turtle, 20> _turtles;
		std::array<Vehicle, 12> _vehicles;

		int _deleteLifeRenderX;
		int _waterUpYValue;
		int _roadLowerYValue;
		int _playerMovementPixels;
		int _mapYOffset;
		int _movementScoreIncrease;
	};
}
