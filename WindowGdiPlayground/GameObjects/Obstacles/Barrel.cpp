#include "GameObjects/Obstacles/Barrel.h"

Barrel::Barrel(const D2D1_POINT_2F& position, const bool& is_destrucable)
	:
	Obstacle(position, is_destrucable)
{
	objectcounter++;
}

Barrel::~Barrel() {
	objectcounter--;
}

void Barrel::draw() {
	s_pgfx->transformTRSM(0.0f, 0.0f, 0.0f, { position.x + 32.0f, position.y + 32.0f }, m_fScalar, m_fScalar, false);
	s_pgfx->drawBitmap(s_pSprite, { position.x, position.y, position.x + 64.0f, position.y + 64.0f }, 1.0f, { 0.0f, 64.0f, 64.0f, 128.0f });
	s_pgfx->restoreDefaultDrawingParameters();
}

void Barrel::loadSprite(const wchar_t* name) {
	if (s_pSprite == nullptr) {
		s_pgfx->loadD2DBitmap(name, 0, s_pSprite);
	}
}

void Barrel::releaseSprite() {
	SafeRelease(&s_pSprite);
}
