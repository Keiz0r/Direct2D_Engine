#include "Sonic.h"

Sonic::Sonic(const D2D1_POINT_2F& objectPosition, const float_t& scale, const float_t& opacity, const float_t& rotationAngle)
	:
	GameObject(objectPosition, scale, opacity, rotationAngle),
	IdleAnimation(SONIC_ANIMATION_IDLE),
	RunAnimation_N_W(SONIC_ANIMATION_RUN_N_W),
	RunAnimation_SW_NE(SONIC_ANIMATION_RUN_SW_NE),
	RunAnimation_S_E(SONIC_ANIMATION_RUN_S_E),
	RunAnimation_SE(SONIC_ANIMATION_RUN_SE),
	RunAnimation_NW(SONIC_ANIMATION_RUN_NW)
{
	objectcounter++;
	loadSprite(GAMESPRITE(SPRITE_SONIC), s_pSprite);
}

Sonic::~Sonic() {
	objectcounter--;
	if (objectcounter == 0) {
		releaseSprite(s_pSprite);
	}
}

void Sonic::update() {
	move();

	if (currentState != Sonic::Action::Jump) {
		if (speedUP) {
			// auto setting one of 8 directional states based on velocity
			// x velocity is pos
			if (velocity.x > 0.0001f) {
				if (velocity.y > 0.0001f) {
					setDirection(Sonic::Direction::NE);
				}
				else if (velocity.y < -0.0001f) {
					setDirection(Sonic::Direction::SE);
				}
				else {
					setDirection(Sonic::Direction::E);
				}
			}
			// x velocity is neg
			else if (velocity.x < -0.0001f) {
				if (velocity.y > 0.0001f) {
					setDirection(Sonic::Direction::NW);
				}
				else if (velocity.y < -0.0001f) {
					setDirection(Sonic::Direction::SW);
				}
				else {
					setDirection(Sonic::Direction::W);
				}
			}
			// if x velocity doesn't exist
			else {
				if (velocity.y > 0.0001f) {
					setDirection(Sonic::Direction::N);
				}
				else if (velocity.y < -0.0001f) {
					setDirection(Sonic::Direction::S);
				}
			}
			setState(Sonic::Action::Run);
		}
		else {
			//speed decay, if no speedup was applied
			if (currentState == Sonic::Action::Run) {
				//if (velo - velodecay) doesnt change sign -> decay; else set to 0
				if (velocity.x > 0.0001f) {
					if (velocity.x - acceleration /2 > 0.0001f) {
						velocity.x -= acceleration /2;
					}
					else {
						velocity.x = 0.0f;
					}
				}
				else if (velocity.x < -0.0001f) {
					if (velocity.x + acceleration /2 < -0.0001f) {
						velocity.x += acceleration /2;
					}
					else {
						velocity.x = 0.0f;
					}
				}
				if (velocity.y > 0.0001f) {
					if (velocity.y - acceleration /2 > 0.0001f) {
						velocity.y -= acceleration /2;
					}
					else {
						velocity.y = 0.0f;
					}
				}
				else if (velocity.y < -0.0001f) {
					if (velocity.y + acceleration /2 < -0.0001f) {
						velocity.y += acceleration /2;
					}
					else {
						velocity.y = 0.0f;
					}
				}
				//absolute velocity is around 0
				if ( velocity.x > -0.0001f && velocity.x < 0.0001f && velocity.y > -0.0001f && velocity.y < 0.0001f) {
					setState(Sonic::Action::Idle);
				}
			}
		}
	}
	


	//count frame
	timeFrameCounter++;
	if (animationChanged || directionChanged) {
		timeFrameCounter = 0u;
		animationChanged = false;
		directionChanged = false;
	}
	//reset flag
	speedUP = false;
}

void Sonic::draw() {
	//	Idle animation
	if (currentState == Sonic::Action::Idle) {
		Animate(IdleAnimation);
	}
	// Running animation
	else if (currentState == Sonic::Action::Run) {
		//add animations depending on speed
		switch(currentDirection) {
		case Sonic::Direction::N:
			facingRight = true;
			Animate(RunAnimation_N_W);
			break;
		case Sonic::Direction::NE:
			facingRight = true;
			Animate(RunAnimation_SW_NE);
			break;
		case Sonic::Direction::E:
			facingRight = true;
			Animate(RunAnimation_S_E);
			break;
		case Sonic::Direction::SE:
			Animate(RunAnimation_SE);
			break;
		case Sonic::Direction::S:
			facingRight = false;
			Animate(RunAnimation_S_E);
			break;
		case Sonic::Direction::SW:
			facingRight = false;
			Animate(RunAnimation_SW_NE);
			break;
		case Sonic::Direction::W:
			facingRight = false;
			Animate(RunAnimation_N_W);
			break;
		case Sonic::Direction::NW:
			Animate(RunAnimation_NW);
			break;
		}
	}

}

