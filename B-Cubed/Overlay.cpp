#include "Overlay.h"
#include "ScreenElement.h"
#include "Graphics.h"
#include <DirectXMath.h>

Overlay::Overlay(Graphics& gfx, float screenWidth, float screenHeight)
	:
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	b = { 0.0f,0.0f,0.0f,0.0f };

	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = true;
	bd.IndependentBlendEnable = true;
	ID3D11BlendState* bs;
	gfx.GetDevice()->CreateBlendState(&bd, &bs);

	gfx.GetContext()->OMSetBlendState(bs, b.data(), 0xffffffff);

	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.25f, -0.75f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar1.png"));
	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.25f, -0.75f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar2.png"));
	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.25f, -0.75f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar3.png"));
	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.25f, -0.75f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar4.png"));

	map = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.43f, -0.23f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\map.png");

	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.5f, 1.05f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\1outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.5f, 1.05f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\2outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.5f, 1.05f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\3outof3.png"));
}

void Overlay::Draw(Graphics & gfx, unsigned short numCharges, unsigned short nLap)
{
	chargeBar[numCharges]->Render(gfx);
	lapNumber[nLap]->Render(gfx);
	map->Render(gfx);
}
