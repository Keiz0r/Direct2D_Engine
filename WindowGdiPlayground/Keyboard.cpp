#include "Keyboard.h"

Keyboard::Event::Event() :
	type(Invalid),
	keyCode(0u)
{
}

Keyboard::Event::Event(Type type, unsigned char keyCode)
	:
	type(type),
	keyCode(keyCode)
{
}

bool Keyboard::Event::isPressed() const {
	return type == Pressed;
}

bool Keyboard::Event::isReleased() const {
	return type == Released;
}

bool Keyboard::Event::isValid() const {
	return type != Invalid;
}

unsigned char Keyboard::Event::getKeyCode() const {
	return keyCode;
}

bool Keyboard::keyIsPressed(unsigned char keycode) const {
	return keyStates[keycode];
}

Keyboard::Event Keyboard::readKey() {
	if (keyBuffer.size() > 0u) {
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	else {
	return Keyboard::Event();
	}
}

char Keyboard::readChar() {
	if (charBuffer.size() > 0u) {
		char charCode = charBuffer.front();
		charBuffer.pop();
		return charCode;
	}
	else {
		return 0;
	}
}

bool Keyboard::keyIsEmpty() const {
	return keyBuffer.empty();
}

bool Keyboard::charIsEmpty() const {
	return charBuffer.empty();
}

void Keyboard::flushKeyBuffer() {
	keyBuffer = std::queue<Event>();
}

void Keyboard::flushCharBuffer() {
	charBuffer = std::queue<char>();
}

void Keyboard::flushKeyboard() {
	flushKeyBuffer();
	flushCharBuffer();
}

void Keyboard::enableAutorepeat() {
	autorepeat = true;
}

void Keyboard::disableAutorepeat() {
	autorepeat = false;
}

bool Keyboard::autorepeatEnabled() const {
	return autorepeat;
}

void Keyboard::onKeyPressed(unsigned char keycode) {
	keyStates[keycode] = true;
	keyBuffer.push(Keyboard::Event(Event::Pressed, keycode));
	trimBuffer(keyBuffer);
}

void Keyboard::onKeyReleased(unsigned char keycode) {
	keyStates[keycode] = false;
	keyBuffer.push(Keyboard::Event(Event::Released,keycode));
	trimBuffer(keyBuffer);
}

void Keyboard::onChar(char c) {
	charBuffer.push(c);
	trimBuffer(charBuffer);
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T> buffer) {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}