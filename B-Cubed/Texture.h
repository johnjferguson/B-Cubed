#pragma once
#include "Bindable.h"
#include <DirectXMath.h>

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const std::wstring& path);
	virtual void Bind(Graphics& gfx) override;
	const DirectX::XMFLOAT2 GetDimensions() const;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	DirectX::XMFLOAT2 dimensions;
};
