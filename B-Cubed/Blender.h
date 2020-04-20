#pragma once
#include "Bindable.h"
#include <vector>

class Blender : public Bindable
{
public:
	Blender(Graphics& gfx, bool blending);
	void Bind(Graphics& gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlender;
	bool blending;
	std::vector<float> factors;
};