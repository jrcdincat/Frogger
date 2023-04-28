#include "pch.h"
#include "Game.h"

namespace Frogger
{
	Game::Game() :
		_highScore{ 0 }, _isRunning{ false }, _playerTimerInSeconds{ 30 },
		_backgroundTexture{ nullptr }, _playerTexture{ nullptr }, 
		_white{ 255, 255, 255, 255 }, _isTitleScreen { true }, _isTitleScreenRendered { false },
		_deleteLifeRenderX{ 0 }, _waterUpYValue{ 421 }, _roadLowerYValue{ 360 },
		_playerMovementPixels{ 60 }, _mapYOffset{ 60 }, _movementScoreIncrease{ 10 }, _hasBeenInRow{ false },
		_windowWidth{ 840 }, _windowHeight{ 900 }, _numberOfFrames{ 0 }, _deltaTime{ 0 }
	{
		// Initialize video and audio
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			SDL_Log("Failed to initialize SDL Video: %s", SDL_GetError());
		}

		_window = SDL_CreateWindow("Frogger", 200, 200, _windowWidth, _windowHeight, false);
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
		
		if (IMG_Init(IMG_INIT_PNG) == 0)
		{
			SDL_Log("Failed to initialize SDL IMG: %s", SDL_GetError());
		}

		if (TTF_Init() == -1)
		{
			SDL_Log("Failed to initialize SDL TTF: %s", SDL_GetError());
		}

		SDL_Surface* icon = IMG_Load("assets/bug_goal.png");
		SDL_SetWindowIcon(_window, icon);
		SDL_FreeSurface(icon);

