#include "GameObjects/Obstacle.h"

Obstacle::Obstacle(const D2D1_POINT_2F& position, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable)
	:
	GameObject(position, opacity, rotationAngle),
	destructable(is_destrucable)
{
	obstacleCounter++;
	loadSprite(GAMESPRITE(SPRITE_OBJECTS), s_pSprite);
}

Obstacle::~Obstacle() {
	obstacleCounter--;
	if (obstacleCounter == 0) {
		releaseSprite(s_pSprite);
	}
}
