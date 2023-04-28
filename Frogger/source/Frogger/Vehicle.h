#pragma once
#include "GameObject.h"

namespace Frogger
{
	enum Direction
	{
		Left,
		Right
	};

	class Vehicle : GameObject
	{
	public:
		Vehicle();
		void Update(size_t& deltaTime);
		Coordinates2D& GetPosition();
		Dimensions& GetDimensions();
		void SetPosition(int x, int y);
		void SetDimension(int w, int h);
		void SetTexture(SDL_Texture& texture, int textureOffset);
		void SetMovementProperties(float speed, enum Direction direction);
		void Render(SDL_Renderer& renderer);

	private:
		void Move(size_t& deltaTime);
		float _speed;
		int _textureOffset;
		int _pixelOffscreenRightReset;
		int _pixelOffscreenLeftReset;
		Direction _movementDirection;
		SDL_Texture* _texture;
		SDL_RendererFlip _flip;
	};
}

