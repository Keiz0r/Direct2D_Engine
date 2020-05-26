#pragma once
#include <deque>
#include <string>

//Logging game objects' interactions + input commands
class Log {
public:
	void putMessage(const wchar_t* message);
private:
	static constexpr int size = 500;
	std::deque<std::wstring> commands;
	friend class GameConsole;
};