#pragma once
#include <DirectXMath.h>
#include "Time.h"
#include "Window.h"

class Camera
{
public:
	virtual DirectX::XMMATRIX GetTransform(const Time& dt) = 0;
	
private:
};

class FreeCamera : public Camera
{
public:
	FreeCamera() = delete;
	FreeCamera(const Keyboard& kbd, const Mouse& mouse, const DirectX::XMFLOAT3& pos);
	virtual DirectX::XMMATRIX GetTransform(const Time& dt) override;
private:
	const Keyboard* pKbd;
	const Mouse* pMouse;
	DirectX::XMVECTOR pos;
	float maxTurnSpeed = 1.0f;
	float moveSpeed = 3.0f;
	float pitch = 0.0f;
	float roll = 0.0f;
};
