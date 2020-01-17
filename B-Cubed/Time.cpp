#include "Time.h"

Time::Time(float time, Time::Type type)
{
	if (type == Time::Type::Seconds)
		seconds = time;
	else if (type == Time::Type::Milliseconds)
		seconds = time / 1000.0f;
	else
	{
		assert(false && "invald time type");
	}
}

float Time::Seconds() const
{
	return seconds;
}

float Time::Milliseconds() const
{
	return seconds * 1000.0f;
}

float Time::operator()() const
{
	return seconds;
}
