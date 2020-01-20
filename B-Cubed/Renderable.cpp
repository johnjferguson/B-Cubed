#include "Renderable.h"
#include "IndexBuffer.h"
#include "TransformBuffer.h"

void Renderable::AddBind(std::unique_ptr<Bindable> bind)
{
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert(pIndexBuffer == nullptr && "more then one index buffer");
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	else if (typeid(*bind) == typeid(TransformBuffer))
	{
		assert(pIndexBuffer == nullptr && "more then one index buffer");
		pTransformBuffer = &static_cast<TransformBuffer&>(*bind);
	}
	bindables.push_back(std::move(bind));
}

void Renderable::Render(Graphics & gfx)
{
	assert(pIndexBuffer != nullptr && "no index buffer renderable");
	for (auto& b : bindables)
	{
		b->Bind(gfx);
	}
	gfx.RenderIndexed(pIndexBuffer->GetCount());
}
