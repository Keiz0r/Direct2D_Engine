#include "GameLevel.h"

GameLevel::GameLevel(Graphics& p_gfx, const int& width, const int& height)
	:
	m_pgfx(p_gfx),
	m_board(width, height),
	m_pBackgroundSprite(NULL)
{
	m_Borders = { m_board.getBoardSize().y / -2.0f, m_board.getBoardSize().x / -2.0f, m_board.getBoardSize().x / 2.0f,
		m_board.getBoardSize().y / 2.0f, };

	loadSprite(GAMESPRITE(SPRITE_BACKGROUND), m_pBackgroundSprite);

	GameBoard::initialize(GAMESPRITE(SPRITE_WORLD_TILES), &m_pgfx, {1.6f, 0.8f});
}

GameLevel::~GameLevel() {
	SafeRelease(&m_pBackgroundSprite);
}

void GameLevel::draw(const D2D1_POINT_2F& position) {
	if (m_pBackgroundSprite != NULL) {
		D2D1_SIZE_U imageSize = m_pBackgroundSprite->GetPixelSize();
		D2D1_SIZE_F screenSize = m_pgfx.getScreenSize();
		D2D1_POINT_2F screencenter{ screenSize.width / 2.0f, screenSize.height / 2.0f };

		m_pgfx.transformTRSM(0.0f, 0.0f, bkgndRttnAngl, screencenter, 1.0f, 1.0f, false);
		m_pgfx.drawBitmap(m_pBackgroundSprite, { 0.0f, 0.0f, screenSize.width, screenSize.height }, 1.0f, { 0.0f, 0.0f, static_cast<float>(imageSize.width), static_cast<float>(imageSize.height) });
		m_pgfx.restoreDefaultDrawingParameters();
	}

	m_board.Draw(position);
}

void GameLevel::rotateBckgnd(float& angle) {
	bkgndRttnAngl = angle;
}

GameBoard* GameLevel::h_Board() {
	return &m_board;
}

void GameLevel::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
//	if (sprite = NULL) {
		m_pgfx.loadD2DBitmap(name, 0, sprite);
//	}
}
