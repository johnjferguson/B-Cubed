#pragma once
#include "Bindable.h"
#include <vector>

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, ID3DBlob* pVertexBlob, const std::vector<D3D11_INPUT_ELEMENT_DESC>& descs);
	virtual void Bind(Graphics& gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};
