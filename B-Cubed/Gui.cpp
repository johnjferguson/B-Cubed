#include "Gui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

Gui::Gui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

Gui::~Gui()
{
	ImGui::DestroyContext();
}

void Gui::Begin(const std::string& title)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin(title.c_str());
}

void Gui::End()
{
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Gui::AddSlider(const std::string & name, float & value, float lowerbound, float upperbound)
{
	ImGui::SliderFloat(name.c_str(), &value, lowerbound, upperbound);
}

void Gui::AddText(const std::string & text)
{
	ImGui::Text(text.c_str());
}

void Gui::AddCheckbox(const std::string & name, bool & state)
{
	ImGui::Checkbox(name.c_str(), &state);
}

void Gui::AddColor4(const std::string & name, float * pColor)
{
	// float has to point to a 4 element array [0,1,2,3]
	//                                          ^
	//                                       beginning of array
	ImGui::ColorEdit3(name.c_str(), pColor);
}
