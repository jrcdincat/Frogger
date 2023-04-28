#include "pch.h"
#include "Vehicle.h"

namespace Frogger
{
	Vehicle::Vehicle() : _texture { nullptr }, _movementDirection { Left }, 
		_flip { SDL_FLIP_NONE } , _pixelOffscreenRightReset { 1000 }, 
		_pixelOffscreenLeftReset { -200 }, _speed { 15.0f }, _textureOffset { 0 }
	{
	}

	void Vehicle::Update(size_t& deltaTime)
	{
		Move(deltaTime);
	}

	Coordinates2D& Vehicle::GetPosition()
	{
		return _position;
	}

	Dimensions& Vehicle::GetDimensions()
	{
		return _dimensions;
	}

	void Vehicle::SetPosition(int x, int y)
	{
		_position.x = x;
		_position.y = y;
	}

	void Vehicle::SetDimension(int w, int h)
	{
		_dimensions.w = w;
		_dimensions.h = h;
	}

	void Vehicle::SetTexture(SDL_Texture& texture, int textureOffset)
	{
		_texture = &texture;
		_textureOffset = textureOffset;
	}

	void Vehicle::SetMovementProperties(float speed, enum Direction direction)
	{
		_speed = speed;
		_movementDirection = direction;

		if (direction == Left)
		{
			_flip = SDL_FLIP_HORIZONTAL;
		}
	}

	void Vehicle::Render(SDL_Renderer& renderer)
	{
		SDL_Rect srcRect{ _textureOffset, 0, _dimensions.w, _dimensions.h };
		SDL_Rect dstRect{ _position.x, _position.y, _dimensions.w, _dimensions.h };
		SDL_RenderCopyEx(&renderer, _texture, &srcRect, &dstRect, 0, nullptr, _flip);
	}

	void Vehicle::Move(size_t& deltaTime)
	{
		if (_movementDirection == Right)
		{
			if (_position.x >= _pixelOffscreenRightReset) 
			{
				_position.x = 0 - _dimensions.w;
			}

			_position.x += static_cast<int>(deltaTime * _speed) / 100;
		}
		else
		{
			if (_position.x <= _pixelOffscreenLeftReset) 
			{
				_position.x = 840 + _dimensions.w;
			}

			_position.x -= static_cast<int>(deltaTime * _speed) / 100;
		}
	}
}