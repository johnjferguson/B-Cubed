#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics & gfx, std::vector<unsigned short>& indices)
	:
	indexCount(indices.size())
{
	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = indices.data();

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(unsigned short) * indices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(unsigned short);

	HRESULT hr = gfx.GetDevice()->CreateBuffer(&bd, &srd, &pIndexBuffer);
	assert(SUCCEEDED(hr) && "index buffer bindable");
}

void IndexBuffer::Bind(Graphics & gfx)
{
	gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const
{
	return indexCount;
}
