#pragma once
#include <bitset>
#include <queue>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	public:
		Event();
		Event(const unsigned char c, Event::Type type);
	private:
		Event::Type type;
		unsigned char c;
	};
public:
	bool IsKeyPressed(const unsigned char c) const;
public:
	template <class T>
	static void LimitQueue(std::queue<T>& queue, int limit)
	{
		while (queue.size() > limit)
		{
			queue.pop();
		}
	}
private:
	void OnKeyPress(const unsigned char c);
	void OnKeyRelease(const unsigned char c);
private:
	std::bitset<256u> keyState;
	std::queue<Event> eventQueue;
	static constexpr int maxElements = 64;
};