#include "GameObjects/Obstacles/Tree.h"

Tree::Tree(const D2D1_POINT_2F& position, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable)
	:
	Obstacle(position, opacity, rotationAngle, is_destrucable)
{
	spriteWidth = 19.0f;
	spriteHeight = 21.0f;
	LegOffset = spriteHeight / -2.0f;
	objectcounter++;
}

Tree::~Tree() {
	objectcounter--;
}

void Tree::draw() {
	if (s_pSprite != nullptr) {
		static float_t halfwidth = spriteWidth / 2.0f;
		static float_t halfheight = spriteHeight / 2.0f;
		static D2D1_POINT_2F screenCenter{ s_pgfx->getScreenSize().width / 2.0f, s_pgfx->getScreenSize().height / 2.0f };
		D2D1_POINT_2F shift = { screenCenter.x - (centerCoordinates->x + centerCoordinates->y - position.x - position.y) * 1.6f , screenCenter.y - (centerCoordinates->x - centerCoordinates->y - position.x + position.y) * 0.8f };
		//no draw if beyond screen
		float_t radiusFromScreenCenter = (position.x - centerCoordinates->x) * (position.x - centerCoordinates->x) + (position.y - centerCoordinates->y) * (position.y - centerCoordinates->y);
		if (visibilityRadius > radiusFromScreenCenter) {
			s_pgfx->transformTRSM(0.0f, LegOffset * m_fScalar, rotationAngle, { shift.x, shift.y }, m_fScalar, m_fScalar, false);
			s_pgfx->drawBitmap(s_pSprite, { shift.x - halfwidth, shift.y - halfheight, shift.x + halfwidth, shift.y + halfheight }, opacity, { 24.0f, 26.0f, 43.0f, 47.0f });
			s_pgfx->restoreDefaultDrawingParameters();
		}
	}
}