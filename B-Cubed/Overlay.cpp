#include "Overlay.h"
#include "ScreenElement.h"
#include "Graphics.h"
#include <DirectXMath.h>
#include <sstream>


Overlay::Overlay(Graphics& gfx, float screenWidth, float screenHeight)
	:
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	b = { 0.0f,0.0f,0.0f,0.0f };

	chargeBar = std::vector<std::unique_ptr<Renderable>>();
	nPlace = std::vector<std::unique_ptr<Renderable>>();

	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = true;
	bd.IndependentBlendEnable = true;
	ID3D11BlendState* bs;
	gfx.GetDevice()->CreateBlendState(&bd, &bs);

	gfx.GetContext()->OMSetBlendState(bs, b.data(), 0xffffffff);

	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, -0.8f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar1.png"));
	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, -0.8f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar2.png"));
	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, -0.8f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar3.png"));
	chargeBar.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, -0.8f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\bar4.png"));

	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.2f, 0.1f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place1.png"));
	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.2f, 0.1f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place4.png"));
	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.2f, 0.1f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place2.png"));
	nPlace.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(-0.2f, 0.1f), 2.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\place3.png"));

	map = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.70f,-0.60f), 1.2f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\map.png");

	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.8f, 0.5f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\1outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.8f, 0.5f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\2outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.8f, 0.5f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\3outof3.png"));
		
	vehicle = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 0.05f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\arrow.png");
	missile = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 0.05f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\missle.png");

}

void Overlay::Draw(Graphics& gfx, unsigned short numCharges, unsigned short nLap, int place, EntityManager& em, const std::vector<unsigned int>& entities)
{
	chargeBar[numCharges % 4]->Render(gfx);
	lapNumber[nLap % 3]->Render(gfx);
	map->Render(gfx);

	if (place > 0)
	{
		nPlace[place - 1]->Render(gfx);
	}

	struct VertexShaderInput
	{
		DirectX::XMFLOAT4 offset;
		DirectX::XMMATRIX transform;
	};

	int depth = 0;
	for (auto& ui : entities)
	{
		Entity* entity = em.Get(ui);
		if (entity != nullptr)
		{
			DirectX::XMFLOAT3 pos = entity->GetPosition();
			DirectX::XMMATRIX transform = entity->GetTransform();


			DirectX::XMVECTOR z = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			//DirectX::XMVECTOR y = transform.r[2];
			DirectX::XMVECTOR y = DirectX::XMVectorSet(entity->GetVelocity().x, entity->GetVelocity().y, entity->GetVelocity().z, 0.0f);
			y = DirectX::XMVector3Normalize(y);
			y = DirectX::XMVectorSetY(y, 0.0f);
			y = DirectX::XMVector3Normalize(y);
			y = DirectX::XMVectorSetY(y, DirectX::XMVectorGetZ(y));
			y = DirectX::XMVectorSetZ(y, 0.0f);
			DirectX::XMVECTOR x = DirectX::XMVector3Cross(y, z);
			DirectX::XMVECTOR w = DirectX::XMVectorSet(0.0f,0.0f,0.0f,0.0f);

			std::stringstream ss;
			ss << "x: " << DirectX::XMVectorGetX(x) << " , "<< DirectX::XMVectorGetY(x) << " ," << DirectX::XMVectorGetZ(x) << " ," << DirectX::XMVectorGetW(x) << "\n";
			ss << "y: " << DirectX::XMVectorGetX(y) << " , " << DirectX::XMVectorGetY(y) << " ," << DirectX::XMVectorGetZ(y) << " ," << DirectX::XMVectorGetW(y) << "\n";
			ss << "z: " << DirectX::XMVectorGetX(z) << " , " << DirectX::XMVectorGetY(z) << " ," << DirectX::XMVectorGetZ(z) << " ," << DirectX::XMVectorGetW(z) << "\n";
			ss << "w: " << DirectX::XMVectorGetX(w) << " , " << DirectX::XMVectorGetY(w) << " ," << DirectX::XMVectorGetZ(w) << " ," << DirectX::XMVectorGetW(w) << "\n";
			Gui::AddText(ss.str().c_str());

			VertexShaderInput vsi = { DirectX::XMFLOAT4(pos.x / 2000.0f + 0.80f, pos.z / 2000.0f - 0.70f,0.1 - 0.001*depth++ , 1.0f),
										DirectX::XMMATRIX(x,y,z,w)
			//DirectX::XMMatrixIdentity()
			};
			

			if (entity->GetType() == Entity::Type::VEHICLE)
			{
				vehicle->UpdateVertex(gfx, vsi);
				vehicle->Render(gfx);

			}
			else if (entity->GetType() == Entity::Type::MISSILE)
			{
				missile->UpdateVertex(gfx, vsi);
				missile->Render(gfx);
			}
		}
	}

}
