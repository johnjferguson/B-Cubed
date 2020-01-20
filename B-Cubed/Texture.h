#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const std::wstring& path);
	virtual void Bind(Graphics& gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};