		LoadTextures();

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			SDL_Log("Failed to initialize Mixer: %s", Mix_GetError());
		}

		// Load music and sfx
		_gameMusic = Mix_LoadMUS("assets/music.mp3");
		if (_gameMusic == NULL)
		{
			SDL_Log("Failed to load music: %s", Mix_GetError());
		}
		Mix_VolumeMusic(55);
		PlayMusic();

		_hopSfx = Mix_LoadWAV("assets/hop_sfx.wav");
		if (_hopSfx == NULL)
		{
			SDL_Log("Failed to load sfx: %s", Mix_GetError());
		}
		Mix_VolumeChunk(_hopSfx,58);

		_squashSfx = Mix_LoadWAV("assets/squash_sfx.wav");
		if (_squashSfx == NULL)
		{
			SDL_Log("Failed to load sfx: %s", Mix_GetError());
		}
		Mix_VolumeChunk(_squashSfx, 58);


		// Set first row as having already been visted by player for scoring purposes
		_hasBeenInRow[12] = true;
		_hasBeenInRow[12] = true;

		// Set properties for logs
		int logCounter = 0;
		for (auto logIterator = _logs.begin(); logIterator < _logs.end(); ++logIterator)
		{
			if (logCounter < 3)
			{
				logIterator->SetTexture(*_longLogTexture);
				logIterator->SetPosition(0 + logCounter * 540, 240);
				logIterator->SetDimension(270, 60);
				logIterator->SetSpeed(18.0f);
			}
			else if (logCounter < 7)
			{
				logIterator->SetTexture(*_mediumLogTexture);
				logIterator->SetPosition(0 + (logCounter - 3) * 380, 120);
				logIterator->SetDimension(200, 60);
				logIterator->SetSpeed(12.0f);
			}
			else
			{
				logIterator->SetTexture(*_smallLogTexture);
				logIterator->SetPosition(0 + (logCounter - 7) * 370, 300);
				logIterator->SetDimension(150,60);
				logIterator->SetSpeed(11.0f);
			}

			logCounter++;
		}

		// Set properties for turtles
		int turtleCounter = 1;
		int initialPosition = 0;
		int pairGap = 230;
		int triGap = 180;
		for (auto turtleInterator = _turtles.begin(); turtleInterator < _turtles.end(); ++turtleInterator)
		{
			if (turtleCounter < 3)
			{
				turtleInterator->SetTexture(*_turtleTexture);
				turtleInterator->SetPosition(initialPosition + turtleCounter * 60, 180);
				turtleInterator->SetDimension(60, 60);
				turtleInterator->SetSpeed(15.0f);
				turtleInterator->SetAnimationProperties(9, 200, SDL_FLIP_HORIZONTAL, true);
			}
			else if (turtleCounter < 9)
			{
				if ((turtleCounter - 1) % 2 == 0)
				{
					initialPosition += pairGap;
				}
				turtleInterator->SetTexture(*_turtleTexture);
				turtleInterator->SetPosition(initialPosition + (turtleCounter) * 60, 180);
				turtleInterator->SetDimension(60, 60);
				turtleInterator->SetSpeed(15.0f);
				turtleInterator->SetAnimationProperties(2, 200, SDL_FLIP_HORIZONTAL, true);
			}
			else if (turtleCounter < 12)
			{
				initialPosition = 0;
				turtleInterator->SetTexture(*_turtleTexture);
				turtleInterator->SetPosition(initialPosition + (turtleCounter) * 60, 360);
				turtleInterator->SetDimension(60, 60);
				turtleInterator->SetSpeed(10.0f);
				turtleInterator->SetAnimationProperties(9, 200, SDL_FLIP_HORIZONTAL, true);
			}
			else
			{
				if (turtleCounter  % 3 == 0)
				{
					initialPosition += triGap;
				}
				turtleInterator->SetTexture(*_turtleTexture);
				turtleInterator->SetPosition(initialPosition + (turtleCounter) * 60, 360);
				turtleInterator->SetDimension(60, 60);
				turtleInterator->SetSpeed(10.0f);
				turtleInterator->SetAnimationProperties(2, 200, SDL_FLIP_HORIZONTAL, true);
			}

			turtleCounter++;
		}

		// Set properties for vehicles
		int vehicleCounter = 0;
		for (auto vehicleIterator = _vehicles.begin(); vehicleIterator < _vehicles.end(); ++vehicleIterator)
		{
			if (vehicleCounter < 2)
			{
				vehicleIterator->SetTexture(*_truckTexture, 0);
				vehicleIterator->SetPosition(0 + vehicleCounter * 350, 480);
				vehicleIterator->SetDimension(120, 60);
				vehicleIterator->SetMovementProperties(18.0f, Left);
			}
			else if (vehicleCounter < 3) {
				vehicleIterator->SetTexture(*_carsTexture, 60);
				vehicleIterator->SetPosition(0 + vehicleCounter * 200, 540);
				vehicleIterator->SetDimension(60, 60);
				vehicleIterator->SetMovementProperties(15.0f, Right);
			}
			else if (vehicleCounter < 6)
			{
				vehicleIterator->SetTexture(*_carsTexture, 0);
				vehicleIterator->SetPosition(0 + vehicleCounter * 240, 600);
				vehicleIterator->SetDimension(60, 60);
				vehicleIterator->SetMovementProperties(10.0f, Left);
			}
			else if (vehicleCounter < 9)
			{
				vehicleIterator->SetTexture(*_carsTexture, 180);
				vehicleIterator->SetPosition(0 + (vehicleCounter - 6) * 240, 660);
				vehicleIterator->SetDimension(60, 60);
				vehicleIterator->SetMovementProperties(12.0f, Right);
			}
			else
			{
				vehicleIterator->SetTexture(*_carsTexture, 120);
				vehicleIterator->SetPosition(0 + (vehicleCounter - 9) * 240, 720);
				vehicleIterator->SetDimension(60, 60);
				vehicleIterator->SetMovementProperties(9.0f, Left);
			}
			vehicleCounter++;
		}

		// Set animation properties for player
		_playerAnimations[0].SetAnimationProperties(3, 150, SDL_FLIP_NONE, false);
		_playerAnimations[1].SetAnimationProperties(3, 150, SDL_FLIP_VERTICAL, false);
		_playerAnimations[2].SetAnimationProperties(3, 150, SDL_FLIP_NONE, false);
		_playerAnimations[3].SetAnimationProperties(3, 150, SDL_FLIP_HORIZONTAL, false);

		// Render time text at bottom of screen
		_font = TTF_OpenFont("assets/Frankfurter/Frankfurter.ttf", 30);
		SDL_Surface* timeSurface = TTF_RenderText_Solid(_font, "Time", _white);
		SDL_Texture* timeTexture = SDL_CreateTextureFromSurface(_renderer, timeSurface);
		SDL_FreeSurface(timeSurface);
		SDL_Rect timeRect{ 710,840,60,60 };
		SDL_RenderCopy(_renderer, timeTexture, NULL, &timeRect);
		SDL_DestroyTexture(timeTexture);
	}
	Game::~Game()
	{
		TTF_CloseFont(_font);
		SDL_DestroyTexture(_lifeTexture);
		SDL_DestroyTexture(_titleTexture);
		SDL_DestroyTexture(_deathTexture);
		SDL_DestroyTexture(_truckTexture);
		SDL_DestroyTexture(_carsTexture);
		SDL_DestroyTexture(_goalTexture);
		SDL_DestroyTexture(_bugGoalTexture);
		SDL_DestroyTexture(_turtleTexture);
		SDL_DestroyTexture(_smallLogTexture);
		SDL_DestroyTexture(_mediumLogTexture);
		SDL_DestroyTexture(_longLogTexture);
		SDL_DestroyTexture(_leftPlayerTexture);
		SDL_DestroyTexture(_playerTexture);
		SDL_DestroyTexture(_backgroundTexture);
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_window);
		Mix_FreeMusic(_gameMusic);
		TTF_Quit();
		IMG_Quit();
		SDL_TLSCleanup();
		SDL_Quit();
	}

	void Game::Run()
	{
		_isRunning = true;
	}

	bool Game::IsRunning() const
	{
		return _isRunning;
	}

	void Game::Update()
	{
		// Win and Lose state
		if (_player.GetLives() == 0)
		{
			Restart();
		}
		else if (_goal.DidPlayerWin())
		{
			Restart();
		}

		_goal.Update();

		// Kill player when time runs out
		if (_goal.IsPlayerOutOfTime())
		{
			_goal.ResetOutOfTime();
			KillPlayer();
		}

		// Kill player when out of bounds
		if (_player.IsPlayerOutOfBounds())
		{
			_player.ResetOutOfBounds();
			KillPlayer();
		}

		// Update logs
		for (auto logIterator = _logs.begin(); logIterator < _logs.end(); ++logIterator)
		{
			logIterator->Update(_deltaTime);
		}

		// Update turtles
		for (auto turtleIterator = _turtles.begin(); turtleIterator < _turtles.end(); ++turtleIterator)
		{
			turtleIterator->Update(_deltaTime);

			if (turtleIterator->GetCurrentFrame() == 4 && 
				_player.GetPosition().y == turtleIterator->GetPosition().y  &&
				_player.GetPosition().x >= turtleIterator->GetPosition().x - 30 &&
				_player.GetPosition().x <= turtleIterator->GetPosition().x + turtleIterator->GetDimensions().w)
			{
				KillPlayer();
			}
		}

		// Update vehicles
		for (auto vehicleIterator = _vehicles.begin(); vehicleIterator < _vehicles.end(); ++vehicleIterator)
		{
			vehicleIterator->Update(_deltaTime);
			CheckForVehicleHit(vehicleIterator);	
		}

		// Update player and player animations
		_player.Update();
		for (auto playerAnimationIterator = _playerAnimations.begin(); playerAnimationIterator < _playerAnimations.end(); ++playerAnimationIterator)
		{
			playerAnimationIterator->Update();
		}
	}
	
	void Game::ProcessInput()
	{
		// Player Input
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			const Uint8* keyState = SDL_GetKeyboardState(NULL);
			switch (event.type)
			{
			case SDL_QUIT:
				_isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.repeat == 0)
				{
					if (_isTitleScreen)
					{
						// Reset game
						_isTitleScreen = false;
						_goal.ResetTimer();
						_fpsTimer.Start();
						_deltaTimer.Start();
						_hasBeenInRow.fill(false);
						_hasBeenInRow[12] = true;
						continue;
					}
					if (keyState[SDL_SCANCODE_UP])
					{
						// Reset playing up animation
						_playerAnimations[0].Play();

						int yPosition = _player.GetPosition().y;
						if (yPosition - _playerMovementPixels < _mapYOffset)
							return;

						PlayHopSfx();
						_player.MoveUp();
						CheckWaterRegion(yPosition, _waterUpYValue);

						// Check for score increase
						for (int i = 0; i < _hasBeenInRow.size(); ++i)
						{
							if (_player.GetPosition().y == (i + 1) * _playerMovementPixels)
							{
								if (_hasBeenInRow[i] == false)
								{
									_hasBeenInRow[i] = true;
									_player.AddToScore(_movementScoreIncrease);
									RenderScore();
								}
							}
						}
					}
					else if (keyState[SDL_SCANCODE_DOWN])
					{
						// Reset playing down animation
						_playerAnimations[1].Play();

						int yPosition = _player.GetPosition().y;
						if (yPosition + 60 >= _windowHeight - _mapYOffset)
							return;

						PlayHopSfx();
						_player.MoveDown();
						CheckWaterRegion(yPosition, _roadLowerYValue);
					}
					else if (keyState[SDL_SCANCODE_RIGHT])
					{
						// Reset playing right animation
						_playerAnimations[2].Play();
						PlayHopSfx();

						if (_player.GetPosition().x + _playerMovementPixels >= _windowWidth)
							return;
						_player.MoveRight();
					}
					else if (keyState[SDL_SCANCODE_LEFT])
					{
						// Reset playing left animation
						_playerAnimations[3].Play();
						PlayHopSfx();

						if (_player.GetPosition().x - _playerMovementPixels < 0)
							return;
						_player.MoveLeft();
					}
				}
			}
		}
	}
	
	void Game::Render()
	{
		// Background
		SDL_Rect backgroundSrcRect{ 0, 00, 840, 780 };
		SDL_Rect backgroundDstRect{0, 60, 840, 780 };
		SDL_RenderCopyEx(_renderer, _backgroundTexture, &backgroundSrcRect, &backgroundDstRect, 0, nullptr, SDL_FLIP_NONE);

		if (_isTitleScreen)
		{
			// Render title screen
			if (_isTitleScreenRendered == false)
			{
				RenderScore();
				RenderHighScore();

				SDL_Rect livesRect{ 20, 840, 60, 60 };
				SDL_RenderCopy(_renderer, _lifeTexture, NULL, &livesRect);
				livesRect.x = 80;
				SDL_RenderCopy(_renderer, _lifeTexture, NULL, &livesRect);
				livesRect.x = 140;
				SDL_RenderCopy(_renderer, _lifeTexture, NULL, &livesRect);

				SDL_Rect titleDstRect{0, 300, 840, 64};
				SDL_RenderCopy(_renderer,_titleTexture, NULL, &titleDstRect);

				SDL_Surface* messageSurface = TTF_RenderText_Solid(_font, "PRESS ANY KEY TO START", _white);
				SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(_renderer, messageSurface);
				SDL_FreeSurface(messageSurface);
				SDL_Rect messageRect{ 250, 500, 320, 60 };
				SDL_RenderCopy(_renderer, messageTexture, NULL, &messageRect);
				SDL_DestroyTexture(messageTexture);
				SDL_RenderPresent(_renderer);
			}
			return;
		}

		// Render logs
		for (auto logIterator = _logs.begin(); logIterator < _logs.end(); ++logIterator)
		{
			SDL_Rect logSrcRect{ 0, 0, logIterator->GetDimensions().w, logIterator->GetDimensions().h };
			SDL_Rect logDstRect{ logIterator->GetPosition().x, logIterator->GetPosition().y, logIterator->GetDimensions().w, logIterator->GetDimensions().h };
			SDL_RenderCopyEx(_renderer, logIterator->GetTexture() , &logSrcRect, &logDstRect, 0, nullptr, SDL_FLIP_NONE);
		}

		// Render turtles
		for (auto turtleIterator = _turtles.begin(); turtleIterator < _turtles.end(); ++turtleIterator)
		{
			turtleIterator->Render(*_renderer, *_turtleTexture);
		}

		// Render vehicles
		for (auto vehicleIterator = _vehicles.begin(); vehicleIterator < _vehicles.end(); ++vehicleIterator)
		{
			vehicleIterator->Render(*_renderer);
		}

		// Render goals
		_goal.Render(*_renderer, *_bugGoalTexture, *_goalTexture);

		// Render player animations
		switch (_player.GetAnimationState())
		{
		case AnimationState::UpAnimation:
			_playerAnimations[0].Render(_renderer, _playerTexture, _player.GetPosition(), _player.GetDimensions());
			break;
		case AnimationState::DownAnimation:
			_playerAnimations[1].Render(_renderer, _playerTexture, _player.GetPosition(), _player.GetDimensions());
			break;
		case AnimationState::LeftAnimation:
			_playerAnimations[2].Render(_renderer, _leftPlayerTexture, _player.GetPosition(), _player.GetDimensions());
			break;
		case AnimationState::RightAnimation:
			_playerAnimations[3].Render(_renderer, _leftPlayerTexture, _player.GetPosition(), _player.GetDimensions());
			break;
		}

		SDL_RenderPresent(_renderer);

		// Fps and delta time
		_numberOfFrames++;
		if (_fpsTimer.GetTimeInMilliseconds() / 1000.0f >= 1)
		{
			_fpsTimer.Start();
			_numberOfFrames = 0;
		}
		if (_deltaTimer.GetTimeInMilliseconds() < 16.7)
		{
			SDL_Delay(static_cast<Uint32>(16.7 - _deltaTimer.GetTimeInMilliseconds()));
		}

		_deltaTime = _deltaTimer.GetTimeInMilliseconds();
		_deltaTimer.Start();
	}
	
	void Game::LoadTextures()
	{
		SDL_Surface* backgroundSurface = IMG_Load("assets/background.png");
		if (backgroundSurface == nullptr)
			return;
		_backgroundTexture = SDL_CreateTextureFromSurface(_renderer, backgroundSurface);
		if (_backgroundTexture == nullptr)
			return;
		SDL_FreeSurface(backgroundSurface);

		SDL_Surface* playerSurface = IMG_Load("assets/frog_up.png");
		if (playerSurface == nullptr)
			return;
		_playerTexture = SDL_CreateTextureFromSurface(_renderer, playerSurface);
		SDL_FreeSurface(playerSurface);
		if (_playerTexture == nullptr)
			return;

		SDL_Surface* leftPlayerSurface = IMG_Load("assets/frog_left.png");
		if (leftPlayerSurface == nullptr)
			return;
		_leftPlayerTexture = SDL_CreateTextureFromSurface(_renderer, leftPlayerSurface);
		SDL_FreeSurface(leftPlayerSurface);
		if (_leftPlayerTexture == nullptr)
			return;

		SDL_Surface* longLogSurface = IMG_Load("assets/log3.png");
		if (longLogSurface == nullptr)
			return;
		_longLogTexture = SDL_CreateTextureFromSurface(_renderer, longLogSurface);
		SDL_FreeSurface(longLogSurface);
		if (_longLogTexture == nullptr)
			return;

		SDL_Surface* mediumLogSurface = IMG_Load("assets/log2.png");
		if (mediumLogSurface == nullptr)
			return;
		_mediumLogTexture = SDL_CreateTextureFromSurface(_renderer, mediumLogSurface);
		SDL_FreeSurface(mediumLogSurface);
		if (_mediumLogTexture == nullptr)
			return;

		SDL_Surface* smallLogSurface = IMG_Load("assets/log1.png");
		if (smallLogSurface == nullptr)
			return;
		_smallLogTexture = SDL_CreateTextureFromSurface(_renderer, smallLogSurface);
		SDL_FreeSurface(smallLogSurface);
		if (_smallLogTexture == nullptr)
			return;

		SDL_Surface* turtleSurface = IMG_Load("assets/turtle.png");
		if (turtleSurface == nullptr)
			return;
		_turtleTexture = SDL_CreateTextureFromSurface(_renderer, turtleSurface);
		SDL_FreeSurface(turtleSurface);
		if (_turtleTexture == nullptr)
			return;
	
		SDL_Surface* bugGoalSurface = IMG_Load("assets/bug_goal.png");
		if (bugGoalSurface == nullptr)
			return;
		_bugGoalTexture = SDL_CreateTextureFromSurface(_renderer, bugGoalSurface);
		SDL_FreeSurface(bugGoalSurface);
		if (_bugGoalTexture == nullptr)
			return;

		SDL_Surface* goalSurface = IMG_Load("assets/goal.png");
		if (goalSurface == nullptr)
			return;
		_goalTexture = SDL_CreateTextureFromSurface(_renderer, goalSurface);
		SDL_FreeSurface(goalSurface);
		if (_goalTexture == nullptr)
			return;

		SDL_Surface* carsSurface = IMG_Load("assets/cars.png");
		if (carsSurface == nullptr)
			return;
		_carsTexture = SDL_CreateTextureFromSurface(_renderer, carsSurface);
		SDL_FreeSurface(carsSurface);
		if (_carsTexture == nullptr)
			return;

		SDL_Surface* truckSurface = IMG_Load("assets/truck.png");
		if (truckSurface == nullptr)
			return;
		_truckTexture = SDL_CreateTextureFromSurface(_renderer, truckSurface);
		SDL_FreeSurface(truckSurface);
		if (_truckTexture == nullptr)
			return;

		SDL_Surface* deathSurface = IMG_Load("assets/death.png");
		if (deathSurface == nullptr)
			return;
		_deathTexture = SDL_CreateTextureFromSurface(_renderer, deathSurface);
		SDL_FreeSurface(deathSurface);
		if (_deathTexture == nullptr)
			return;

		SDL_Surface* titleSurface = IMG_Load("assets/froggerTitle.png");
		if (titleSurface == nullptr)
			return;
		_titleTexture = SDL_CreateTextureFromSurface(_renderer, titleSurface);
		SDL_FreeSurface(titleSurface);
		if (_titleTexture == nullptr)
			return;

		SDL_Surface* lifeSurface = IMG_Load("assets/life.png");
		if (lifeSurface == nullptr)
			return;
		_lifeTexture = SDL_CreateTextureFromSurface(_renderer, lifeSurface);
		SDL_FreeSurface(lifeSurface);
		if (_lifeTexture == nullptr)
			return;
	}
	
	void Game::CheckWaterRegion(int const yPosition, int const lowerYWaterRegion)
	{
		if (yPosition >= 0 && yPosition < lowerYWaterRegion)
		{
			bool attachedToObject = false;
			int playerX = _player.GetPosition().x;
			int playerY = _player.GetPosition().y;
			int playerWidth = _player.GetDimensions().w;
			bool isPlayerInGoal = _goal.IsPlayerInGoal(_player);
			// Check if player is in goal
			if (isPlayerInGoal)
			{
				_player.AddToScore(_goal.GetGoalScore());
				_hasBeenInRow.fill(false);
				_hasBeenInRow[12] = true;
				RenderScore();
				_player.Respawn();
				_goal.ResetTimer();
			}

			// Determine if player is attached to objects in water
			for (auto logIterator = _logs.begin(); logIterator < _logs.end(); ++logIterator)
			{
				int logX = logIterator->GetPosition().x;
				int logWidth = logIterator->GetDimensions().w;
				if (playerY == logIterator->GetPosition().y &&
					((playerX > logX && playerX < logX + logWidth) ||
						(playerX + playerWidth < logX + logWidth && playerX + playerWidth > logX)))
				{
					_player.AttachPosition(logIterator->GetPosition(), logIterator->GetDimensions(), playerX - logX);
					attachedToObject = true;
					break;
				}
			}

			for (auto turtleIterator = _turtles.begin(); turtleIterator < _turtles.end(); ++turtleIterator)
			{
				int turtleX = turtleIterator->GetPosition().x;
				int turtleWidth = turtleIterator->GetDimensions().w;
				if (playerY == turtleIterator->GetPosition().y && ((playerX > turtleX && playerX < turtleX + turtleWidth) ||
					(playerX + playerWidth < turtleX + turtleWidth && playerX + playerWidth > turtleX)))
				{
					_player.AttachPosition(turtleIterator->GetPosition(), turtleIterator->GetDimensions(), playerX - turtleX);
					attachedToObject = true;
					break;
				}
			}

			// Kill player if they are in the water
			if (!attachedToObject && !isPlayerInGoal)
			{
				KillPlayer();
			}
		}
	}

	void Game:: CheckForVehicleHit(std::array<Vehicle, 12Ui64>::iterator vehicleIterator)
	{
		int playerY = _player.GetPosition().y;
		if (playerY > _roadLowerYValue)
		{
			if (playerY != vehicleIterator->GetPosition().y)
			{
				return;
			}

			int playerX = _player.GetPosition().x;
			int vehicleX = vehicleIterator->GetPosition().x;
			if (playerX >= vehicleX && playerX <= vehicleX + vehicleIterator->GetDimensions().w)
			{
				KillPlayer();
			}
		}
	}

	void Game::RenderScore()
	{
		SDL_Rect clearScoreRect{ 60, 0, 200 ,60 };
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(_renderer, &clearScoreRect);

		_stringStream.str("");
		_stringStream << "Score:";
		_stringStream << _player.GetScore();
		SDL_Surface* messageSurface = TTF_RenderText_Solid(_font, _stringStream.str().c_str(), _white);
		SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(_renderer, messageSurface);
		SDL_FreeSurface(messageSurface);
		SDL_Rect messageRect{ 60,0,200,60 };
		SDL_RenderCopy(_renderer, messageTexture, NULL, &messageRect);
		SDL_DestroyTexture(messageTexture);
	}

	void Game::RenderHighScore()
	{
		SDL_Rect clearScoreRect{ 420, 0, 200 ,60 };
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(_renderer, &clearScoreRect);

		_stringStream.str("");
		_stringStream << "High Score:";
		_stringStream << _highScore;
		SDL_Surface* highScoreSurface = TTF_RenderText_Solid(_font, _stringStream.str().c_str(), _white);
		SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(_renderer, highScoreSurface);
		SDL_FreeSurface(highScoreSurface);
		SDL_Rect highScoreRect{ 420,0,200,60 };
		SDL_RenderCopy(_renderer, highScoreTexture, NULL, &highScoreRect);
	}

	void Game::KillPlayer()
	{
		PlaySquashSfx();
		_player.Die();

		SDL_Rect livesRect{ 140 - _deleteLifeRenderX, 840, 60, 60 };
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(_renderer, &livesRect);
		_deleteLifeRenderX += 60;
	}
	
	void Game::Restart()
	{
		int score = _player.GetScore();
		if (score > _highScore)
		{
			_highScore = score;
		}
		_isTitleScreen = true;
		_player.Reset();
		_goal.Reset();
		_deleteLifeRenderX = 0;
	}

	void Game::PlayMusic()
	{
		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(_gameMusic, -1);
		}
	}

	void Game::PlayHopSfx()
	{
		Mix_PlayChannel(-1, _hopSfx, 0);
	}

	void Game::PlaySquashSfx()
	{
		Mix_PlayChannel(-1, _squashSfx, 0);
	}
}
