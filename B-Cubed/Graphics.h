#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Gui.h"


class Graphics
{
public:
	Graphics() = default;
	Graphics(HWND hwnd, unsigned int width, unsigned int height);
	void EndFrame();
	void TestDraw(int x, int y);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
private:
	int width;
	int height;
};