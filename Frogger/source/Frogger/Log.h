#pragma once
#include "GameObject.h"

namespace Frogger
{
	class Log : GameObject
	{
	public :
		Log();
		void Update(size_t& deltaTime);
		Coordinates2D& GetPosition();
		Dimensions& GetDimensions();
		SDL_Texture* GetTexture();
		void SetPosition(int x, int y);
		void SetDimension(int w, int h);
		void SetTexture(SDL_Texture& texture);
		void SetSpeed(float speed);

	private:
		void MoveRight(size_t& deltaTime);

		int _pixelOffscreenReset;
		float _speed;
		SDL_Texture* _texture;
	};
}
