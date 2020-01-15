#include "Graphics.h"
#include <DirectXMath.h>
#include <vector>
#include "imgui/imgui_impl_dx11.h"
#include "DirectXTex/WICTextureLoader/WICTextureLoader.h"

// use this way to link libraries
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")       // needed to compile shaders
#pragma comment( lib, "dxguid.lib")          // this is need for WIC Texture

// less typing while not losing too much clarity
namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics(HWND hwnd, unsigned int width, unsigned int height)
	:
	width(width),
	height(height)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	 wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	 pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	 pDevice->CreateRenderTargetView(
		 pBackBuffer.Get(),
		 nullptr,
		 &pTarget
	 );

	 // create z-buffer
	 D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	 dsDesc.DepthEnable = TRUE;
	 dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	 dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	 wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	 pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	 // bind depth state
	 pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	 // create depth stensil texture
	 wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	 D3D11_TEXTURE2D_DESC descDepth = {};
	 descDepth.Width = width;
	 descDepth.Height = height;
	 descDepth.MipLevels = 1u;
	 descDepth.ArraySize = 1u;
	 descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	 descDepth.SampleDesc.Count = 1u;
	 descDepth.SampleDesc.Quality = 0u;
	 descDepth.Usage = D3D11_USAGE_DEFAULT;
	 descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	 pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	 // create view of depth stensil texture
	 D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	 descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	 descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	 descDSV.Texture2D.MipSlice = 0u;
	 pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);

	 // bind depth stensil to pipeline
	 pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	 // configure viewport
	 D3D11_VIEWPORT vp;
	 vp.Width = float(width);
	 vp.Height = float(height);
	 vp.MinDepth = 0.0f;
	 vp.MaxDepth = 1.0f;
	 vp.TopLeftX = 0.0f;
	 vp.TopLeftY = 0.0f;
	 pContext->RSSetViewports(1u, &vp);

	 // init imgui
	 ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());


	 // this stuff goes here instead of draw test because loading images is expensive
	 // and I was getting only 30 fps
	 // image path
	 const wchar_t* path = L"images//dice.jpg";

	 wrl::ComPtr<ID3D11Resource> pResource;
	 // load image from file
	 HRESULT hr = DirectX::CreateWICTextureFromFile(pDevice.Get(), path, pResource.GetAddressOf(), pTextureView.GetAddressOf());
	 assert(SUCCEEDED(hr) && "WIC failed to load texture");
}

void Graphics::EndFrame()
{
	// swaps the front and back buffer  
	// _____    ____
	// | 1 | -> |  |
	// |0_2| <- |__|
	pSwap->Present(0u, 0u);
}

