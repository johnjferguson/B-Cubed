#include "Sampler.h"

Sampler::Sampler(Graphics & gfx)
{
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	HRESULT hr = gfx.GetDevice()->CreateSamplerState(&sd, &pSampleState);
	assert(SUCCEEDED && "sampler bindable");
}

void Sampler::Bind(Graphics & gfx)
{
	gfx.GetContext()->PSSetSamplers(0, 1, pSampleState.GetAddressOf());
}
