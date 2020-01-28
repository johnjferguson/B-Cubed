#include "Texture.h"
#include "DirectXTex/WICTextureLoader/WICTextureLoader.h"
#include <string>

#pragma comment( lib, "dxguid.lib")          // this is need for WIC Texture

Texture::Texture(Graphics & gfx, const std::wstring& widePath)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pResource;
	// load image from file
	HRESULT hr = DirectX::CreateWICTextureFromFile(gfx.GetDevice(), gfx.GetContext(), widePath.c_str(), pResource.GetAddressOf(), pTextureView.GetAddressOf());
	assert(SUCCEEDED(hr) && "WIC failed to load texture");
}

void Texture::Bind(Graphics & gfx)
{
	// might have to add more texture slots later
	gfx.GetContext()->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());
}
