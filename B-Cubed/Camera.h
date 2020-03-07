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
	void ToggleInput();
	void WrapAngle(float& f);
private:
	const Keyboard* pKbd;
	const Mouse* pMouse;
	// just setting for better view of scene
	DirectX::XMVECTOR pos;
	float maxTurnSpeed = 2.0f;
	float mouseDeadzone = 0.05f;
	float moveSpeed = 100.0f;
	float pitch = -3.14f;
	float roll = 1.0f;
	bool inputEnabled = true;
};

class FollowCamera : public Camera
{
public:
	FollowCamera() = default;
	void SetTarget(const class Entity& target);
	virtual DirectX::XMMATRIX GetTransform(const Time& dt) override;
private:
	const class Entity* entity = nullptr;
	const float followZ = 15.0f;
	const float followY = 6.0f;
	DirectX::XMVECTOR previous_vz = DirectX::XMVectorZero();
	float radianPerSecond = 4.0f;
};
