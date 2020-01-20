#pragma once
#include <vector>
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, std::vector<unsigned short>& indices);
	virtual void Bind(Graphics& gfx) override;
	UINT GetCount() const;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	UINT indexCount;
};
