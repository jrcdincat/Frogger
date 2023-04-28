#pragma once
#include "Coordinates2D.h"

namespace Frogger
{
	struct Home
	{
		Home();
		Coordinates2D _positions;
		bool _successfulGoal;
		SDL_Texture* _frogSuccessTexture;
	};
}
