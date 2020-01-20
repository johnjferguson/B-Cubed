#pragma once
#include <vector>
#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template <class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		:
		stride(sizeof(V))
	{
		D3D11_SUBRESOURCE_DATA srd;
		srd.pSysMem = vertices.data();

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(V) * vertices.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizeof(V);

		HRESULT hr = gfx.GetDevice()->CreateBuffer(&bd, &srd, &pVertexBuffer);
		assert(SUCCEEDED(hr) && "vertex buffer bindable");
	}
	virtual void Bind(Graphics& gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT stride;
};