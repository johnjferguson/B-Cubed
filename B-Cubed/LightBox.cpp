#include "LightBox.h"
#include <vector>
#include "BindableHeader.h"
#include "Geometry.h"

namespace dx = DirectX;

LightBox::LightBox(Graphics & gfx, float side)
{
	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};

	IndexedVertexList<Vertex> ivl = Geometry::MakeLightBox<Vertex>(side);

	AddBind(std::make_unique<VertexBuffer>(gfx, ivl.vertices));
	AddBind(std::make_unique<IndexBuffer>(gfx, ivl.indices));

	std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShaderBasic.cso");
	ID3DBlob* vertexBlob = pVertexShader->GetBlob();
	AddBind(std::move(pVertexShader));

	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShaderBasic.cso"));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_unique<InputLayout>(gfx, vertexBlob, ied));

	struct VertexConstant
	{
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX perspective;
	}vertexConstant;

	// constant buffers
	AddBind(std::make_unique<VertexConstantBuffer>(gfx, vertexConstant));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}
