#pragma once
#include "Keyboard.h"
#include "Backend\Graphics.h"
#include "FrameTimer.h"
#include "Sonic.h"
#include "GameLevel.h"
#include <string>
#include "Backend\Sound.h"
#include "Log.h"
#include "GameConsole.h"
#include <thread>
#include <memory>
#include "Obstacles.h"
#include <condition_variable>

//Prob needs an "INPUT" class with CMDthread managing

class Game {
public:
	Game(const HWND &hwnd, Keyboard& kbd);
	~Game();
	void gameLoop();
	void commandInput();
private:
	void updateGameState();
	void composeFrame();
	void LoadLevel(GameLevel& level);
	void execCommand(std::wstring& command);
	void clampCoordinates(Sonic& sonic);
private:
	Keyboard* m_kbd;
	HWND m_hwnd;
	Graphics m_gfx;
	GameConsole m_console;	//copies log every frame. not the best solution probably, but solves invalid access
	FrameTimer ft;
	Sonic m_Sonic;
	GameLevel m_Level;
	unsigned int updObstacles = 0;	// mek it part  of obstacles after implemented only 1 cmd per frame
	Obstacles m_obstacles;

	bool cmdRun = true;
	std::thread cmdln;
	std::mutex cmdMutex;
	std::condition_variable cmdCV;
	std::wstring str_GameCommand;

	float rotor = 0.0f;
};
