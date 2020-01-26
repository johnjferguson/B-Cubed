#pragma once
#include "Renderable.h"
#include <DirectXMath.h>
#include <string>

class Box : public Renderable
{
public:
	enum class Type
	{
		Regular,
		Sky
	};
public:
	// lwh is the length width and height of box
	Box(Graphics& gfx, DirectX::XMFLOAT3 lwh, const std::wstring& texture, Box::Type type = Box::Type::Regular);
private:
};
