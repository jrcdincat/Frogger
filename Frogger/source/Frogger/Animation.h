#pragma once
#include "GameObject.h"

namespace Frogger
{
	class Animation
	{
	public :
		Animation();
		void Update();
		void Render(SDL_Renderer* renderer, SDL_Texture* texture, Coordinates2D& position, Dimensions& dimensions );
		void SetAnimationProperties(int numberOfFrames, int animationSpeed, SDL_RendererFlip flip, bool _loop);
		int GetCurrentFrame() const;
		void Play();

	private:
		bool _loop;
		bool _isEnd;
		int _frame;
		int _numberOfFrames;
		int _animationSpeed;
		SDL_RendererFlip _flip;
	};
}
