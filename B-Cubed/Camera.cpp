#include "Camera.h"
#include "Gui.h"
#include <cassert>
#include "Entity.h"
#include "EntityManager.h"

namespace dx = DirectX;

FreeCamera::FreeCamera(const Keyboard & kbd, const Mouse & mouse, const dx::XMFLOAT3 & pos)
	:
	pKbd(&kbd),
	pMouse(&mouse),
	pos(dx::XMVectorSet(pos.x,pos.y,pos.z,1.0f))
{}

dx::XMMATRIX FreeCamera::GetTransform(const Time & dt)
{
	Gui::AddSlider("Move Speed", moveSpeed, 0.0f, 20.0f);
	Gui::AddSlider("Turn Speed", maxTurnSpeed, 0.0f, 10.0f);

	Gui::AddSlider("Camera roll", roll, -30.0f, 30.0f);
	Gui::AddSlider("Camera pitch)", pitch, -30.0f, 30.0f);

	Gui::AddSlider("Mouse Deadzone", mouseDeadzone, 0.0f, 1.0f);

	static const dx::XMVECTOR forward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static const dx::XMVECTOR up = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	const dx::XMVECTOR look = dx::XMVector4Transform(forward, dx::XMMatrixRotationRollPitchYaw(roll, pitch, 0.0f));
	
	dx::XMVECTOR right = dx::XMVector3Cross(up, look);
	right = dx::XMVector4Normalize(right);

	const dx::XMVECTOR strideForward = dx::XMVectorScale(look, moveSpeed*dt());
	const dx::XMVECTOR strideRight = dx::XMVectorScale(right, moveSpeed*dt());
	const dx::XMVECTOR strideUp = dx::XMVectorScale(up, moveSpeed*dt());

	// this way is not perfect if going in two or three directions you can move
	// faster then intended. would need to add all directions -> normalize -> scale

	if (inputEnabled)
	{
		const std::pair<float, float> mousePos = pMouse->NormalPos();
		const float magnitude = sqrt(pow(mousePos.first, 2) + pow(mousePos.second, 2));

		if (magnitude > mouseDeadzone)
		{
			pitch += mousePos.first*maxTurnSpeed*dt();
			roll -= mousePos.second*maxTurnSpeed*dt();
		}

		roll = std::clamp(roll, -dx::XM_PI / 2.1f, dx::XM_PI / 2.1f);
		WrapAngle(pitch);

		if (pKbd->IsKeyPressed('W'))
		{
			pos = dx::XMVectorAdd(pos, strideForward);
		}
		if (pKbd->IsKeyPressed('S'))
		{
			pos = dx::XMVectorSubtract(pos, strideForward);
		}
		if (pKbd->IsKeyPressed('D'))
		{
			pos = dx::XMVectorAdd(pos, strideRight);
		}
		if (pKbd->IsKeyPressed('A'))
		{
			pos = dx::XMVectorSubtract(pos, strideRight);
		}
		if (pKbd->IsKeyPressed(VK_SPACE))
		{
			if (pKbd->IsKeyPressed(VK_SHIFT))
			{
				pos = dx::XMVectorSubtract(pos, strideUp);
			}
			else
			{
				pos = dx::XMVectorAdd(pos, strideUp);
			}
		}
	}

	const dx::XMVECTOR target = dx::XMVectorAdd(pos, look);

	return dx::XMMatrixLookAtLH(pos, target, up);
}

void FreeCamera::ToggleInput()
{
	inputEnabled = !inputEnabled;
}

void FreeCamera::WrapAngle(float& angle)
{
	// there must be a more elegant way of doing this

	int i = floor(abs(angle) / dx::XM_2PI);

	if (i != 0)
	{
		if (angle > 0)
		{
			if (i % 2 == 0)
			{
				angle = angle - i * dx::XM_2PI;
			}
			else
			{
				angle = angle - (i + 1) * dx::XM_2PI;
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				angle = angle + i * dx::XM_2PI;
			}
			else
			{
				angle = angle + (i + 1) * dx::XM_2PI;
			}
		}
	}

}


FollowCamera::FollowCamera(EntityManager & entityManager, unsigned int targetId)
	:
	entityManager(&entityManager),
	targetId(targetId)
{
}

void FollowCamera::SetTarget(unsigned int targetId_in)
{
	targetId = targetId_in;
}

DirectX::XMMATRIX FollowCamera::GetTransform(const Time & dt)
{
	Gui::AddSlider("radians", radianPerSecond, 0.0f, 6.0f);

	if (entityManager->Get(targetId) != nullptr)
	{
		DirectX::XMFLOAT3 pos = entityManager->Get(targetId)->GetPosition();
		DirectX::XMMATRIX transform = entityManager->Get(targetId)->GetTransform();
	
		DirectX::XMVECTOR vz = transform.r[2];

		//DirectX::XMVECTOR lerp

		DirectX::XMVECTOR i = DirectX::XMVectorSubtract(vz, previous_vz);
		i = DirectX::XMVectorAdd(previous_vz, DirectX::XMVectorScale(i, radianPerSecond*dt()));
		DirectX::XMVECTOR r = DirectX::XMVector3Normalize(i);

		previous_vz = r;

		r = DirectX::XMVectorScale(r, -followZ);
		r = DirectX::XMVectorAdd(r, DirectX::XMVectorSet(0.0f, followY, 0.0f, 1.0f));
		r = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&pos), r);


		return DirectX::XMMatrixLookAtLH(r, DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}
	assert(false && "follow camera does not have a target");
	return DirectX::XMMatrixIdentity();
}
