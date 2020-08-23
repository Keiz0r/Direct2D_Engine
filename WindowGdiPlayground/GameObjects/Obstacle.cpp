#include "GameObjects/Obstacle.h"

Obstacle::Obstacle(const D2D1_POINT_2F& position, const float& opacity, const float& rotationAngle, const bool& is_destrucable)
	:
	GameObject(position, opacity, rotationAngle),
	destructable(is_destrucable)
{
}

Obstacle::~Obstacle() {
	
}
