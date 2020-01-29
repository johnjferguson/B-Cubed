#include "ConstantBuffer.h"

void VertexConstantBuffer::Bind(Graphics & gfx)
{
	gfx.GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}

void PixelConstantBuffer::Bind(Graphics & gfx)
{
	gfx.GetContext()->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
}


