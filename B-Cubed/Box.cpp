#include "Box.h"
#include <DirectXMath.h>
#include <vector>
#include "BindableHeader.h"

namespace dx = DirectX;

Box::Box(Graphics & gfx)
{
	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT2 tex;
	};

	std::vector<Vertex> vertices =
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
	int i = vertices.size();

	std::vector<unsigned short> indices
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

	struct Transform
	{
		dx::XMMATRIX transform;
	};

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	AddBind(std::make_unique<IndexBuffer>(gfx, indices));
	AddBind(std::make_unique<Texture>(gfx, L"images//dice.jpg"));

	std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShaderTexture.cso");
	ID3DBlob* vertexBlob = pVertexShader->GetBlob();
	AddBind(std::move(pVertexShader));

	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShaderTexture.cso"));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_unique<InputLayout>(gfx, vertexBlob, ied));

	Transform transform = { 
		dx::XMMatrixIdentity() 
	};

	AddBind(std::make_unique<TransformBuffer>(gfx, transform));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}
