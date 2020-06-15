#pragma once
#include "Graphics.h"
#include "Gameboard.h"

class GameLevel {
public:
	GameLevel(Graphics& p_gfx, const int& width, const int& height);
	~GameLevel();
	void draw();
	void rotateBckgnd(float& angle);
	void Initialize();
	GameBoard& h_Board();
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	GameBoard m_board;
	ID2D1Bitmap* m_pBackgroundSprite;
	bool initialised = false;
	float bkgndRttnAngl = 0.0f;
};