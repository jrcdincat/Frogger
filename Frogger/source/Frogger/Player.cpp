#include "pch.h"
#include "Player.h"

namespace Frogger
{
	Player::Player() : _score{ 0 }, _lives{ 3 }, _objectPositionPlayerIsAttached{ nullptr }, 
		_objectDimensionPlayerIsAttached{nullptr}, _currentAnimation { UpAnimation },
		_xPositionOffset { 0 }, _isAttachedToObject { false }, _xSpawnPosition { 360 },
		_ySpawnPosition { 780 }, _playerMovementPixels { 60 }, _isPlayerOutOfBounds { false }
	{
		_position.x = 360;
		_position.y = 780;
		_dimensions.w = 60;
		_dimensions.h = 60;
	}

	GameObject::~GameObject()
	{
	}
	
	int Player::GetScore() const
	{
		return _score;
	}

	int Player::GetLives() const
	{
		return _lives;
	}
	
	void Player::Die()
	{
		_lives--;
		_isAttachedToObject = false;
		_position.x = _xSpawnPosition;
		_position.y = _ySpawnPosition;
	}
	
	void Player::AddToScore(int const addValue)
	{
		_score += addValue;
	}
	
	void Player::Update()
	{
		if (_isAttachedToObject)
		{
			// Kill player if they float off screen
			int newXPosition = _objectPositionPlayerIsAttached->x + _xPositionOffset;
			if (newXPosition + _dimensions.w >= 900 || 
				newXPosition <= -60 || 
				newXPosition + _dimensions.w <= _objectPositionPlayerIsAttached->x ||
				newXPosition >= _objectPositionPlayerIsAttached->x + _objectDimensionPlayerIsAttached->w)
			{
				_isAttachedToObject = false;
				_isPlayerOutOfBounds = true;
			}
			else
			{
				_position.x = newXPosition;
			}
		}
	}

	void Player::MoveLeft()
	{
		_currentAnimation = LeftAnimation;
		if (_isAttachedToObject)
		{
			_xPositionOffset -= _playerMovementPixels;
		}
		_position.x -= _playerMovementPixels;
	}

	void Player::MoveRight()
	{
		_currentAnimation = RightAnimation;
		if (_isAttachedToObject)
		{
			_xPositionOffset += _playerMovementPixels;
		}
		_position.x += _playerMovementPixels;
	}
	
	void Player::MoveUp()
	{
		_currentAnimation = UpAnimation;
		_position.y -= _playerMovementPixels;
		if (_isAttachedToObject)
		{
			_isAttachedToObject = false;
		}
	}

	void Player::MoveDown()
	{
		_currentAnimation = DownAnimation;
		_position.y += _playerMovementPixels;
		if (_isAttachedToObject)
		{
			_isAttachedToObject = false;
		}
	}

	void Player::AttachPosition(Coordinates2D& objectPosition, Dimensions& objectDimension, int xOffset)
	{
		_objectPositionPlayerIsAttached = &objectPosition;
		_objectDimensionPlayerIsAttached = &objectDimension;
		_xPositionOffset = xOffset;
		_isAttachedToObject = true;
	}

	Coordinates2D& Player::GetPosition()
	{
		return _position;
	}
	
	Dimensions& Player::GetDimensions()
	{
		return _dimensions;
	}

	void Player::Reset()
	{
		_lives = 3;
		_score = 0;
		_isAttachedToObject = false;
	}

	void Player::Respawn()
	{
		_position.x = _xSpawnPosition;
		_position.y = _ySpawnPosition;
	}

	AnimationState Player::GetAnimationState() const
	{
		return _currentAnimation;
	}

	bool Player::IsPlayerOutOfBounds() const
	{
		return _isPlayerOutOfBounds;
	}

	void Player::ResetOutOfBounds()
	{
		_isPlayerOutOfBounds = false;
	}
}
