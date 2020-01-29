#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "Bindable.h"
#include "TransformBuffer.h"       // i dont like this...

class IndexBuffer;
//class TransformBuffer;

class Renderable
{
public:
	Renderable() = default;
	void AddBind(std::unique_ptr<Bindable> bind);
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
private:
	std::vector<std::unique_ptr<Bindable>> bindables;
	VertexConstantBuffer* pVertexConstant = nullptr;
	PixelConstantBuffer* pPixelConstant = nullptr;
	IndexBuffer* pIndexBuffer = nullptr;
};
