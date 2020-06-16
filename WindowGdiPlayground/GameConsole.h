#pragma once
#include "Graphics.h"
#include "Log.h"

class GameConsole {
public:
	GameConsole(Graphics& p_gfx, Log& p_log);
	void draw();
	void activate();
	void activateInput();
	bool isActive() const;
	bool isActiveInput() const;
	void setInput(const char& c);
	std::wstring getInput();
private:
	bool active = false;
	Graphics& m_pgfx;
	Log& m_plog;
	bool activeinput = false;
	std::wstring blink = L"_";
	unsigned short blinkCounter = 0;
	std::string tempInput;
	std::wstring input;
	unsigned short textColor = Graphics::D2D_SOLID_COLORS::Gray;
	unsigned short bckgndColor = Graphics::D2D_SOLID_COLORS::Black;
};