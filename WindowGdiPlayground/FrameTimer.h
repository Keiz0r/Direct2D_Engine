#pragma once
#include <chrono>
#include <string>

class FrameTimer {
public:
	FrameTimer();
	float Mark();
	std::wstring getFPS();
private:
	std::chrono::steady_clock::time_point last;
	float fps = 0.0f;
	int framecounter = 0;
	float timecounter = 0.0f;
	std::wstring m_sdisplayfps = L"";
};