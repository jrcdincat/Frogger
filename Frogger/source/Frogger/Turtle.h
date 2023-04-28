#pragma once
#include "GameObject.h"
#include "Animation.h"

namespace Frogger
{
	class Turtle : GameObject
	{
	public: 
		Turtle();
		void Update(size_t& deltaTime);
		Coordinates2D& GetPosition();
		Dimensions& GetDimensions();
		SDL_Texture* GetTexture();
		void SetPosition(int x, int y);
		void SetDimension(int w, int h);
		void SetTexture(SDL_Texture& texture);
		void SetSpeed(float speed);
		void SetAnimationProperties(int numberOfFrames, int animationSpeed, SDL_RendererFlip flip, bool loop);
		void Render(SDL_Renderer& renderer, SDL_Texture& texture);
		int GetCurrentFrame() const;
		bool IsSinkingTurtle() const;

	private:
		void MoveLeft(size_t& deltaTime);

		float _speed;
		SDL_Texture* _texture;
		Animation _animation;
		bool _isSinkingTurtle;
		int _pixelOffscreenReset;
	};
}

