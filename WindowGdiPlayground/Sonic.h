#pragma once
#include "Backend\Graphics.h"
#include "AnimationData.h"

class Sonic {
public:
	enum Action {
		Idle, RRun, LRun, Jump
	};
	Sonic(Graphics& p_gfx, const D2D1_POINT_2F& position);
	~Sonic();
	void update();
	void draw();	// TODO : add various animations depending on speed
	void setAction(Action action);
	void setPosition(const float& x, const float& y);
	void speedUp(const bool& move);
	void setFacing(const bool& facingright);
	void move(const float& x, const float& y);
	void setScalar(const float& scalar);
	D2D1_POINT_2F getPosition() const;
private:
	void setState(Action action);
	void Animate(AnimationData& Animation);
	void Initialize(const D2D1_POINT_2F& position);
	void loadSprite();
	void clampSpeed(const float& topSpeed);
private:
	Graphics& m_pgfx;
	D2D1_POINT_2F position;
	float speed = 0.0f;
	bool speedUP = false;
	float speedDecay = 0.22f;
	float m_fScalar = 1.0f;
	ID2D1Bitmap* m_pSprite;
	bool initialised = false;
	bool facingRight = true;
	int currentState = Idle;
	bool animationChanged = false;
	unsigned int timeFrameCounter = 0u;
	unsigned short int currentFrameNum = 0u;
	AnimationData IdleAnimation;
	AnimationData RunAnimation;
};