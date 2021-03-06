#pragma once
#include "Backend\Graphics.h"
#include "Log.h"

class GameConsole {
public:
	GameConsole(Graphics& p_gfx);
	void draw();
	void activate();
	void activateInput();
	bool isActive() const;
	bool isActiveInput() const;
	void setInput(const char& c);
	void removeInput();
	std::wstring getInput();
private:
	bool active = false;
	Graphics& m_pgfx;
	bool activeinput = false;
	std::wstring blink = L"_";
	uint8_t blinkCounter = 0;
	std::string tempInput;
	std::wstring input;
	Graphics::D2D_SOLID_COLORS textColor = Graphics::D2D_SOLID_COLORS::Gray;
	Graphics::D2D_SOLID_COLORS bckgndColor = Graphics::D2D_SOLID_COLORS::Black;
};