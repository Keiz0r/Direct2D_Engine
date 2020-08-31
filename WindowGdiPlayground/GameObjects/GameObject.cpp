#include "GameObject.h"

GameObject::GameObject(const D2D1_POINT_2F& objectPosition, const float_t& scale, const float_t& opacity, const float_t& rotationAngle)
	:
	position(objectPosition),
	m_fScalar(scale),
	m_opacity(opacity),
	m_rotationAngle(rotationAngle)
{
}

GameObject::~GameObject() {
}

void GameObject::initialize(Graphics* gfx, D2D1_POINT_2F* screencenterVar, const D2D1_POINT_2F& isometricCoeffs) {
	if (s_pgfx == nullptr) {
		s_pgfx = gfx;
	}
	if (centerCoordinates == nullptr) {
		centerCoordinates = screencenterVar;
		isometricCoefficients = isometricCoeffs;
	}
}

void GameObject::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
	if (sprite == nullptr) {
		s_pgfx->loadD2DBitmap(name, 0, sprite);
	}
}

void GameObject::releaseSprite(ID2D1Bitmap*& sprite) {
	SafeRelease(&sprite);
}

void GameObject::setPosition(const D2D1_POINT_2F& pos) {
	position = pos;
}

void GameObject::setPosition(const float_t& x, const float_t& y) {
	position.x = x;
	position.y = y;
}

D2D1_POINT_2F GameObject::getPosition() const {
	return position;
}

void GameObject::setRotationAngle(const float_t& Rangle) {
	m_rotationAngle = Rangle;
}

void GameObject::setScalar(const float_t& scalar) {
	m_fScalar = scalar;
	if (scalar < 0.01f) {
		Log::putError(L" Object scalar is 0 or negative");
	}
}

void GameObject::setOpacity(const float_t& opac) {
	m_opacity = opac;
}

void GameObject::blink(const unsigned int& frames) {
	m_blinkframes = frames;
}

void GameObject::runScript() {
	if (p_script != nullptr) {
		p_script();
	}
}
