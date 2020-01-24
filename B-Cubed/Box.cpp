#include "Box.h"
#include <vector>
#include "BindableHeader.h"
#include "Geometry.h"

namespace dx = DirectX;

Box::Box(Graphics & gfx, DirectX::XMFLOAT3 lwh, const std::wstring& texture)
{
	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT2 tex;
	};

	IndexedVertexList<Vertex> ivl = Geometry::MakeBoxIndependentFaces<Vertex>(lwh.x, lwh.y, lwh.z);

	struct Transform
	{
		dx::XMMATRIX transform;
	};

	AddBind(std::make_unique<VertexBuffer>(gfx, ivl.vertices));
	AddBind(std::make_unique<IndexBuffer>(gfx, ivl.indices));
	AddBind(std::make_unique<Texture>(gfx, texture.c_str()));

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
	AddBind(std::make_unique<Sampler>(gfx));
}
