#pragma once
#include "GameObjects/GameObject.h"

class Obstacle : public GameObject {
public:
	Obstacle(const D2D1_POINT_2F& position, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable);
	~Obstacle();
protected:
	inline static ID2D1Bitmap* s_pSprite = nullptr;
	bool destructable = false;
private:
	static inline uint32_t obstacleCounter = 0;
};