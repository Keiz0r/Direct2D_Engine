#pragma once
#include "GameObjects/Obstacle.h"

class Barrel : public Obstacle {
public:
	Barrel(const D2D1_POINT_2F& position, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable);
	~Barrel();
	void draw() override;
private:
	inline static ID2D1Bitmap* s_pSprite = nullptr;
	inline static float_t LegOffset = 0;
	inline static uint32_t objectcounter = 0;
};