#pragma once
#include <random>
#include "GameObjects/Obstacle.h"

class Tree : public Obstacle {
public:
	Tree(const D2D1_POINT_2F& position, const float_t& scale, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable);
	~Tree();
	enum class Treetype {
		Regular, Spruce, Palm1, Palm2, Willow1, Willow2, Pine, Aspen, Type9, Dried, Type11, Type12, Type13, Type14, Count
	};
	void draw() override;
private:
	inline static ID2D1Bitmap* s_pSprite[14] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	D2D1_SIZE_U spriteSize = { 0, 0 };
	inline static float_t LegOffset = 0;
	inline static uint32_t objectcounter = 0;
	Treetype type = Treetype::Regular;
};
