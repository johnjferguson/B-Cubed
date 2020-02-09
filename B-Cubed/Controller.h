#pragma once
#include <Windows.h>
#include <xinput.h>
#include <DirectXMath.h>

class Controller
{
public:
	enum class Button
	{
		A,
		B,
		Y,
		X,
		L_TRIGGER,
		R_TRIGGER
	};
public:
	Controller() = default;
	bool IsPressed(Controller::Button button, unsigned char slot = 0u);
	DirectX::XMFLOAT2 GetLeftStick(unsigned char slot = 0);
	DirectX::XMFLOAT2 GetRightStick(unsigned char slot = 0);
private:
	bool Update(unsigned char slot);
private:
	XINPUT_STATE state;
	constexpr static float deadzone_squared = 0.020f;
};
