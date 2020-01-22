#include "Entity.h"
#include "DirectXMath.h"
#include "Gui.h"

void Entity::AddRenderable(std::unique_ptr<Renderable> pRenderable)
{
	renderable = std::move(pRenderable);
}

void Entity::Render(Graphics & gfx)
{
	Gui::AddSlider("pitch", pitch, -2 * 3.14f, 2 * 3.14f);
	Gui::AddSlider("yaw", yaw, -2 * 3.14f, 2 * 3.14f);
	Gui::AddSlider("roll", roll, -2 * 3.14f, 2 * 3.14f);
	Gui::AddSlider("distance", distance, 0.0f, 40.0f);

	static float x = 0.0f;
	static float y = 0.0f;
	static float z = 0.0f;

	Gui::AddSlider("x", x, 0.0f, 20.0f);
	Gui::AddSlider("y", y, 0.0f, 20.0f);
	Gui::AddSlider("z", z, 0.0f, 20.0f);


	// temp
	struct Transform
	{
		DirectX::XMMATRIX transform;
	};
	const DirectX::XMVECTOR eye = DirectX::XMVectorSet(x, y, z, 0.0f);
	DirectX::XMVECTOR look = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	look = DirectX::XMVector4Transform( look, DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw));
	const DirectX::XMVECTOR target = DirectX::XMVectorAdd(eye, look);
	Transform transform =
	{
		DirectX::XMMatrixRotationRollPitchYaw(0.0f,0.0f,0.0f)*
		DirectX::XMMatrixTranslation(0.0f,0.0f, distance)*
		DirectX::XMMatrixLookAtLH(eye, target, up)*
		DirectX::XMMatrixPerspectiveLH(1.0f, float(gfx.GetHeight()) / float(gfx.GetWidth()),1.0f,400.0f)
	};
	renderable->Update(gfx, transform);
	renderable->Render(gfx);
}
