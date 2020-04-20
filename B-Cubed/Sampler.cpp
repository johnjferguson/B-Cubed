#include "Sampler.h"

Sampler::Sampler(Graphics & gfx)
{
	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
	sd.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// these are needed to use mip levels
	sd.MipLODBias = 0.0f;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = gfx.GetDevice()->CreateSamplerState(&sd, &pSampleState);
	assert(SUCCEEDED && "sampler bindable");
}

void Sampler::Bind(Graphics & gfx)
{
	gfx.GetContext()->PSSetSamplers(0, 1, pSampleState.GetAddressOf());
}
