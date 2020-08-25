#pragma once
#include "Backend\Graphics.h"
#include "AnimationData.h"

class Box {
public:
	Box(Graphics& p_gfx, const D2D1_POINT_2F& position);
	~Box();
	void draw();
	void Initialize();
	D2D1_POINT_2F getPosition() const;
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	D2D1_POINT_2F position;
	float_t m_fScalar = 1.0f;
	ID2D1Bitmap* m_pSprite;
	bool initialised = false;
};