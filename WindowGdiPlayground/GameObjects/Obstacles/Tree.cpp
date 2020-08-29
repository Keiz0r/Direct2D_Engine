#include "GameObjects/Obstacles/Tree.h"

Tree::Tree(const D2D1_POINT_2F& position, const float_t& scale, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable)
	:
	Obstacle(position, scale, opacity, rotationAngle, is_destrucable)
{
	objectcounter++;
	loadSprite(GAMESPRITE(SPRITE_TREE1), s_pSprite);
	spriteSize = s_pSprite->GetPixelSize();
	LegOffset = spriteSize.height / -3.0f ;
}

Tree::~Tree() {
	objectcounter--;
	if (objectcounter == 0) {
		releaseSprite(s_pSprite);
	}
}

void Tree::draw() {
	if (s_pSprite != nullptr) {
		float_t halfwidth = (spriteSize.width * m_fScalar) / 2.0f;
		float_t halfheight = (spriteSize.height * m_fScalar) / 2.0f;
		static D2D1_POINT_2F screenCenter{ s_pgfx->getScreenSize().width / 2.0f, s_pgfx->getScreenSize().height / 2.0f };
		D2D1_POINT_2F shift = { screenCenter.x - (centerCoordinates->x + centerCoordinates->y - position.x - position.y) * 1.6f , screenCenter.y - (centerCoordinates->x - centerCoordinates->y - position.x + position.y) * 0.8f };
		//no draw if beyond screen
		float_t radiusFromScreenCenter = (position.x - centerCoordinates->x) * (position.x - centerCoordinates->x) + (position.y - centerCoordinates->y) * (position.y - centerCoordinates->y);
		if (visibilityRadius > radiusFromScreenCenter) {
			s_pgfx->transformTRSM(0.0f, LegOffset * m_fScalar * m_fScalar, rotationAngle, { shift.x, shift.y }, m_fScalar, m_fScalar, false);
			s_pgfx->drawBitmap(s_pSprite, { shift.x - halfwidth, shift.y - halfheight, shift.x + halfwidth, shift.y + halfheight }, opacity, { 0.0f, 0.0f, static_cast<float>(spriteSize.width), static_cast<float>(spriteSize.height) });
			s_pgfx->restoreDefaultDrawingParameters();
		}
	}
}