void Graphics::TestDraw(int x, int y)
{
	// static variables for testing only
	static float pitch = 0.0f; // rotation around y-axis
	static float yaw = 0.0f;   // rotation around z-axis
	static float roll = 0.0f;  // rotation around x-axis
	static float distance = 10.0f;
	static float triangleColor[4] = { 1.0f, 0.55f, 0.60f, 1.00f };
	const static float pi = 3.141529f;

	HRESULT hr;

	Gui::AddText("some random text");

	// clear and set background color
	FLOAT color[4] = { 0,1.0f,159.0f/255.0f,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	// clear z-buffer
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT2 tex;
	};

	struct Transform
	{
		dx::XMMATRIX transform;
	};

	Transform transform =
	{
		dx::XMMatrixRotationRollPitchYaw(pitch,yaw,roll)*
		dx::XMMatrixTranslation(0.0f,0.0f, distance)*
		dx::XMMatrixPerspectiveLH(1.0f, 9.0f/16.0f,1.0f,40.0f)
	};

	Gui::AddSlider("pitch", pitch, -2 * pi, 2 * pi);
	Gui::AddSlider("yaw", yaw, -2 * pi, 2 * pi);
	Gui::AddSlider("roll", roll, -2 * pi, 2 * pi);
	Gui::AddSlider("distance", distance, 0.0f, 40.0f);


	// vertices being sent to gpu to be drawn
	Vertex vertices[] =
	{
		{ dx::XMFLOAT3(-1.0f,1.0f,1.0f),dx::XMFLOAT2(0.5f,1.0f) },  // 0
		{ dx::XMFLOAT3(1.0f,1.0f,1.0f),dx::XMFLOAT2(0.75f,1.0f) },   // 1
		{ dx::XMFLOAT3(-1.0f,1.0f,-1.0f),dx::XMFLOAT2(0.5f,0.66f) }, // 2
		{ dx::XMFLOAT3(1.0f,1.0f,-1.0f),dx::XMFLOAT2(0.75f,0.66f) },  // 3
		{ dx::XMFLOAT3(-1.0f,-1.0f,-1.0f),dx::XMFLOAT2(0.5f,0.33f) },// 4
		{ dx::XMFLOAT3(1.0f,-1.0f,-1.0f),dx::XMFLOAT2(0.75f,0.33f) }, // 5
		{ dx::XMFLOAT3(-1.0f,-1.0f,1.0f),dx::XMFLOAT2(0.5f,0.0f) }, // 6
		{ dx::XMFLOAT3(1.0f,-1.0f,1.0f),dx::XMFLOAT2(0.75f,0.0f) },  // 7
		{ dx::XMFLOAT3(1.0f,1.0f,1.0f),dx::XMFLOAT2(1.0f,0.66f) },   // 8
		{ dx::XMFLOAT3(1.0f,-1.0f,1.0f),dx::XMFLOAT2(1.0f,0.33f) },  // 9
		{ dx::XMFLOAT3(-1.0f,1.0f,1.0f),dx::XMFLOAT2(0.25f,0.66f) },  // 10
		{ dx::XMFLOAT3(-1.0f,-1.0f,1.0f),dx::XMFLOAT2(0.25f,0.33f) }, // 11
		{ dx::XMFLOAT3(1.0f,1.0f,1.0f),dx::XMFLOAT2(0.0f,0.66f) },   // 12
		{ dx::XMFLOAT3(1.0f,-1.0f,1.0f),dx::XMFLOAT2(0.0f,0.33f) },  // 13
	};

	const unsigned short indices[]
	{
		// dice 1
		2,0,1,
		2,1,3,
		// dice 2
		11,10,2,
		11,2,4,
		// dice 3
		4,2,3,
		4,3,5,
		// dice 4
		13,12,10,
		13,10,11,
		// dice 5
		5,3,8,
		5,8,9,
		// dice 6
		6,4,5,
		6,5,7
	};
	
	// bind texture to pixel shader
	pContext->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());

	// create vertex buffer and bind to pipeline
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = vertices;

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(Vertex);

	hr = pDevice->CreateBuffer(&bd, &srd, &pVertexBuffer);
	assert(SUCCEEDED(hr));
	UINT stride = sizeof(Vertex);
	UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// create index buffer and bind to pipeline 
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
	srd.pSysMem = indices;

	bd.ByteWidth = sizeof(indices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(unsigned short);

	hr = pDevice->CreateBuffer(&bd, &srd, &pIndexBuffer);
	assert(SUCCEEDED(hr));
	stride = sizeof(unsigned short);
	offset = 0u;
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, offset);

	// blob for shaders
	wrl::ComPtr<ID3DBlob> pBlob;

	// create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShaderTexture.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	// bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// input (vertex) layout
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	// input layout wants blob from creation of vertex shader so it needs to be done first
	pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	pContext->IASetInputLayout(pInputLayout.Get());

	// create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShaderTexture.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	// bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	// sending 4 floats to the pixel shader
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = 4*sizeof(float);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = triangleColor;
	pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	// bind 4 floats to pixel shader
	pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// creating and binding tranform matrix to vertex shader
	Microsoft::WRL::ComPtr<ID3D11Buffer> pTransformConstantBuffer;

	D3D11_BUFFER_DESC tcbd;
	tcbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tcbd.Usage = D3D11_USAGE_DYNAMIC;
	tcbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tcbd.MiscFlags = 0u;
	tcbd.ByteWidth = sizeof(transform);
	tcbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA tcsd = {};
	tcsd.pSysMem = &transform;
	hr = pDevice->CreateBuffer(&tcbd, &tcsd, &pTransformConstantBuffer);
	assert(SUCCEEDED(hr) && "transform constant buffer failed");

	// bind transform matrix to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pTransformConstantBuffer.GetAddressOf());

	// variable color panel
	//Gui::AddColor4("<-(click) Triangle Color", triangleColor);

	// set primitive topology
	pContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw object
	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
	
}
