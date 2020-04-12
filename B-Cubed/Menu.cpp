#include "Menu.h"
#include "ScreenElement.h"
#include "Graphics.h"
#include "Sound.h"
#include "Physics.h"
#include <DirectXMath.h>
#include <sstream>

using namespace physx;

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

	end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\ending_screen.png");

	load = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\loading_screen.png");
}

void Menu::Draw(Graphics& gfx, Controller& ctlr, std::vector<int> finish_order)
{
	if (race_finished) {

		player_1 = false;
		player_2 = false;
		player_3 = false;
		player_4 = false;
		
		int p1 = finish_order[0];
		int p2 = finish_order[1];
		int p3 = finish_order[2];
		int p4 = finish_order[3];

		ScoreBoard(gfx, finish_order);

		end->Render(gfx);

		race_finished = false;
		end_screen = true;

	}
	else if (end_screen) {

		end->Render(gfx);

		if (ctlr.IsPressed(Controller::Button::X, 0) || ctlr.IsPressed(Controller::Button::X, 1) ||
			ctlr.IsPressed(Controller::Button::X, 2) || ctlr.IsPressed(Controller::Button::X, 3)) {

			finish_order.clear();
			end_screen = false;
			Sound::Play("sounds//start.wav", 1.0f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);

			a_1_released = false;
			a_2_released = false;
			a_3_released = false;
			a_4_released = false;
		}
	}
	else {

		if (first_load) {


			start->Render(gfx);

			if (play_start) {
				Sound::Play("sounds//win.wav", 1.3f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);

				play_start = false;
			}

			if (ctlr.IsPressed(Controller::Button::R_TRIGGER) || ctlr.IsPressed(Controller::Button::L_TRIGGER) ||
				ctlr.IsPressed(Controller::Button::X) || ctlr.IsPressed(Controller::Button::Y) ||
				ctlr.IsPressed(Controller::Button::A) || ctlr.IsPressed(Controller::Button::B) ||
				ctlr.IsPressed(Controller::Button::R_TRIGGER, 1) || ctlr.IsPressed(Controller::Button::L_TRIGGER, 1) ||
				ctlr.IsPressed(Controller::Button::X, 1) || ctlr.IsPressed(Controller::Button::Y, 1) ||
				ctlr.IsPressed(Controller::Button::A, 1) || ctlr.IsPressed(Controller::Button::B, 1) || 
				ctlr.IsPressed(Controller::Button::R_TRIGGER, 2) || ctlr.IsPressed(Controller::Button::L_TRIGGER, 2) ||
				ctlr.IsPressed(Controller::Button::X, 2) || ctlr.IsPressed(Controller::Button::Y, 2) ||
				ctlr.IsPressed(Controller::Button::A, 2) || ctlr.IsPressed(Controller::Button::B, 2) || 
				ctlr.IsPressed(Controller::Button::R_TRIGGER, 3) || ctlr.IsPressed(Controller::Button::L_TRIGGER, 3) ||
				ctlr.IsPressed(Controller::Button::X, 3) || ctlr.IsPressed(Controller::Button::Y, 3) ||
				ctlr.IsPressed(Controller::Button::A, 3) || ctlr.IsPressed(Controller::Button::B, 3)) {

				first_load = false;
				Sound::Play("sounds//start.wav", 1.2f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				Sound::Play("sounds//BackgroundLoop.wav", 0.1f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), true);

			}
		}
		else {

			if (ctlr.IsPressed(Controller::Button::A, 0)  && a_1_released) {
				Sound::Play("sounds//lock_in.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_1 = true;
				a_1_released = false;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 0) && b_1_released) {
				Sound::Play("sounds//unlock.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_1 = false;
				b_1_released = false;
			}

			if (ctlr.IsPressed(Controller::Button::A, 1) && a_2_released) {
				Sound::Play("sounds//lock_in.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_2 = true;
				a_2_released = false;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 1) && b_2_released) {
				Sound::Play("sounds//unlock.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_2 = false;
				b_2_released = false;
			}

			if (ctlr.IsPressed(Controller::Button::A, 2) && a_3_released) {
				Sound::Play("sounds//lock_in.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_3 = true;
				a_3_released = false;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 2) && b_3_released) {
				Sound::Play("sounds//unlock.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_3 = false;
				b_3_released = false;
			}

			if (ctlr.IsPressed(Controller::Button::A, 3) && a_4_released) {
				Sound::Play("sounds//lock_in.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_4 = true;
				a_4_released = false;
			}
			else if (ctlr.IsPressed(Controller::Button::B, 3) && b_4_released) {
				Sound::Play("sounds//unlock.wav", 0.8f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), false);
				player_4 = false;
				b_4_released = false;
			}

			if (player_1 && (ctlr.IsPressed(Controller::Button::X, 0))) {
				StartGame = true;
				load->Render(gfx);
			}
			else if (player_2 && (ctlr.IsPressed(Controller::Button::X, 1))) {
				StartGame = true;
				load->Render(gfx);
			}
			else if (player_3 && (ctlr.IsPressed(Controller::Button::X, 2))) {
				StartGame = true;
				load->Render(gfx);
			}
			else if (player_4 && (ctlr.IsPressed(Controller::Button::X, 3))) {
				StartGame = true;
				load->Render(gfx);
			}

			if (!(ctlr.IsPressed(Controller::Button::A, 0))) {
				a_1_released = true;
			}
			if (!(ctlr.IsPressed(Controller::Button::A, 1))) {
				a_2_released = true;
			}
			if (!(ctlr.IsPressed(Controller::Button::A, 2))) {
				a_3_released = true;
			}
			if (!(ctlr.IsPressed(Controller::Button::A, 3))) {
				a_4_released = true;
			}

			if (!(ctlr.IsPressed(Controller::Button::B, 0))) {
				b_1_released = true;
			}
			if (!(ctlr.IsPressed(Controller::Button::B, 1))) {
				b_2_released = true;
			}
			if (!(ctlr.IsPressed(Controller::Button::B, 2))) {
				b_3_released = true;
			}
			if (!(ctlr.IsPressed(Controller::Button::B, 3))) {
				b_4_released = true;
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
	}
}

void Menu::ScoreBoard(Graphics& gfx, std::vector<int> finish_order)
{

	std::vector<int> finish;

	for (int i = 0; i < 4; i++) {
		if (finish_order[0] == i + 1) {
			finish.push_back(1);
		}
		if (finish_order[1] == i + 1) {
			finish.push_back(2);
		}
		if (finish_order[2] == i + 1) {
			finish.push_back(3);
		}
		if (finish_order[3] == i + 1) {
			finish.push_back(4);
		}
	}

	Sound::Play("sounds//BackgroundLoop.wav", 0.1f, PxVec3(0.f, 0.f, 0.f), PxVec3(0.f, 0.f, 0.f), true);

	if (finish[0] == 1 && finish[1] == 2 && finish[2] == 3 && finish[3] == 4) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_1234.png");
	}
	else if (finish[0] == 1 && finish[1] == 2 && finish[2] == 4 && finish[3] == 3) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_1243.png");
	}
	else if (finish[0] == 1 && finish[1] == 3 && finish[2] == 2 && finish[3] == 4) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_1324.png");
	}
	else if (finish[0] == 1 && finish[1] == 3 && finish[2] == 4 && finish[3] == 2) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_1342.png");
	}
	else if (finish[0] == 1 && finish[1] == 4 && finish[2] == 2 && finish[3] == 3) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_1423.png");
	}
	else if (finish[0] == 1 && finish[1] == 4 && finish[2] == 3 && finish[3] == 2) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_1432.png");
	}
	else if (finish[0] == 2 && finish[1] == 1 && finish[2] == 3 && finish[3] == 4) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_2134.png");
	}
	else if (finish[0] == 2 && finish[1] == 1 && finish[2] == 4 && finish[3] == 3) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_2143.png");
	}
	else if (finish[0] == 2 && finish[1] == 3 && finish[2] == 1 && finish[3] == 4) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_2314.png");
	}
	else if (finish[0] == 2 && finish[1] == 3 && finish[2] == 4 && finish[3] == 1) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_2341.png");
	}
	else if (finish[0] == 2 && finish[1] == 4 && finish[2] == 1 && finish[3] == 3) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_2413.png");
	}
	else if (finish[0] == 2 && finish[1] == 4 && finish[2] == 3 && finish[3] == 1) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_2431.png");
	}
	else if (finish[0] == 3 && finish[1] == 1 && finish[2] == 2 && finish[3] == 4) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_3124.png");
	}
	else if (finish[0] == 3 && finish[1] == 1 && finish[2] == 4 && finish[3] == 2) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_3142.png");
	}
	else if (finish[0] == 3 && finish[1] == 2 && finish[2] == 1 && finish[3] == 4) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_3214.png");
	}
	else if (finish[0] == 3 && finish[1] == 2 && finish[2] == 4 && finish[3] == 1) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_3241.png");
	}
	else if (finish[0] == 3 && finish[1] == 4 && finish[2] == 1 && finish[3] == 2) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_3412.png");
	}
	else if (finish[0] == 3 && finish[1] == 4 && finish[2] == 2 && finish[3] == 1) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_3421.png");
	}
	else if (finish[0] == 4 && finish[1] == 1 && finish[2] == 2 && finish[3] == 3) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_4123.png");
	}
	else if (finish[0] == 4 && finish[1] == 1 && finish[2] == 3 && finish[3] == 2) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_4132.png");
	}
	else if (finish[0] == 4 && finish[1] == 2 && finish[2] == 1 && finish[3] == 3) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_4213.png");
	}
	else if (finish[0] == 4 && finish[1] == 2 && finish[2] == 3 && finish[3] == 1) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_4231.png");
	}
	else if (finish[0] == 4 && finish[1] == 3 && finish[2] == 1 && finish[3] == 2) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_4312.png");
	}
	else if (finish[0] == 4 && finish[1] == 3 && finish[2] == 2 && finish[3] == 1) {
		end = std::make_unique<ScreenElement>(gfx, DirectX::XMFLOAT2(0.0f, 0.0f), 3.6f, DirectX::XMFLOAT2(screenWidth, screenHeight), L"images\\scoreboard\\score_4321.png");
	}
	
}
