#include "InputLayout.h"

InputLayout::InputLayout(Graphics & gfx, ID3DBlob * pVertexBlob, const std::vector<D3D11_INPUT_ELEMENT_DESC>& descs)
{
	int i = (UINT)descs.size();
	HRESULT hr = gfx.GetDevice()->CreateInputLayout(descs.data(), (UINT)descs.size(), pVertexBlob->GetBufferPointer(), pVertexBlob->GetBufferSize(), &pInputLayout);
	assert(SUCCEEDED(hr) && "input layout bindable");
}

void InputLayout::Bind(Graphics & gfx)
{
	gfx.GetContext()->IASetInputLayout(pInputLayout.Get());
}
