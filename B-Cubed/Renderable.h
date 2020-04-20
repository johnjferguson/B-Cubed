#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <DirectXMath.h>
#include "Bindable.h"
#include "TransformBuffer.h"       // i dont like this...

class IndexBuffer;
//class TransformBuffer;

class Renderable
{
public:
	Renderable() = default;
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddDepthBind(std::unique_ptr<Bindable> bind);
	const std::vector<DirectX::XMFLOAT3>& GetVertices() const;
	const std::vector<unsigned short>& GetIndices() const;
	template <class C>
	void UpdateVertex(Graphics& gfx, const C& data)
	{
		if (pVertexConstant != nullptr)
			pVertexConstant->Update(gfx, data);
	}
	template <class C>
	void UpdatePixel(Graphics& gfx, const C& data)
	{
		if (pPixelConstant != nullptr)
			pPixelConstant->Update(gfx, data);
	}
	void Render(Graphics& gfx);
	void RenderDepth(Graphics& gfx);
private:
	std::vector<std::unique_ptr<Bindable>> bindables;
	std::vector<std::unique_ptr<Bindable>> depthBindables;
	VertexConstantBuffer* pVertexConstant = nullptr;
	PixelConstantBuffer* pPixelConstant = nullptr;
	IndexBuffer* pIndexBuffer = nullptr;
protected:
	std::vector<DirectX::XMFLOAT3> m_vertices;
	std::vector<unsigned short> m_indices;
};
