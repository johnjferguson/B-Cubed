#include "Menu.h"
#include "ScreenElement.h"
#include "Graphics.h"
#include <DirectXMath.h>
#include <sstream>

Menu::Menu(Graphics& gfx, float screenWidth, float screenHeight)
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

	map = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.70f, -0.60f), 1.2f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\map.png");

	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.8f, 0.5f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\1outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.8f, 0.5f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\2outof3.png"));
	lapNumber.push_back(std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.8f, 0.5f), 1.0f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\3outof3.png"));

	vehicle = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 0.05f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\arrow.png");
	missile = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 0.05f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\missle.png");


	start = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\starting_screen.png");

	p0 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\0_players.png");
	p1 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\1_players.png");
	p2 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\2_players.png");
	p3 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\3_players.png");
	p4 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\4_players.png");
	p12 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\12_players.png");
	p13 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\13_players.png");
	p14 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\14_players.png");
	p23 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\23_players.png");
	p24 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\24_players.png");
	p34 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\34_players.png");
	p123 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\123_players.png");
	p124 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\124_players.png");
	p134 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\134_players.png");
	p234 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\234_players.png");
	p1234 = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\1234_players.png");
}

void Menu::Draw(Graphics& gfx, Controller& ctlr, std::vector<int> finish_order)
{
	//if (finish_order.size() != 0) {
	if (false) {
		start->Render(gfx);

	}
	else {

		if (first_load) {
			start->Render(gfx);

			if (ctlr.IsPressed(Controller::Button::R_TRIGGER) || ctlr.IsPressed(Controller::Button::L_TRIGGER) ||
				ctlr.IsPressed(Controller::Button::X) || ctlr.IsPressed(Controller::Button::Y) ||
				ctlr.IsPressed(Controller::Button::A) || ctlr.IsPressed(Controller::Button::B)) {
				first_load = false;


				if (ctlr.IsPressed(Controller::Button::A, 0)) {
					a_1_pressed_start = true;
				}
				else if (ctlr.IsPressed(Controller::Button::A, 1)) {
					a_2_pressed_start = true;
				}
				else if (ctlr.IsPressed(Controller::Button::A, 2)) {
					a_3_pressed_start = true;
				}
				else if (ctlr.IsPressed(Controller::Button::A, 3)) {
					a_4_pressed_start = true;
				}
			}
		}
		else {

			if (ctlr.IsPressed(Controller::Button::A, 0) && !a_1_pressed_start) {
				player_1 = true;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 0)) {
				player_1 = false;
			}

			if (ctlr.IsPressed(Controller::Button::A, 1) && !a_2_pressed_start) {
				player_2 = true;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 1)) {
				player_2 = false;
			}

			if (ctlr.IsPressed(Controller::Button::A, 2) && !a_3_pressed_start) {
				player_3 = true;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 2)) {
				player_3 = false;
			}

			if (ctlr.IsPressed(Controller::Button::A, 3) && !a_4_pressed_start) {
				player_4 = true;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 3)) {
				player_4 = false;
			}

			if (player_1 && (ctlr.IsPressed(Controller::Button::X, 0))) {
				StartGame = true;
			}
			else if (player_2 && (ctlr.IsPressed(Controller::Button::X, 1))) {
				StartGame = true;
			}
			else if (player_3 && (ctlr.IsPressed(Controller::Button::X, 2))) {
				StartGame = true;
			}
			else if (player_4 && (ctlr.IsPressed(Controller::Button::X, 3))) {
				StartGame = true;
			}

			if (!player_1 && !player_2 && !player_3 && !player_4) {
				p0->Render(gfx);
			}
			else if (player_1 && !player_2 && !player_3 && !player_4) {
				p1->Render(gfx);
			}
			else if (!player_1 && player_2 && !player_3 && !player_4) {
				p2->Render(gfx);
			}
			else if (!player_1 && !player_2 && player_3 && !player_4) {
				p3->Render(gfx);
			}
			else if (!player_1 && !player_2 && !player_3 && player_4) {
				p4->Render(gfx);
			}
			else if (player_1 && player_2 && !player_3 && !player_4) {
				p12->Render(gfx);
			}
			else if (player_1 && !player_2 && player_3 && !player_4) {
				p13->Render(gfx);
			}
			else if (player_1 && !player_2 && !player_3 && player_4) {
				p14->Render(gfx);
			}
			else if (!player_1 && player_2 && player_3 && !player_4) {
				p23->Render(gfx);
			}
			else if (!player_1 && player_2 && !player_3 && player_4) {
				p24->Render(gfx);
			}
			else if (!player_1 && !player_2 && player_3 && !player_4) {
				p34->Render(gfx);
			}
			else if (player_1 && player_2 && player_3 && !player_4) {
				p123->Render(gfx);
			}
			else if (player_1 && player_2 && !player_3 && player_4) {
				p124->Render(gfx);
			}
			else if (!player_1 && player_2 && player_3 && player_4) {
				p234->Render(gfx);
			}
			else if (player_1 && !player_2 && player_3 && player_4) {
				p134->Render(gfx);
			}
			else if (player_1 && player_2 && player_3 && player_4) {
				p1234->Render(gfx);
			}
		}

		if (!ctlr.IsPressed(Controller::Button::A, 0)) {
			a_1_pressed_start = false;
		}
		else if (!ctlr.IsPressed(Controller::Button::A, 1)) {
			a_2_pressed_start = false;
		}
		else if (!ctlr.IsPressed(Controller::Button::A, 2)) {
			a_3_pressed_start = false;
		}
		else if (!ctlr.IsPressed(Controller::Button::A, 3)) {
			a_4_pressed_start = false;
		}
	}
}
