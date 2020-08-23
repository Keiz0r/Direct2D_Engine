#include "GameObjects/Obstacles/Barrel.h"

Barrel::Barrel(const D2D1_POINT_2F& position, const bool& is_destrucable)
	:
	Obstacle(position, is_destrucable)
{
	spriteWidth = 64.0f;
	spriteHeight = 64.0f;
	objectcounter++;
	loadSprite(GAMESPRITE(SPRITE_OBJECTS), s_pSprite);
}

Barrel::~Barrel() {
	objectcounter--;
	if (objectcounter == 0) {
		releaseSprite(s_pSprite);
	}
}

void Barrel::draw() {
	static D2D1_POINT_2F screenCenter{ s_pgfx->getScreenSize().width / 2.0f, s_pgfx->getScreenSize().height / 2.0f };
	D2D1_POINT_2F shift = {screenCenter.x - centerCoordinates->x + position.x, screenCenter.y - centerCoordinates->y + position.y };
	//need current screen center coords (world)
	//position -> screen coords (calculated through position)
	static float halfwidth = spriteWidth / 2.0f;
	static float halfheight = spriteHeight / 2.0f;
//	D2D1_POINT_2F imagecenter{ position.x + (spriteWidth / 2.0f), position.y + (spriteHeight / 2.0f) };
//	s_pgfx->transformTRSM(0.0f, 0.0f, 0.0f, imagecenter, m_fScalar, m_fScalar, false);
	s_pgfx->drawBitmap(s_pSprite, { shift.x - halfwidth, shift.y - halfheight, shift.x + halfwidth, shift.y + halfheight }, 1.0f, { 0.0f, 64.0f, 64.0f, 128.0f });
//	s_pgfx->restoreDefaultDrawingParameters();
}