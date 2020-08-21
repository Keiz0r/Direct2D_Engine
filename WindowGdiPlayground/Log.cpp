#include "Log.h"

void Log::putMessage(const wchar_t* message) {
	if (commands.size() > size) {
		commands.pop_back();
	}
	commands.push_front(message);
}

void Log::putError(const wchar_t* message) {
	if (errors.size() > size) {
		errors.pop_back();
	}
	errors.push_front(message);
}
