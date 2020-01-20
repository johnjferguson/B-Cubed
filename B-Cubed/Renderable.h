#pragma once
#include <vector>
#include <memory>
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
	void Update(Graphics& gfx, const C& data)
	{
		assert(pTransform != nullptr && "no transform buffer renderable");
		pTransformBuffer->Update(gfx, data);
	}
	void Render(Graphics& gfx);
private:
	std::vector<std::unique_ptr<Bindable>> bindables;
	IndexBuffer* pIndexBuffer = nullptr;
	TransformBuffer* pTransformBuffer = nullptr;
};
