#include "ScreenElement.h"

#include <vector>
#include "BindableHeader.h"
#include "Geometry.h"

namespace dx = DirectX;

ScreenElement::ScreenElement(Graphics& gfx, const DirectX::XMFLOAT2& tl, float scale, const DirectX::XMFLOAT2& swh, const std::wstring& texturePath)
{

	struct Vertex
	{
		dx::XMFLOAT2 pos;
		dx::XMFLOAT2 tex;
	};

	std::unique_ptr<Texture> texture = std::make_unique<Texture>(gfx, texturePath.c_str());
	DirectX::XMFLOAT2 dimensions = texture->GetDimensions();
	AddBind(std::move(texture));

	float width;
	float height;

	width = dimensions.x > dimensions.y ? 1.0f : dimensions.x / dimensions.y;
	height = dimensions.y > dimensions.x ? 1.0f : dimensions.y / dimensions.x;

	width *= scale;
	height *= scale;

	float halfStepX = (width * swh.y / swh.x) / 2.0f;
	float halfStepY = height / 2.0f;


	IndexedVertexList<Vertex> ivl;
	ivl.vertices =
	{
		{{- halfStepX, halfStepY},{0.0f,0.0f}},
		{{halfStepX, halfStepY},{1.0f,0.0f}},
		{{halfStepX,- halfStepY},{1.0f,1.0f}},
		{{- halfStepX,- halfStepY},{0.0f,1.0f}}

	};

	ivl.indices = 
	{
		0,1,2,
		0,2,3
	};

	AddBind(std::make_unique<VertexBuffer>(gfx, ivl.vertices));
	AddBind(std::make_unique<IndexBuffer>(gfx, ivl.indices));

	std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShaderOverlay.cso");
	ID3DBlob* vertexBlob = pVertexShader->GetBlob();
	AddBind(std::move(pVertexShader));

	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShaderOverlay.cso"));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_unique<InputLayout>(gfx, vertexBlob, ied));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	struct VertexShaderInput
	{
		DirectX::XMFLOAT4 offset;
		DirectX::XMMATRIX rotation;
	};
	VertexShaderInput vsi = { DirectX::XMFLOAT4(tl.x,tl.y,0.11f,1.0f), DirectX::XMMatrixIdentity() };

	AddBind(std::make_unique<VertexConstantBuffer>(gfx,vsi));

	AddBind(std::make_unique<Sampler>(gfx));
	AddBind(std::make_unique<Blender>(gfx, true));
}
