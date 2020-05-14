#pragma once
#include "Animations.h"

struct AnimationData {
	AnimationData(const float& BatchStartx, const float& BatchStarty, const float& Width, const float& Height, const float& Stride,
		const unsigned short int& TotalFrames, const unsigned short int& FrameTime)
		:
		BatchStartx(BatchStartx),
		BatchStarty(BatchStarty),
		Width(Width),
		Height(Height),
		Stride(Stride),
		TotalFrames(TotalFrames),
		FrameTime(FrameTime)
	{
	}
	const float BatchStartx;
	const float BatchStarty;
	const float Width;
	const float Height;
	const float Stride;
	const unsigned short int TotalFrames;
	const unsigned short int FrameTime;
};
