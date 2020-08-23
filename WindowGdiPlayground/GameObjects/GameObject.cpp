#include "GameObject.h"

GameObject::GameObject(const D2D1_POINT_2F& objectPosition, const float& opacity, const float& rotationAngle)
	:
	position(objectPosition),
	opacity(opacity),
	rotationAngle(rotationAngle)
{
}

GameObject::~GameObject() {
}

void GameObject::initialize(Graphics* gfx, D2D1_POINT_2F* screencenterVar, const D2D1_POINT_2F& isometricCoeffs) {
	if (s_pgfx == nullptr) {
		s_pgfx = gfx;
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

D2D1_POINT_2F GameObject::getPosition() const {
	return position;
}

void GameObject::setScalar(const float& scalar) {
	m_fScalar = scalar;
	if (scalar < 0.01f) {
		Log::putError(L" Object scalar is 0 or negative");
	}
}