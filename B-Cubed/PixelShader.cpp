#include "PixelShader.h"

PixelShader::PixelShader(Graphics & gfx, const std::wstring & path)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(path.c_str(), &pBlob);
	assert(SUCCEEDED(hr) && "pixel shader blob bindable");
	hr = gfx.GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	assert(SUCCEEDED(hr) && "pixel shader bindable");
}

void PixelShader::Bind(Graphics & gfx)
{
	gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
