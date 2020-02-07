#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class RenderTexture
{
public:
	RenderTexture(ID3D11Device* device, int width, int height, float near_plane, float far_plane);
	void SetRenderTarget(ID3D11DeviceContext* pContext);
	ID3D11ShaderResourceView*const* GetShaderResourceView();
	void ClearRenderTarget(ID3D11DeviceContext* pContext);
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	D3D11_VIEWPORT viewport;

	DirectX::XMMATRIX perspective;
	DirectX::XMMATRIX orthographic;
};