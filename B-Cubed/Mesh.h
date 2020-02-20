#pragma once
#include "Renderable.h"
#include <DirectXMath.h>
#include <string>

class Mesh : public Renderable
{
public:
	Mesh(Graphics& gfx, float scale, const std::string& path);
private:
};
