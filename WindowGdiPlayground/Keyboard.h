#pragma once
#include <queue>
#include <bitset>

class Keyboard {
	friend class mainWindow;
public:
	class Event {
	public:
		enum Type {
			Pressed, Released, Invalid
		};
	public:
		Event();
		Event(Type type, unsigned char code);
		bool isPressed() const;
		bool isReleased() const;
		bool isValid() const;
		unsigned char getKeyCode() const;
	private:
		Type type;
		unsigned char keyCode;
	};
public:
	Keyboard() = default;
	bool keyIsPressed(unsigned char keycode) const;
	Event readKey();
	char readChar();
	bool keyIsEmpty() const;
	bool charIsEmpty() const;
	void flushKeyBuffer();
	void flushCharBuffer();
	void flushKeyboard();
	void enableAutorepeat();
	void disableAutorepeat();
	bool autorepeatEnabled() const;
private:
	void onKeyPressed(unsigned char keycode);
	void onKeyReleased(unsigned char keycode);
	void onChar(char c);
	template<typename T>
	void trimBuffer(std::queue<T> buffer);
private:
	static constexpr uint16_t nKeys = 256u;
	static constexpr uint8_t bufferSize = 4u;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
	bool autorepeat = false;
};