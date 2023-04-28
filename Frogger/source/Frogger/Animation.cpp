#include "pch.h"
#include "Animation.h"

namespace Frogger
{
	Animation::Animation() : _loop { false }, _isEnd { false }, _frame { 0 },
		_numberOfFrames { 0 }, _animationSpeed { 0 }, _flip { SDL_FLIP_NONE }
	{
	}

	void Animation::Update()
	{
		// Play all frames of animation in loop or single time
		if (_loop || !_isEnd)
		{
			_isEnd = false;
			_frame = (SDL_GetTicks() / _animationSpeed) % _numberOfFrames;
		}

		if (!_loop && _frame == _numberOfFrames - 1)
		{
			_isEnd = true;
			_frame = _numberOfFrames - 1;
		}
	}

	void Animation::Render(SDL_Renderer* renderer, SDL_Texture* texture, Coordinates2D& position, Dimensions& dimensions)
	{
		SDL_Rect srcRect{ dimensions.w * _frame,0, dimensions.w, dimensions.h};
		SDL_Rect dstRect{ position.x, position.y, dimensions.w, dimensions.h};
		SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0, nullptr, _flip);
	}

	void Animation::SetAnimationProperties(int numberOfFrames, int animationSpeed, SDL_RendererFlip flip, bool loop)
	{
		_numberOfFrames = numberOfFrames;
		_animationSpeed = animationSpeed;
		_flip = flip;
		_loop = loop;
	}

	int Animation::GetCurrentFrame() const
	{
		return _frame;
	}

	void Animation::Play()
	{
		_isEnd = false;
	}
}
