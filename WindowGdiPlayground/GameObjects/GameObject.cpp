#include "GameObject.h"

GameObject::GameObject(const D2D1_POINT_2F& position)
	:
	position(position)
{
}

GameObject::~GameObject() {
}

void GameObject::initialize(Graphics* gfx) {
	if (s_pgfx == nullptr) {
		s_pgfx = gfx;
	}
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