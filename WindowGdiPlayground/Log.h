#pragma once
#include <deque>
#include <string>

//Logging game objects' interactions + input commands
class Log {
	friend class GameConsole;
public:
	static void putMessage(const wchar_t* message);
private:
	static constexpr int size = 500;
	static inline std::deque<std::wstring> commands;
};