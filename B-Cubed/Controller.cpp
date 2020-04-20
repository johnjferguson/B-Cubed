#include "Controller.h"
#include <cassert>
#pragma comment(lib,"Xinput9_1_0.lib")

bool Controller::IsPressed(Controller::Button button, unsigned char slot)
{
	if (Update(slot))
	{
		switch (button)
		{
		case Controller::Button::A:
			return ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
			break;
		case Controller::Button::B:
			return ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
			break;
		case Controller::Button::Y:
			return ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
			break;
		case Controller::Button::X:
			return ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
			break;
		case Controller::Button::L_TRIGGER:
			return (unsigned char)state.Gamepad.bLeftTrigger > 0u;
			break;
		case Controller::Button::R_TRIGGER:
			return (unsigned char)state.Gamepad.bRightTrigger > 0u;
			break;
		}
	}
	return false;
}

DirectX::XMFLOAT2 Controller::GetLeftStick(unsigned char slot)
{
	Update(slot);
	float x = (float)state.Gamepad.sThumbLX / 32767;
	float y = (float)state.Gamepad.sThumbLY / 32767;

	DirectX::XMFLOAT2 output = (pow(x, 2) + pow(y, 2)) < deadzone_squared ? DirectX::XMFLOAT2(0.0f, 0.0f) : DirectX::XMFLOAT2(x, y);
	return output;
}

DirectX::XMFLOAT2 Controller::GetRightStick(unsigned char slot)
{
	Update(slot);
	float x = (float)state.Gamepad.sThumbRX / 32767;
	float y = (float)state.Gamepad.sThumbRY / 32767;

	DirectX::XMFLOAT2 output = (pow(x, 2) + pow(y, 2)) < deadzone_squared ? DirectX::XMFLOAT2(0.0f, 0.0f) : DirectX::XMFLOAT2(x, y);
	return output;
}

bool Controller::Update(unsigned char slot)
{
	state = {};
	unsigned long i = XInputGetState(slot, &state);
	if (i == -1)
	{
		return false;
	}
	return true;
}
