#pragma once
#include <cassert>

class Time
{
public:
	enum class Type
	{
		Seconds,
		Milliseconds
	};
public:
	Time(float time, Time::Type type = Time::Type::Seconds);
	
	float Seconds() const;
	float Milliseconds() const;
	// easier way to get time in seconds: time()
	float operator()() const;
private:
	float seconds;
};
