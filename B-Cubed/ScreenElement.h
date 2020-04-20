#pragma once

#include "Renderable.h"
#include <DirectXMath.h>
#include <string>

class ScreenElement : public Renderable
{
public:
	// lwh is the length width and height of box
	ScreenElement(Graphics& gfx, const DirectX::XMFLOAT2& topLeft, float scale, const DirectX::XMFLOAT2& screenwidthHeight, const std::wstring& texturePath);
private:
};
