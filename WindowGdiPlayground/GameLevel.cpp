#include "GameLevel.h"

GameLevel::GameLevel(Graphics& p_gfx, const int& width, const int& height)
	:
	m_pgfx(p_gfx),
	m_board(p_gfx, width, height),
	m_pBackgroundSprite(NULL)
{
}

GameLevel::~GameLevel() {
	SafeRelease(&m_pBackgroundSprite);
}

void GameLevel::draw(const D2D1_POINT_2F& CenterCoord) {
	D2D1_POINT_2F screencenter{ 1366.0f / 2, 768.0f / 2 };
	// Apply necessary transformations
	m_pgfx.transformTRSM(0.0f, 0.0f, bkgndRttnAngl, screencenter, 1.0f, 1.0f, false);
	m_pgfx.drawBitmap(m_pBackgroundSprite, { 0.0f, 0.0f, 1366.0f, 768.0f }, 1.0f, { 0.0f, 0.0f, 1366.0f, 768.0f });
	//	go back from mirrored sprites
	m_pgfx.restoreDefaultDrawingParameters();
	m_board.drawBoardCells(CenterCoord);
}

void GameLevel::rotateBckgnd(float& angle) {
	bkgndRttnAngl = angle;
}

void GameLevel::Initialize() {
	if(!initialised) {
		loadSprite(GAMESPRITE(SPRITE_BACKGROUND), m_pBackgroundSprite);
		initialised = true;
	}
}

GameBoard& GameLevel::h_Board() {
	return m_board;
}

void GameLevel::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
//	if (sprite = NULL) {
		m_pgfx.loadD2DBitmap(name, 0, sprite);
//	}
}
