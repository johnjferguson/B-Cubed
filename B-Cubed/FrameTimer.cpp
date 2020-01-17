#include "FrameTimer.h"

FrameTimer::FrameTimer()
{
	last = std::chrono::steady_clock::now();
}

Time FrameTimer::Set()
{
	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	const std::chrono::duration<float> duration = current - last;
	last = current;
	return duration.count();
}
