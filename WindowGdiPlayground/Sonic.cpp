#include "Sonic.h"

Sonic::Sonic(Graphics& p_gfx, const D2D1_POINT_2F& position)
	:
	m_pgfx(p_gfx),
	m_pSprite(NULL),
	IdleAnimation(SONIC_ANIMATION_IDLE),
	RunAnimation_UP(SONIC_ANIMATION_RUN_UP),
	RunAnimation_LR(SONIC_ANIMATION_RUN_LR)
{
	Initialize(position);
}

Sonic::~Sonic() {
	SafeRelease(&m_pSprite);
}

void Sonic::update() {
	move(speed, 0.0f);

	if (currentState != to_underlying(Sonic::Action::Jump)) {
		clampSpeed(8.0f);

		// auto set state
		if (speed > 0.0f) {
			setState(Sonic::Action::RRun);
			facingRight = true;
		}
		else if (speed < 0.0f) {
			setState(Sonic::Action::LRun);
			facingRight = false;
		}

		if (currentState == to_underlying(Sonic::Action::RRun)) {
			if (!speedUP) {
				speed -= speedDecay;
				if (speed < 0.0f) {
					speed = 0.0f;
					setState(Sonic::Action::Idle);
				}
			}
		}
		else if (currentState == to_underlying(Sonic::Action::LRun)) {
			if (!speedUP) {
				speed += speedDecay;
				if (speed > 0.0f) {
					speed = 0.0f;
					setState(Sonic::Action::Idle);
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
	if (currentState == to_underlying(Sonic::Action::Idle)) {
		Animate(IdleAnimation);
	}
	// Running animation
	else if (currentState == to_underlying(Sonic::Action::RRun) || currentState == to_underlying(Sonic::Action::LRun)) {
		//add animations depending on speed
		Animate(RunAnimation_LR);
	}

}

void Sonic::setAction(Sonic::Action action) {
	currentState = to_underlying(action);
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

void Sonic::setScalar(const float& scalar) {
	m_fScalar = scalar;
}

D2D1_POINT_2F Sonic::getPosition() const {
	return position;
}

void Sonic::setState(Sonic::Action action) {
	if (currentState != to_underlying(action)) {
		currentState = to_underlying(action);
		animationChanged = true;
		currentFrameNum = 0u;
	}
}

void Sonic::Animate(AnimationData& Animation) {
	D2D1_POINT_2F imagecenter{ position.x + (Animation.Width / 2), position.y + (Animation.Height / 2) };

	// frame switcher
	if (timeFrameCounter > Animation.FrameTime && currentFrameNum < Animation.TotalFrames) {
		currentFrameNum++;
		// frames reset
		if (currentFrameNum == Animation.TotalFrames) {
			currentFrameNum = 0u;
		}
		timeFrameCounter = 0u;
	}
	// Apply necessary transformations
	m_pgfx.transformTRSM(0.0f, 0.0f, 0.0f, imagecenter, m_fScalar, m_fScalar, !facingRight);
	m_pgfx.drawBitmap(m_pSprite, { position.x, position.y, position.x + Animation.Width, position.y + Animation.Height }, 1.0f, Animation.frameCoords[currentFrameNum]);
	//	go back from mirrored sprites
	m_pgfx.restoreDefaultDrawingParameters();
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
	m_pgfx.loadD2DBitmap(GAMESPRITE(SPRITE_SONIC), 0, m_pSprite);
}

void Sonic::clampSpeed(const float& topSpeed) {
	if (speed > topSpeed) {
		speed = topSpeed;
	}
	else if (speed < (-1) * topSpeed) {
		speed = (-1) * topSpeed;
	}
}
