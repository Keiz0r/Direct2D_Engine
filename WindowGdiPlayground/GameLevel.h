#pragma once
#include "Gameboard.h"
#include "Lists\LevelData.h"

class GameLevel {
public:
	GameLevel(Graphics& p_gfx, const uint32_t& width, const uint32_t& height);
	~GameLevel();
	void draw(const D2D1_POINT_2F& position);
	void rotateBckgnd(float_t& angle);
	GameBoard* h_Board();
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	GameBoard m_board;
	ID2D1Bitmap* m_pBackgroundSprite;
	float_t bkgndRttnAngl = 0.0f;
	D2D1_RECT_F m_Borders;
};