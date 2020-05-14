#pragma once
#include "Graphics.h"

class GameLevel {
public:
	GameLevel(Graphics& p_gfx);
	void draw();
	void Initialize();
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	ID2D1Bitmap* m_pBackgroundSprite;
};