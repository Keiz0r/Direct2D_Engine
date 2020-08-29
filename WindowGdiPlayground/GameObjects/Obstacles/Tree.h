#pragma once
#include "GameObjects/Obstacle.h"

class Tree : public Obstacle {
public:
	Tree(const D2D1_POINT_2F& position, const float_t& scale, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable);
	~Tree();
	void draw() override;
private:
	inline static ID2D1Bitmap* s_pSprite = nullptr;
	D2D1_SIZE_U spriteSize = { 0, 0 };
	inline static float_t LegOffset = 0;
	inline static uint32_t objectcounter = 0;
};
