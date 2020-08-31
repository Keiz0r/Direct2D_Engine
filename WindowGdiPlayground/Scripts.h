#pragma once
#include "Backend/Graphics.h"
#include "GameObjects/GameObject.h"

namespace Scripts {
	class patrol {
	public:
		patrol(GameObject& obj, const float_t& scale);
		patrol(const patrol& p);
		patrol(patrol&& p) noexcept;
		~patrol();
		void operator()();
	private:
		GameObject& obj;
		float_t scale;
		D2D1_POINT_2F initial_pos;
		bool flag_posX = true;
		bool flag_posY = true;
		const float_t range = 100.0f;
		uint8_t blinkframes = 44;
	};
}