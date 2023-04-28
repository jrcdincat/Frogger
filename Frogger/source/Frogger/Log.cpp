#include "pch.h"
#include "Log.h"

namespace Frogger
{
	Log::Log() : _texture { nullptr }, _speed { 15.0f }, _pixelOffscreenReset { 1300 }
	{
	}

	void Log::Update(size_t& deltaTime)
	{
		MoveRight(deltaTime);
	}

	Coordinates2D& Log::GetPosition() 
	{
		return _position;
	}

	Dimensions& Log::GetDimensions() 
	{
		return _dimensions;
	}

	SDL_Texture* Log::GetTexture()
	{
		return _texture;
	}

	void Log::SetPosition(int x, int y)
	{
		_position.x = x;
		_position.y = y;
	}
	
	void Log::SetDimension(int w, int h)
	{
		_dimensions.w = w;
		_dimensions.h = h;
	}

	void Log::SetTexture(SDL_Texture& texture)
	{
		_texture = &texture;
	}

	void Log::SetSpeed(float speed)
	{
		_speed = speed;
	}

	void Log::MoveRight(size_t& deltaTime)
	{
		if (_position.x >= _pixelOffscreenReset)
		{
			_position.x = 0 - _dimensions.w;
		}

		_position.x += static_cast<int>(deltaTime * _speed) / 100;
	}
}