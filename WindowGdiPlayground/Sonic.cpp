#include "Sonic.h"

Sonic::Sonic(Graphics& p_gfx, const D2D1_POINT_2F& position)
	:
	m_pgfx(p_gfx),
	m_pSprite(NULL),
	IdleAnimation(SONIC_ANIMATION_IDLE),
	RunAnimation(SONIC_ANIMATION_RUN)
{
	Initialize(position);
}

Sonic::~Sonic()
{
	SafeRelease(&m_pSprite);
}

void Sonic::update() {
	move(speed, 0.0f);

	if (currentState != Jump) {
		clampSpeed(8.0f);

		// auto set state
		if (speed > 0.0f) {
			setState(RRun);
			facingRight = true;
		}
		else if (speed < 0.0f) {
			setState(LRun);
			facingRight = false;
		}

		if (currentState == RRun) {
			if (!speedUP) {
				speed -= speedDecay;
				if (speed < 0.0f) {
					speed = 0.0f;
					setState(Idle);
				}
			}
		}
		else if (currentState == LRun) {
			if (!speedUP) {
				speed += speedDecay;
				if (speed > 0.0f) {
					speed = 0.0f;
					setState(Idle);
				}
			}
		}
	}
	


	//count frame
	timeFrameCounter++;
	if (animationChanged) {
		timeFrameCounter = 0u;
		animationChanged = false;
	}
	//reset flag
	speedUP = false;
}

void Sonic::draw()
{
	//	Idle animation
	if (currentState == Idle) {
		Animate(IdleAnimation);
	}
	// Running animation
	else if (currentState == RRun || currentState == LRun) {
		//add animations depending on speed
		Animate(RunAnimation);
	}

	//	go back from mirrored sprites
	if (!facingRight) {
		m_pgfx.restoreDefaultDrawingParameters();
	}
}

void Sonic::setAction(Action action) {
	currentState = action;
}

void Sonic::setPosition(const float& x, const float& y)
{
	position.x = x;
	position.y = y;
}

void Sonic::speedUp(const bool& move) {
	if (move) {
		speed += 0.5f;
	}
	else {
		speed -= 0.5f;
	}
	speedUP = true;
}

void Sonic::setFacing(const bool& facingright)
{
	facingRight = facingright;
}

void Sonic::move(const float& x, const float& y)
{
	position.x += x;
	position.y += y;
}

void Sonic::setState(Action action) {
	if (currentState != action) {
		currentState = action;
		animationChanged = true;
		currentFrameNum = 0u;
	}
}

void Sonic::Animate(AnimationData& Animation) {

	// check facing, flip picture if necessary
	if (!facingRight) {
		m_pgfx.mirrorDrawing(2 * (position.x + (Animation.Width / 2)), 0.0f);
	}

	if (timeFrameCounter > Animation.FrameTime&& currentFrameNum < Animation.TotalFrames) {
		currentFrameNum++;
		if (currentFrameNum == Animation.TotalFrames) {
			currentFrameNum = 0u;
		}
		timeFrameCounter = 0u;
	}
	m_pgfx.drawBitmap(m_pSprite, { position.x, position.y, position.x + Animation.Width, position.y + Animation.Height }, 1.0f, { (currentFrameNum * (Animation.Width + Animation.Stride)) + Animation.BatchStartx, Animation.BatchStarty, ((currentFrameNum * (Animation.Stride + Animation.Width)) + Animation.Width) + Animation.BatchStartx, Animation.BatchStarty + Animation.Height });
}

void Sonic::Initialize(const D2D1_POINT_2F& position)
{
	if (!initialised) {
	setPosition(position.x, position.y);
	loadSprite();
	initialised = true;
	}
}

void Sonic::loadSprite() {
	m_pgfx.loadD2DBitmap(L"Sonics.png", 0, m_pSprite);
}

void Sonic::clampSpeed(const float& topSpeed) {
	if (speed > topSpeed) {
		speed = topSpeed;
	}
	else if (speed < (-1) * topSpeed) {
		speed = (-1) * topSpeed;
	}
}
