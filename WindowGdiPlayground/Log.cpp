#include "Log.h"

void Log::putMessage(const wchar_t* p_message) {
	if (commands.size() > size) {
		commands.pop_back();
	}
	commands.push_front(p_message);
}
