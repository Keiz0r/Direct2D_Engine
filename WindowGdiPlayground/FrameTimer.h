#pragma once
#include <chrono>
#include <string>
#include <inttypes.h>

class FrameTimer {
public:
	FrameTimer();
	float_t Mark();
	std::wstring getFPS();
private:
	std::chrono::steady_clock::time_point last;
	float_t fps = 0.0f;
	uint16_t framecounter = 0;
	float_t timecounter = 0.0f;
	std::wstring m_sdisplayfps = L"";
};