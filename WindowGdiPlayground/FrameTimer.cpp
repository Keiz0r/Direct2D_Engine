#include "FrameTimer.h"

FrameTimer::FrameTimer()
{
	last = std::chrono::steady_clock::now();
}

float_t FrameTimer::Mark()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
    timecounter += frameTime.count();

    if (timecounter >= 1.0f) {
        fps = framecounter / timecounter;
        timecounter -= 1.0f;
        framecounter = 0;
    }
    m_sdisplayfps = L"fps: " + std::to_wstring(fps);
    framecounter++;

	return frameTime.count();
}

std::wstring FrameTimer::getFPS()
{
    return m_sdisplayfps;
}
