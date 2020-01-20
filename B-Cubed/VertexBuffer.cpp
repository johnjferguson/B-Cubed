#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics & gfx)
{
	static UINT offset = 0u;
	gfx.GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
