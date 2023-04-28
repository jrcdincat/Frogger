#include "pch.h"
#include "Turtle.h"

namespace Frogger
{
	Turtle::Turtle() : _texture{ nullptr }, _speed{ 15.0f }, _isSinkingTurtle{false}, _pixelOffscreenReset{-500}
	{
	}

	void Turtle::Update(size_t& deltaTime)
	{
		MoveLeft(deltaTime);
		_animation.Update();
	}
	Coordinates2D& Turtle::GetPosition()
	{
		return _position;
	}

	Dimensions& Turtle::GetDimensions()
	{
		return _dimensions;
	}

	SDL_Texture* Turtle::GetTexture()
	{
		return _texture;
	}

	void Turtle::SetPosition(int x, int y)
	{
		_position.x = x;
		_position.y = y;
	}

	void Turtle::SetDimension(int w, int h)
	{
		_dimensions.w = w;
		_dimensions.h = h;
	}

	void Turtle::SetTexture(SDL_Texture& texture)
	{
		_texture = &texture;
	}

	void Turtle::SetSpeed(float speed)
	{
		_speed = speed;
	}

	void Turtle::SetAnimationProperties(int numberOfFrames, int animationSpeed, SDL_RendererFlip flip, bool loop)
	{
		_animation.SetAnimationProperties(numberOfFrames, animationSpeed, flip, loop);

		// Sinking turtle has more than 2 frames
		if (numberOfFrames > 2)
		{
			_isSinkingTurtle = true;
		}
	}

	void Turtle::Render(SDL_Renderer& renderer, SDL_Texture& texture)
	{
		_animation.Render(&renderer, &texture, _position, _dimensions);
	}

	int Turtle::GetCurrentFrame() const
	{
		return _animation.GetCurrentFrame();
	}

	bool Turtle::IsSinkingTurtle() const
	{
		return _isSinkingTurtle;
	}

	void Turtle::MoveLeft(size_t& deltaTime)
	{
		if (_position.x <= _pixelOffscreenReset)
		{
			_position.x = 840 + _dimensions.w;
		}

		_position.x -= static_cast<int>(deltaTime * _speed) / 100;
	}
}