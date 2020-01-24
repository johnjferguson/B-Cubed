#pragma once
#include <queue>

class Mouse
{
	// allows window access to private data
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LeftPress,
			LeftRelease,
			RightPress,
			RightRelease,
			Move,
			Invalid
		};
	public:
		Event(Event::Type type, int x, int y);
	private:
		Type type;
		int x;
		int y;
	};
public:
	Mouse() = delete;
	Mouse(int width, int height);
	int PosX() const;
	int PosY() const;
	// Normal x:[-1,1] and y:[-1,1]
	std::pair<float, float> NormalPos() const;
	std::pair<int, int> Pos() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	void Flush();
private:
	void OnLeftPress(int x_in, int y_in);
	void OnLeftRelease(int x_in, int y_in);
	void OnRightPress(int x_in, int y_in);
	void OnRightRelease(int x_in, int y_in);
	void OnMove(int x_in, int y_in);
private:
	std::queue<Event> events;
	int x;
	int y;
	bool leftIsPressed;
	bool rightIsPressed;
	int width;
	int height;
};
