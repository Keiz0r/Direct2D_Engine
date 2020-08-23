#pragma once
#include "GameObjects/GameObject.h"

class Obstacle : public GameObject {
public:
	Obstacle(const D2D1_POINT_2F& position, const float& opacity, const float& rotationAngle, const bool& is_destrucable);
	virtual ~Obstacle();
protected:
	bool destructable = false;
};