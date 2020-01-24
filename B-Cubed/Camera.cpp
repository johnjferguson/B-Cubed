#include "Camera.h"

namespace dx = DirectX;

FreeCamera::FreeCamera(const Keyboard & kbd, const Mouse & mouse, const dx::XMFLOAT3 & pos)
	:
	pKbd(&kbd),
	pMouse(&mouse),
	pos(dx::XMVectorSet(pos.x,pos.y,pos.z,1.0f))
{}

dx::XMMATRIX FreeCamera::GetTransform(const Time & dt)
{
	static const dx::XMVECTOR forward = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static const dx::XMVECTOR up = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	if (pKbd->IsKeyPressed(VK_SPACE))
	{
		const std::pair<float, float> mousePos = pMouse->NormalPos();
		// should clamp these values
		pitch += mousePos.first*maxTurnSpeed*dt();
		roll -= mousePos.second*maxTurnSpeed*dt();
	}

	const dx::XMVECTOR look = dx::XMVector4Transform(forward, dx::XMMatrixRotationRollPitchYaw(roll, pitch, 0.0f));
	//const dx::XMVECTOR stride = dx::XMVectorScale(look, moveSpeed*dt());
	//pos = dx::XMVectorAdd(pos, stride);

	const dx::XMVECTOR target = dx::XMVectorAdd(pos, look);

	return dx::XMMatrixLookAtLH(pos, target, up);
}
