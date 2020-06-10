#pragma once
#include "Graphics.h"

class GameLevel {
public:
	GameLevel(Graphics& p_gfx);
	~GameLevel();
	void draw();
	void rotateBckgnd(float& angle);
	void Initialize();
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	ID2D1Bitmap* m_pBackgroundSprite;
	bool initialised = false;
	float bkgndRttnAngl = 0.0f;
};