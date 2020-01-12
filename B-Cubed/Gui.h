#pragma once
#include <Windows.h>
#include <string>
// TODO imgui is a global state machine could make it into a singleton to avoid problems
// or set up some safety precautions
class Gui
{
public:
	Gui();
	~Gui();
	void Begin(const std::string& title);
	void End();
	static void AddSlider(const std::string& name, float& value, float lowerbound, float upperbound);
	static void AddText(const std::string& text);
	static void AddCheckbox(const std::string& name, bool& state);
	static void AddColor4(const std::string& name, float* pColor);
private:
};
