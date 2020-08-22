#pragma once
#include "Backend\Graphics.h"
#include "Log.h"

class GameObject {
public:
	GameObject(const D2D1_POINT_2F& position);
	virtual ~GameObject();
	virtual void draw() = 0;
	void setPosition(const D2D1_POINT_2F& pos);
	D2D1_POINT_2F getPosition() const;
	void setScalar(const float& scalar);
	static void initialize(Graphics* gfx);
private:
protected:
	inline static Graphics* s_pgfx = nullptr;

	D2D1_POINT_2F position;
	float m_fScalar = 1.0f;
};