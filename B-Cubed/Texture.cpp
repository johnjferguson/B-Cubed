#include "Texture.h"
#include "DirectXTex/WICTextureLoader/WICTextureLoader.h"
#include <string>
#include <dxgiformat.h>
#include <assert.h>
#include <wincodec.h>


#pragma comment( lib, "dxguid.lib")          // this is need for WIC Texture

using namespace DirectX;
using Microsoft::WRL::ComPtr;

Texture::Texture(Graphics & gfx, const std::wstring& widePath)
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pResource;
	// load image from file
	HRESULT hr = DirectX::CreateWICTextureFromFile(gfx.GetDevice(), gfx.GetContext(), widePath.c_str(), pResource.GetAddressOf(), pTextureView.GetAddressOf());
	assert(SUCCEEDED(hr) && "WIC failed to load texture");

	ID3D11Texture2D* pTextureInterface;
	pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc(&desc);

	dimensions = XMFLOAT2(desc.Width, desc.Height);
}

void Texture::Bind(Graphics & gfx)
{
	// might have to add more texture slots later
	gfx.GetContext()->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());
}

const DirectX::XMFLOAT2 Texture::GetDimensions() const
{
	return dimensions;
}
