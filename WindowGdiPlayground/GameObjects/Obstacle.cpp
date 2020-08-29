#include "GameObjects/Obstacle.h"

Obstacle::Obstacle(const D2D1_POINT_2F& position, const float_t& scale, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable)
	:
	GameObject(position, scale, opacity, rotationAngle),
	destructable(is_destrucable)
{
}

Obstacle::~Obstacle() {
}
