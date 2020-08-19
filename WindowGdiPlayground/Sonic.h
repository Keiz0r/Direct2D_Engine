#pragma once
#include "Backend\Graphics.h"
#include "AnimationData.h"

class Sonic {
public:
	enum class Action {
		Idle, Run, Jump
	};
	enum class Direction {
		N, NE, E, SE, S, SW, W, NW
	};
	Sonic(Graphics& p_gfx, const D2D1_POINT_2F& position);
	~Sonic();
	void update();
	void draw();	// TODO : add various animations depending on speed
	void setPosition(const float& x, const float& y);
	void speedUp(Direction direction);
	void move();
	void setScalar(const float& scalar);
	D2D1_POINT_2F getPosition() const;
	void setMaxVelocity(const float& maxVel);
private:
	void setDirection(Direction direction);
	void setState(Action action);
	void Animate(AnimationData& Animation);
	void Initialize(const D2D1_POINT_2F& position);
	void loadSprite();
	void clampVelocity();
private:
	Graphics& m_pgfx;
	D2D1_POINT_2F position;
	float acceleration = 0.5f;
	D2D1_POINT_2F velocity = { 0.0f, 0.0f };
	float maxVelocity = 6.0f;
	bool speedUP = false;
	float m_fScalar = 1.0f;
	ID2D1Bitmap* m_pSprite;
	bool initialised = false;
	bool facingRight = true;
	Sonic::Action currentState = Sonic::Action::Idle;
	Sonic::Direction currentDirection = Sonic::Direction::NE;
	bool animationChanged = false;
	bool directionChanged = false;
	unsigned int timeFrameCounter = 0u;
	unsigned short int currentFrameNum = 0u;
	AnimationData IdleAnimation;
	AnimationData RunAnimation_N_W;
	AnimationData RunAnimation_SW_NE;
	AnimationData RunAnimation_S_E;
	AnimationData RunAnimation_SE;
	AnimationData RunAnimation_NW;

	bool ScreencenteredDrawing = true;	//Swith to screen coordinates
};