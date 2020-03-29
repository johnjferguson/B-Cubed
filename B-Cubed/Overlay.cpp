#include "Overlay.h"
#include "ScreenElement.h"
#include "Graphics.h"
#include <DirectXMath.h>

Overlay::Overlay(Graphics& gfx, float screenWidth, float screenHeight, const std::vector<unsigned int>& vehicles, EntityManager& em)
	:
	screenWidth(screenWidth),
	screenHeight(screenHeight),
	pVId(&vehicles)
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

	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.7f,1.2f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place1.png"));
	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.7f,1.2f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place2.png"));
	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.7f,1.2f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place3.png"));
	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.7f,1.2f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place4.png"));

	map = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.43f, -0.23f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\map.png");

	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.5f, 1.05f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\1outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.5f, 1.05f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\2outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.5f, 1.05f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\3outof3.png"));

	for (auto& v : *pVId)
	{
		Entity* entity = em.Get(v);
		if (entity != nullptr)
		{
			DirectX::XMFLOAT3 pos = entity->GetPosition();
			arrows.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(pos.x / 1500.0f, pos.z / 1500.0f), 0.1f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\arrow.png"));
		}
	}

}

void Overlay::Draw(Graphics& gfx, unsigned short numCharges, unsigned short nLap, int place, EntityManager& em)
{
	chargeBar[numCharges % 4]->Render(gfx);
	lapNumber[nLap % 3]->Render(gfx);
	map->Render(gfx);

	if (place > 0)
	{
		int j;
		nPlace[place - 1]->Render(gfx);
	}

	struct VertexShaderInput
	{
		DirectX::XMFLOAT4 offset;
	};

	for (int i = 0; i < arrows.size(); i++)
	{
		Entity* entity = em.Get((*pVId)[i]);
		if (entity != nullptr)
		{
			DirectX::XMFLOAT3 pos = entity->GetPosition();
			VertexShaderInput vsi = { DirectX::XMFLOAT4(pos.x / 1500.0f + 0.75f, pos.z / 1500.0f - 0.70f,0.1 - 0.001*i , 1.0f) };
			arrows[i]->UpdateVertex(gfx, vsi);
		}
		arrows[i]->Render(gfx);
	}

}
