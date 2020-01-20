#pragma once
#include "Bindable.h"
#include <cstring>

class ConstantBuffer : public Bindable
{
public:
	template <class C>
	ConstantBuffer(Graphics& gfx, const C& data)
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(C);
		bd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = &data;
		HRESULT hr = gfx.GetDevice()->CreateBuffer(&bd, &srd, &pConstantBuffer);
		assert(SUCCEEDED(hr) && "constant buffer bindable");
	}
	virtual ~ConstantBuffer() = default;
	virtual void Bind(Graphics& gfx) override = 0;
	template <class C>
	void Update(Graphics& gfx, const C& data)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		gfx.GetContext()->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		memcpy(msr.pData, &data, sizeof(C));
		gfx.GetContext()->Unmap(pConstantBuffer.Get(), 0u);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

class VertexConstantBuffer : public ConstantBuffer
{
public:
	template <class C>
	VertexConstantBuffer(Graphics& gfx, const C& data)
		:
		ConstantBuffer(gfx, data)
	{}
	virtual void Bind(Graphics& gfx) override;
private:
};

class PixelConstantBuffer : public ConstantBuffer
{
public:
	template <class C>
	PixelConstantBuffer(Graphics& gfx, const C& data)
		:
		ConstantBuffer(gfx, data)
	{}
	virtual void Bind(Graphics& gfx) override;
private:
};