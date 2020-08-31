#include "Scripts.h"

Scripts::patrol::patrol(GameObject& obj, const float_t& scale)
	:
	obj(obj),
	scale(scale)
{
	initial_pos = obj.getPosition();
}

Scripts::patrol::patrol(const patrol& p)
	:
	obj(p.obj),
	scale(p.scale)
{
	initial_pos = p.initial_pos;
}

Scripts::patrol::patrol(patrol&& p) noexcept
	:
	obj(p.obj),
	scale(p.scale)
{
	initial_pos = p.initial_pos;

	p.scale = 0.0f;
	p.initial_pos = {0.0f, 0.0f};
}

Scripts::patrol::~patrol() {
}

void Scripts::patrol::operator()() {
	D2D1_POINT_2F current_pos = obj.getPosition();
	if (current_pos.x > initial_pos.x + range) {
		flag_posX = false;
	}
	else if (current_pos.x < initial_pos.x - range) {
		flag_posX = true;
	}

	if (flag_posX) {
		obj.setPosition(current_pos.x += 1.0f * scale, current_pos.y);
	}
	else {
		obj.setPosition(current_pos.x -= 1.0f * scale, current_pos.y);
	}

	if (blinkframes <= 0) {
		obj.blink(10);
		blinkframes = 44;
	}
	else {
		blinkframes--;
	}
}