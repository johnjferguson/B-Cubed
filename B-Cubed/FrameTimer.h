#pragma once
#include <chrono>
#include "Time.h"

class FrameTimer
{
public:
	FrameTimer();
	Time Set();
private:
	std::chrono::steady_clock::time_point last;
};
