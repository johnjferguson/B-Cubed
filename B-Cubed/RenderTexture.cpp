#include "RenderTexture.h"
#include <cassert>
#include <Windows.h>

RenderTexture::RenderTexture(ID3D11Device * device, int width, int height, float near_plane, float far_plane)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC td = {};

	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	hr = device->CreateTexture2D(&td, nullptr, &pTexture);
	assert(SUCCEEDED(hr) && "texture2d failed RenderTexture");

	D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};

	rtvd.Format = td.Format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(pTexture.Get(), &rtvd, &pRenderTargetView);
	assert(SUCCEEDED(hr) && "render target view failed RenderTexture");


	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = td.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(pTexture.Get(), &srvd, &pSRV);
	assert(SUCCEEDED(hr) && "shader resource view failed RenderTexture");

	D3D11_TEXTURE2D_DESC dbd = {};

	dbd.Width = width;
	dbd.Height = height;
	dbd.MipLevels = 1;
	dbd.ArraySize = 1;
	dbd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dbd.SampleDesc.Count = 1;
	dbd.SampleDesc.Quality = 0;
	dbd.Usage = D3D11_USAGE_DEFAULT;
	dbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dbd.CPUAccessFlags = 0;
	dbd.MiscFlags = 0;

	hr = device->CreateTexture2D(&dbd, nullptr, &pDepthStencilBuffer);
	assert(SUCCEEDED(hr) && "depth stencil buffer failed RenderTexture");


	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};

	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(pDepthStencilBuffer.Get(), &dsvd, &pDepthStencilView);
	assert(SUCCEEDED(hr) && "depth stencil view failed RenderTexture");

	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	perspective = DirectX::XMMatrixPerspectiveLH(1.0f, float(height) / float(width), near_plane, far_plane);
	orthographic = DirectX::XMMatrixOrthographicLH(width, height, near_plane, far_plane);

}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext * pContext)
{
	pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());

	pContext->RSSetViewports(1, &viewport);
}

ID3D11ShaderResourceView*const* RenderTexture::GetShaderResourceView()
{
	return pSRV.GetAddressOf();
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext * pContext)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
