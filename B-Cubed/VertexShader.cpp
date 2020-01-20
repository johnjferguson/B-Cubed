#include "VertexShader.h"

VertexShader::VertexShader(Graphics & gfx, const std::wstring & path)
{
	HRESULT hr;
	hr = D3DReadFileToBlob(path.c_str(), &pBlob);
	assert(SUCCEEDED(hr) && "vertex shader blob bindable");
	hr = gfx.GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	assert(SUCCEEDED(hr) && "vertex shader bindable");
}

void VertexShader::Bind(Graphics & gfx)
{
	gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob * VertexShader::GetBlob()
{
	return pBlob.Get();
}
