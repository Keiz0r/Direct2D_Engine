#include "GameLevel.h"

GameLevel::GameLevel(Graphics& p_gfx)
	:
	m_pgfx(p_gfx),
	m_pBackgroundSprite(NULL)
{
}

GameLevel::~GameLevel() {
	SafeRelease(&m_pBackgroundSprite);
}

void GameLevel::draw() {
	D2D1_POINT_2F screencenter{ 1366.0f / 2, 768.0f / 2 };
	// Apply necessary transformations
	m_pgfx.transformTRSM(0.0f, 0.0f, bkgndRttnAngl, screencenter, 1.0f, 1.0f, false);
	m_pgfx.drawBitmap(m_pBackgroundSprite, { 0.0f, 0.0f, 1366.0f, 768.0f }, 1.0f, { 0.0f, 0.0f, 1366.0f, 768.0f });
	//	go back from mirrored sprites
	m_pgfx.restoreDefaultDrawingParameters();
}

void GameLevel::rotateBckgnd(float& angle) {
	bkgndRttnAngl = angle;
}

void GameLevel::Initialize() {
	if(!initialised) {
		loadSprite(GAMESPRITE(Wallpaper.jpg), m_pBackgroundSprite);
		initialised = true;
	}
}

void GameLevel::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
//	if (sprite = NULL) {
		m_pgfx.loadD2DBitmap(name, 0, sprite);
//	}
}
