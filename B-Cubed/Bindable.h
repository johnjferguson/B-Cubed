#pragma once
#include <assert.h>
#include "Graphics.h"
#include <d3d11.h>

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	// don't forget your virtual destructor...
	virtual ~Bindable() = default;
private:
};
