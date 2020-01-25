#include "Keyboard.h"
#include <cassert>

bool Keyboard::IsKeyPressed(const unsigned char c) const
{
	return keyState[c];
}

Keyboard::Event Keyboard::PopEvent()
{
	Event out;
	if (!IsEmpty())
	{
		out = eventQueue.front();
		eventQueue.pop();
	}
	else
	{
		assert(false && "keyboard event queue is empty call IsEmpty befor PopEvent");
	}
	return out;
}

bool Keyboard::IsEmpty() const
{
	return eventQueue.size() == 0;
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

Keyboard::Event::Type Keyboard::Event::GetType() const
{
	return type;
}

unsigned char Keyboard::Event::GetCharacter() const
{
	return c;
}
