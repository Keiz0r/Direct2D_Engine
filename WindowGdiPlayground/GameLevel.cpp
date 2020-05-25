#include "GameLevel.h"

GameLevel::GameLevel(Graphics& p_gfx)
	:
	m_pgfx(p_gfx)
{
}

void GameLevel::draw() {
	m_pgfx.drawBitmap(m_pBackgroundSprite, { 0.0f, 0.0f, 1366.0f, 768.0f }, 1.0f, { 0.0f, 0.0f, m_pBackgroundSprite->GetSize().width, m_pBackgroundSprite->GetSize().height });

}

void GameLevel::Initialize() {
	if(!initialised) {
		loadSprite(L"..\\Assets\\Wallpaper.jpg", m_pBackgroundSprite);
		initialised = true;
	}
}

void GameLevel::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
//	if (sprite = NULL) {
		m_pgfx.loadD2DBitmap(name, 0, sprite);
//	}
}
