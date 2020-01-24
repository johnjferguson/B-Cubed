#include "Keyboard.h"
#include <cassert>

bool Keyboard::IsKeyPressed(const unsigned char c) const
{
	return keyState[c];
}

void Keyboard::OnKeyPress(const unsigned char c)
{
	keyState[c] = true;
	eventQueue.emplace(c, Event::Type::Press);
	LimitQueue(eventQueue, maxElements);
	assert(eventQueue.size() <= maxElements && "keyboard event queue overflow");
}

void Keyboard::OnKeyRelease(const unsigned char c)
{
	keyState[c] = false;
	eventQueue.emplace(c, Event::Type::Release);
	LimitQueue(eventQueue, maxElements);
	assert(eventQueue.size() <= maxElements && "keyboard event queue overflow");
}

Keyboard::Event::Event()
	:
	type(Event::Type::Invalid)
{}

Keyboard::Event::Event(const unsigned char c, Event::Type type)
	:
	type(type),
	c(c)
{}