void Sonic::speedUp(Sonic::Direction direction) {
	float diagonalAccel = acceleration / 1.41f;
	//float diagonalAccel = sqrt((acceleration * acceleration) / 2);
	//float diagonalMaxVel = sqrt((maxVelocity * maxVelocity) / 2);
	switch (direction) {
	case Sonic::Direction::N:
		velocity.y += acceleration;
		break;
	case Sonic::Direction::E:
		velocity.x += acceleration;
		break;
	case Sonic::Direction::W:
		velocity.x -= acceleration;
		break;
	case Sonic::Direction::S:
		velocity.y -= acceleration;
		break;
	case Sonic::Direction::NE:
		velocity.x += diagonalAccel;
		velocity.y += diagonalAccel;
		break;
	case Sonic::Direction::SW:
		velocity.x -= diagonalAccel;
		velocity.y -= diagonalAccel;
		break;
	case Sonic::Direction::NW:
		velocity.x -= diagonalAccel;
		velocity.y += diagonalAccel;
		break;
	case Sonic::Direction::SE:
		velocity.x += diagonalAccel;
		velocity.y -= diagonalAccel;
		break;
	}
	clampVelocity();
	speedUP = true;
}

void Sonic::move() {
	position.x += velocity.x;
	position.y += velocity.y;
}

void Sonic::setMaxVelocity(const float& maxVel) {
	maxVelocity = maxVel;
}

void Sonic::setDirection(Direction direction) {
	if (currentDirection != direction) {
		currentDirection = direction;
		directionChanged = true;
	}
}

void Sonic::setState(Sonic::Action action) {
	if (currentState != action) {
		currentState = action;
		animationChanged = true;
		currentFrameNum = 0u;
	}
}

void Sonic::Animate(AnimationData& Animation) {
	if (s_pSprite != nullptr) {
		static float LegOffset = Animation.Height / -4.0f;
		D2D1_SIZE_F screensize = s_pgfx->getScreenSize();

		// frame switcher
		if (timeFrameCounter > Animation.FrameTime && currentFrameNum < Animation.TotalFrames) {
			currentFrameNum++;
			// frames reset
			if (currentFrameNum == Animation.TotalFrames) {
				currentFrameNum = 0u;
			}
			timeFrameCounter = 0u;
		}

		if(m_blinkframes == 0) {
			// Apply necessary transformations
			//for screen coords
			if (ScreencenteredDrawing) {
				s_pgfx->transformSRTM(0.0f, LegOffset, m_rotationAngle, { screensize.width / 2.0f, screensize.height / 2.0f }, m_fScalar, m_fScalar, !facingRight);
				float drawrectstartX = (screensize.width - Animation.Width) / 2;
				float drawrectstartY = (screensize.height - Animation.Height) / 2;
				s_pgfx->drawBitmap(s_pSprite, { drawrectstartX, drawrectstartY, drawrectstartX + Animation.Width, drawrectstartY + Animation.Height}, 1.0f, Animation.frameCoords[currentFrameNum]);
			}
			else {
				D2D1_POINT_2F imagecenter{ position.x + (Animation.Width / 2), position.y + (Animation.Height / 2) };
				s_pgfx->transformTRSM(0.0f, LegOffset * m_fScalar, m_rotationAngle, imagecenter, m_fScalar, m_fScalar, !facingRight);
				s_pgfx->drawBitmap(s_pSprite, { position.x, position.y, position.x + Animation.Width, position.y + Animation.Height }, 1.0f, Animation.frameCoords[currentFrameNum]);
			}
			//	go back from mirrored sprites
			s_pgfx->restoreDefaultDrawingParameters();
		}
		else {
			m_blinkframes--;
		}
	}
}

void Sonic::clampVelocity() {
	float velVectorLength = velocity.x * velocity.x + velocity.y * velocity.y;
	if (velVectorLength > maxVelocity * maxVelocity) {
		//calc direction, make new vel vector with same direction, but max speed scalar
		float length = sqrtf(velVectorLength);
		D2D1_POINT_2F DirectionVector = { velocity.x / length, velocity.y / length };
		velocity.x = DirectionVector.x * maxVelocity;
		velocity.y = DirectionVector.y * maxVelocity;
	}
}
