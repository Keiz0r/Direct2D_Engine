#pragma once
#include "Graphics.h"
#include "Log.h"

class GameConsole {
public:
	GameConsole(Graphics& p_gfx, Log& p_log);
	void draw() const;
	void activate();
private:
	bool active = false;
	Graphics& m_pgfx;
	Log& m_plog;
};