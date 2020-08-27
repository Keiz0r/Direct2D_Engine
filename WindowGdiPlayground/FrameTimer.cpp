#include "FrameTimer.h"

FrameTimer::FrameTimer()
{
	last = std::chrono::steady_clock::now();
}

float_t FrameTimer::Mark() {
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = last - old;
    timecounter += frameTime.count();

    if (timecounter >= 1.0f) {
        fps = framecounter / timecounter;
        timecounter -= 1.0f;
        framecounter = 0;
    }
    framecounter++;

	return frameTime.count();
}

float_t FrameTimer::getFPS() const{
    return fps;
}