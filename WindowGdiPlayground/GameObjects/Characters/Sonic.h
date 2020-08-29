#pragma once
#include "Backend\Graphics.h"
#include "AnimationData.h"
#include "GameObjects/GameObject.h"

class Sonic : public GameObject {
public:
	enum class Action {
		Idle, Run, Jump
	};
	enum class Direction {
		N, NE, E, SE, S, SW, W, NW
	};
	Sonic(const D2D1_POINT_2F& objectPosition, const float_t& scale, const float_t& opacity, const float_t& rotationAngle);
	~Sonic();
	void update();
	void draw();
	void speedUp(Direction direction);
	void move();
	void setMaxVelocity(const float& maxVel);
	void blink(const unsigned int& frames);
private:
	void setDirection(Direction direction);
	void setState(Action action);
	void Animate(AnimationData& Animation);
	void clampVelocity();
private:
	inline static ID2D1Bitmap* s_pSprite = nullptr;
	float acceleration = 0.5f;
	D2D1_POINT_2F velocity = { 0.0f, 0.0f };
	float maxVelocity = 6.0f;
	bool speedUP = false;
	Sonic::Action currentState = Sonic::Action::Idle;
	Sonic::Direction currentDirection = Sonic::Direction::NE;
	bool animationChanged = false;
	bool directionChanged = false;
	unsigned short int timeFrameCounter = 0u;
	unsigned short int currentFrameNum = 0u;
	unsigned short int m_blinkframes = 0;
	AnimationData IdleAnimation;
	AnimationData RunAnimation_N_W;
	AnimationData RunAnimation_SW_NE;
	AnimationData RunAnimation_S_E;
	AnimationData RunAnimation_SE;
	AnimationData RunAnimation_NW;

	bool ScreencenteredDrawing = true;	//Swith to screen coordinates
	inline static uint32_t objectcounter = 0;
};