#pragma once
#include "Backend\Graphics.h"
#include "Log.h"
#include <memory>

class GameObject {
public:
	GameObject(const D2D1_POINT_2F& position);
	~GameObject();
private:
	static void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
//	static const Graphics& m_pgfx;
	ID2D1Bitmap* m_pSprite;
	D2D1_POINT_2F position;
	float m_fScalar = 1.0f;
};