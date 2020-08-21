#pragma once
#include <deque>
#include <string>

//Logging game objects' interactions + input commands
class Log {
	friend class GameConsole;
public:
	Log() = delete;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
	~Log() = delete;
	static void putMessage(const wchar_t* message);
	static void putError(const wchar_t* message);
private:
	static constexpr uint32_t size = 500;
	static inline std::deque<std::wstring> commands;
	static inline std::deque<std::wstring> errors;
};