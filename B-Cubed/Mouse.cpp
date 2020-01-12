#include "Mouse.h"

Mouse::Event::Event(Event::Type type, int x, int y)
	:
	x(x),
	y(y),
	type(type)
{}

Mouse::Mouse()
	:
	x(0),
	y(0),
	leftIsPressed(false),
	rightIsPressed(false)
{}

int Mouse::PosX() const
{
	return x;
}

int Mouse::PosY() const
{
	return y;
}

std::pair<int, int> Mouse::Pos() const
{
	return { x,y };
}

bool Mouse::LeftIsPressed() const
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const
{
	return rightIsPressed;
}

void Mouse::Flush()
{
	events = std::queue<Event>();
}

void Mouse::OnLeftPress(int x_in, int y_in)
{
	events.emplace(Event::Type::LeftPress, x_in, y_in);
	leftIsPressed = true;
	x = x_in;
	y = y_in;
}

void Mouse::OnLeftRelease(int x_in, int y_in)
{
	events.emplace(Event::Type::LeftRelease, x_in, y_in);
	leftIsPressed = false;
	x = x_in;
	y = y_in;
}

void Mouse::OnRightPress(int x_in, int y_in)
{
	events.emplace(Event::Type::RightPress, x_in, y_in);
	rightIsPressed = true;
	x = x_in;
	y = y_in;
}

void Mouse::OnRightRelease(int x_in, int y_in)
{
	events.emplace(Event::Type::RightRelease, x_in, y_in);
	rightIsPressed = false;
	x = x_in;
	y = y_in;
}

void Mouse::OnMove(int x_in, int y_in)
{
	events.emplace(Event::Type::Move, x_in, y_in);
	x = x_in;
	y = y_in;
}
