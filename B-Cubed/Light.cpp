#include "Light.h"

Light::Light(const DirectX::XMFLOAT4 & pos)
	:
	pos(pos)
{}

const DirectX::XMFLOAT4 & Light::GetPosition() const
{
	return pos;
}

void Light::SetPosition(const DirectX::XMFLOAT4 & newPos)
{
	pos = newPos;
}
