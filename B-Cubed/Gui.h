#pragma once
#include <Windows.h>
#include <string>

class Gui
{
public:
	Gui();
	~Gui();
	void Begin(const std::string& title);
	void End();
	void AddSlider(const std::string& name, float& value, float lowerbound, float upperbound);
	void AddText(const std::string& text);
	void AddCheckbox(const std::string& name, bool& state);
	void AddColor4(const std::string& name, float* pColor);
private:
};
