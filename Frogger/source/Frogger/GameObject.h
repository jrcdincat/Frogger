#pragma once
#include "Coordinates2D.h"
#include "Dimensions.h"

namespace Frogger
{
	class GameObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject() = 0;
		virtual Coordinates2D& GetPosition() = 0;
		virtual Dimensions& GetDimensions() = 0;
		Dimensions _dimensions;
		Coordinates2D _position;
	};
}
