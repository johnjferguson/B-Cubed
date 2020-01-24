#include "Mouse.h"

Mouse::Event::Event(Event::Type type, int x, int y)
	:
	x(x),
	y(y),
	type(type)
{}

Mouse::Mouse(int width, int height)
	:
	x(0),
	y(0),
	leftIsPressed(false),
	rightIsPressed(false),
	width(width),
	height(height)
{}

int Mouse::PosX() const
{
	return x;
}

int Mouse::PosY() const
{
	return y;
}

std::pair<float, float> Mouse::NormalPos() const
{
	return { (float(x)*2.0f / float(width)) - 1.0f, 1.0f - (float(y)*2.0f / float(height)) };
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
