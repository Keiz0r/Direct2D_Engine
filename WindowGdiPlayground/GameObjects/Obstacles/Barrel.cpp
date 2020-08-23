#include "GameObjects/Obstacles/Barrel.h"

Barrel::Barrel(const D2D1_POINT_2F& position, const float& rotationAngle, const bool& is_destrucable)
	:
	Obstacle(position, rotationAngle, is_destrucable)
{
	spriteWidth = 19.0f;
	spriteHeight = 21.0f;
	LegOffset = spriteHeight / -2.0f;
	objectcounter++;
	loadSprite(GAMESPRITE(SPRITE_OBJECTS), s_pSprite);
}

Barrel::~Barrel() {
	objectcounter--;
	s_pSprite;
	if (objectcounter == 0) {
		releaseSprite(s_pSprite);
	}
}

void Barrel::draw() {
	if (s_pSprite != nullptr) {
		static D2D1_POINT_2F screenCenter{ s_pgfx->getScreenSize().width / 2.0f, s_pgfx->getScreenSize().height / 2.0f };
		D2D1_POINT_2F shift = { screenCenter.x - (centerCoordinates->x + centerCoordinates->y - position.x - position.y) * 1.6f , screenCenter.y - (centerCoordinates->x - centerCoordinates->y - position.x + position.y) * 0.8f };
		static float halfwidth = spriteWidth / 2.0f;
		static float halfheight = spriteHeight / 2.0f;
		s_pgfx->transformTRSM(0.0f, LegOffset, rotationAngle, {shift.x, shift.y}, m_fScalar, m_fScalar, false);
		s_pgfx->drawBitmap(s_pSprite, { shift.x - halfwidth, shift.y - halfheight, shift.x + halfwidth, shift.y + halfheight}, 0.85f, { 24.0f, 26.0f, 43.0f, 47.0f });
		s_pgfx->restoreDefaultDrawingParameters();
	}
}