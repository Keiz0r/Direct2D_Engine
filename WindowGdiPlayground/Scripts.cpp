#include "Scripts.h"

void Scripts::patrol(GameObject* obj, const float_t& scale) {
	static D2D1_POINT_2F initial_pos = obj->getPosition();	//TODO: these static are static for every function call, so common for any object
	static bool flag_posX = true;
	static bool flag_posY = true;
	static const float_t range = 100.0f;

	D2D1_POINT_2F current_pos = obj->getPosition();
	if (current_pos.x > initial_pos.x + range) {
		flag_posX = false;
	}
	else if (current_pos.x < initial_pos.x - range) {
		flag_posX = true;
	}

	if (flag_posX) {
		obj->setPosition(current_pos.x += 1.0f * scale, current_pos.y);
	}
	else {
		obj->setPosition(current_pos.x -= 1.0f * scale, current_pos.y);
	}

	static uint8_t blinkframes = 300;
	if (blinkframes <= 0) {
		obj->blink(10);
		blinkframes = 300;
	}
	else {
		blinkframes--;
	}
}
