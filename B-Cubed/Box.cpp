#include "Box.h"
#include <vector>
#include "BindableHeader.h"
#include "Geometry.h"

namespace dx = DirectX;

Box::Box(Graphics & gfx, DirectX::XMFLOAT3 lwh, const std::wstring& texture, Box::Type type)
{
	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 norm;
		dx::XMFLOAT2 tex;
	};

	IndexedVertexList<Vertex> ivl;
	if (type == Box::Type::Regular)
		ivl = Geometry::MakeBoxIndependentFaces<Vertex>(lwh.x, lwh.y, lwh.z);
	else if (type == Box::Type::Sky)
		ivl = Geometry::MakeSkyBox<Vertex>(lwh.x);

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
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_unique<InputLayout>(gfx, vertexBlob, ied));
	
	struct VertexConstant
	{
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX perspective;
		DirectX::XMMATRIX rollpitchyaw;
	}vertexConstant;

	struct PixelConstant
	{
		DirectX::XMFLOAT4 light;
	}pixelConstant;

	// constant buffers
	AddBind(std::make_unique<VertexConstantBuffer>(gfx, vertexConstant));
	AddBind(std::make_unique<PixelConstantBuffer>(gfx, pixelConstant));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<Sampler>(gfx));

	// depth stuff ----------------------------------------------------------------
	// ------------------------------------------------------------------------------
	AddDepthBind(std::make_unique<VertexBuffer>(gfx, ivl.vertices));

	std::unique_ptr<VertexShader> pVertexShaderDepth = std::make_unique<VertexShader>(gfx, L"VertexShaderDepth.cso");
	ID3DBlob* vertexBlobDepth = pVertexShaderDepth->GetBlob();
	AddDepthBind(std::move(pVertexShaderDepth));

	AddDepthBind(std::make_unique<PixelShader>(gfx, L"PixelShaderDepth.cso"));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied0 =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddDepthBind(std::make_unique<InputLayout>(gfx, vertexBlobDepth, ied0));

	AddDepthBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}
