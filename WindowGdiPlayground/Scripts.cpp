#include "Scripts.h"

void Scripts::patrol(D2D1_POINT_2F& pos, const float_t& scale) {
	static D2D1_POINT_2F initial_pos = pos;
	static bool flag_posX = true;
	static bool flag_posY = true;
	static const float_t range = 100.0f;
	if (pos.x > initial_pos.x + range) {
		flag_posX = false;
	}
	else if (pos.x < initial_pos.x - range) {
		flag_posX = true;
	}

	if (flag_posX) {
		pos.x += 1.0f * scale;
	}
	else {
		pos.x -= 1.0f * scale;
	}
}
