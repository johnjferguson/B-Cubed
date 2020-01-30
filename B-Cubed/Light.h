#pragma once
#include <DirectXMath.h>
#include "LightBox.h"
#include "Graphics.h"
#include "Camera.h"

class Light
{
public:
	Light() = delete;
	Light(Graphics& gfx, const DirectX::XMFLOAT4& pos);
	const DirectX::XMFLOAT4& GetPosition() const;
	void Update(Graphics& gfx, const DirectX::XMMATRIX& camera);
	void Render(Graphics& gfx);
	void SetPosition(const DirectX::XMFLOAT4& newPos);
private:
	DirectX::XMFLOAT4 pos;
	LightBox lightBox;
};