#include "Box.h"

Box::Box(Graphics& p_gfx, const D2D1_POINT_2F& position)
	:
	m_pgfx(p_gfx),
	position(position),
	m_pSprite(NULL)

{
	Initialize();
}

Box::~Box() {
	SafeRelease(&m_pSprite);
}

void Box::draw() {
	m_pgfx.transformTRSM(0.0f, 0.0f, 0.0f, { position.x + 32.0f, position.y + 32.0f}, 4.0f, 4.0f, false);
	m_pgfx.drawBitmap(m_pSprite, { position.x, position.y, position.x + 64.0f, position.y + 64.0f }, 1.0f, { 0.0f, 64.0f, 64.0f, 128.0f });
	m_pgfx.restoreDefaultDrawingParameters();
}

void Box::Initialize() {
	if (!initialised) {
		loadSprite(GAMESPRITE(Objects_Sprites.png), m_pSprite);
		initialised = true;
	}
}

D2D1_POINT_2F Box::getPosition() const {
	return position;
}

void Box::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
	m_pgfx.loadD2DBitmap(name, 0, sprite);
}
