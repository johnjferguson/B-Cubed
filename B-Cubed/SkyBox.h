#pragma once
#include "Renderable.h"
#include <DirectXMath.h>
#include <string>

class SkyBox : public Renderable
{
public:
	SkyBox(Graphics& gfx, float side, const std::wstring& texture);
private:
};