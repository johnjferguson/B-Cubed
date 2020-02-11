#pragma once
#include <DirectXMath.h>

class PhysicsComponent
{
public:
	virtual ~PhysicsComponent() = default;
	virtual void Update(DirectX::XMFLOAT3& pos, DirectX::XMMATRIX& transform ) = 0;
private:
};