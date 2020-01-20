#pragma once
#include "ConstantBuffer.h"

class TransformBuffer : public Bindable
{
public:
	template <class C>
	TransformBuffer(Graphics& gfx, const C& data)
		:
		vertexBuffer(gfx, data)
	{}
	
	virtual void Bind(Graphics& gfx) override;
	
	template <class C>
	void Update(Graphics& gfx, const C& data)
	{
		vertexBuffer.Update(gfx, data);
	}
private:
	VertexConstantBuffer vertexBuffer;
};
