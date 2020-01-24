#pragma once
#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics& gfx);
	virtual void Bind(Graphics& gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampleState;
};
