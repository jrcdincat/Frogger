#pragma once
#include "GameObject.h"

namespace Frogger
{
	enum AnimationState
	{
		UpAnimation, 
		DownAnimation,
		LeftAnimation,
		RightAnimation
	};

	class Player : public GameObject
	{
	public: 
		Player();
		int GetScore() const;
		int GetLives() const;
		void Die();
		void AddToScore(int const addValue);
		void Update();
		void MoveLeft();
		void MoveRight();
		void MoveUp();
		void MoveDown();
		void AttachPosition(Coordinates2D& objectPosition, Dimensions& objectDimension, int xOffset);
		Coordinates2D& GetPosition();
		Dimensions& GetDimensions();
		void Reset();
		void Respawn();
		AnimationState GetAnimationState() const;
		bool IsPlayerOutOfBounds() const;
		void ResetOutOfBounds();

	private: 
		int _score;
		int _lives;
		int _xPositionOffset;
		int _xSpawnPosition;
		int _ySpawnPosition;
		int _playerMovementPixels;
		bool _isAttachedToObject;
		bool _isPlayerOutOfBounds;
		Coordinates2D* _objectPositionPlayerIsAttached;
		Dimensions* _objectDimensionPlayerIsAttached;
		AnimationState _currentAnimation;
	};
}
