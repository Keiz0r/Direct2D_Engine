#pragma once
#include "Keyboard.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include "Gameboard.h"
#include "Sonic.h"
#include "GameLevel.h"
#include "Box.h"
#include <string>
#include "Sound.h"
#include "Log.h"
#include "GameConsole.h"

class Game {
public:
	Game(const HWND &hwnd, Keyboard& kbd);
	~Game();
	void gameLoop();
private:
	void updateGameState();
	void composeFrame();
	void LoadLevel(GameLevel& level);
private:
	Keyboard* m_kbd;
	HWND m_hwnd;
	Graphics m_gfx;
	Log m_log;
	GameConsole m_console;
	FrameTimer ft;
	GameBoard m_board;
	Sonic m_Sonic;
	GameLevel Homescreen;
	std::vector<Box*> boxes{};
};
