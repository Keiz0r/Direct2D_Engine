#pragma once
#include "Lists\Animations.h"

struct AnimationData {
	AnimationData(const float& BatchStartx, const float& BatchStarty, const float& Width, const float& Height, const float& Stride,
		const unsigned short int& animationRows, const unsigned short int& FramesInARow, const unsigned short int& FrameTime)
		:
		Width(Width),
		Height(Height),
		FrameTime(FrameTime)
	{
		TotalFrames = FramesInARow * animationRows;
		float movingFramePoint = Width + Stride;
		float rowShift = 0.0f;
		for (int j = 0; j < animationRows; j++) {
			for (int i = 0; i < FramesInARow; i++) {
				frameCoords.emplace<D2D1_RECT_F>(frameCoords.begin() + i, { (i * movingFramePoint) + BatchStartx, BatchStarty + rowShift, ((i * movingFramePoint) + Width) + BatchStartx, BatchStarty + Height + rowShift });
			}
			rowShift += Height + 1.0f;
		}
	}
	const float Width;
	const float Height;
	const unsigned short int FrameTime;
	unsigned short int TotalFrames;
	std::vector<D2D1_RECT_F> frameCoords;
};