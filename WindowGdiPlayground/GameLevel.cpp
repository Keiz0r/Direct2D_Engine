#include "GameLevel.h"

GameLevel::GameLevel(Graphics& p_gfx)
	:
	m_pgfx(p_gfx)
{
}

void GameLevel::draw() {
	m_pgfx.drawBitmap(m_pBackgroundSprite, { 0.0f, 0.0f, 1366.0f, 768.0f }, 1.0f, { 0.0f, 0.0f, 1366.0f, 768.0f });

}

void GameLevel::Initialize()
{
	loadSprite(L"Wallpaper.jpg", m_pBackgroundSprite);
}

void GameLevel::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
//	if (sprite = NULL) {
		m_pgfx.loadD2DBitmap(name, 0, sprite);
//	}
}
