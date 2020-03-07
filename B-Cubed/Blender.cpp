#include "Blender.h"

Blender::Blender(Graphics & gfx, bool blending)
	:
	blending(blending)
{
	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
	auto& brt = blendDesc.RenderTarget[0];

	for (int i = 0; i < 4; i++)
	{
		factors.push_back(0.5f);
	}
	if (blending)
	{
		brt.BlendEnable = TRUE;

		//brt.SrcBlend = D3D11_BLEND_BLEND_FACTOR;
		//brt.DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;

		brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	}

	gfx.GetDevice()->CreateBlendState(&blendDesc, &pBlender);
}

void Blender::Bind(Graphics & gfx)
{
	gfx.GetContext()->OMSetBlendState(pBlender.Get(), factors.data(), 0xFFFFFFFFu);
}
