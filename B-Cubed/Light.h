#pragma once
#include <DirectXMath.h>

class Light
{
public:
	Light() = delete;
	Light(const DirectX::XMFLOAT4& pos);
	const DirectX::XMFLOAT4& GetPosition() const;
	void SetPosition(const DirectX::XMFLOAT4& newPos);
private:
	DirectX::XMFLOAT4 pos;
};