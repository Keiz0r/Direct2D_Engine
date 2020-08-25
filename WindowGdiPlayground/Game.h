#pragma once
#include "Keyboard.h"
#include "Backend\Graphics.h"
#include "FrameTimer.h"
#include "GameLevel.h"
#include <string>
#include "Backend\Sound.h"
#include "Log.h"
#include "GameConsole.h"
#include <thread>
#include <memory>
#include "Obstacles.h"
#include <condition_variable>
#include <regex>
#include "Lists/GameObjectsList.h"

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
	template<typename T> std::vector<T> getArgs(const std::wstring cmd) const;
private:
	Keyboard* m_kbd;
	HWND m_hwnd;
	Graphics m_gfx;
	GameConsole m_console;	//copies log every frame. not the best solution probably, but solves invalid access
	FrameTimer ft;
	std::unique_ptr<Sonic> m_pSonic;
	GameLevel m_Level;
	unsigned int updObstacles = 0;	// mek it part  of obstacles after implemented only 1 cmd per frame
	Obstacles m_obstacles;
	bool cmdRun = true;
	std::thread cmdln;
	std::mutex cmdMutex;
	std::condition_variable cmdCV;
	std::wstring str_GameCommand;

	float rotor = 0.0f;
	D2D1_POINT_2F screenCenterCoordinates {0.0f, 0.0f};
	const D2D1_POINT_2F isometricCoefficients {1.6f, 0.8f}; // /1.41f //tied to (Board::cellwidth/amountofspacex)/(2*sqrt(2))
};