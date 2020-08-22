#include "GameObjects/Obstacle.h"

Obstacle::Obstacle(const D2D1_POINT_2F& position, const bool& is_destrucable)
	:
	GameObject(position),
	destructable(is_destrucable)
{
}

Obstacle::~Obstacle() {
	
}
