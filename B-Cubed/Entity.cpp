#include "Entity.h"
#include "DirectXMath.h"
#include "Gui.h"



void Entity::AddRenderable(std::unique_ptr<Renderable> pRenderable)
{
	renderable = std::move(pRenderable);
}

void Entity::Render(Graphics & gfx, const DirectX::XMMATRIX& camera)
{
	struct Transform
	{
		DirectX::XMMATRIX transform;
	};
	
	Transform transform =
	{
		DirectX::XMMatrixRotationRollPitchYaw(pitch,yaw,roll)*
		DirectX::XMMatrixTranslation(pos.x,pos.y, pos.z)*
		camera*
		DirectX::XMMatrixPerspectiveLH(1.0f, float(gfx.GetHeight()) / float(gfx.GetWidth()),1.0f,400.0f)
	};
	renderable->Update(gfx, transform);
	renderable->Render(gfx);
}

void Entity::SetPosition(float x, float y, float z)
{
	pos = DirectX::XMFLOAT3(x,y,z);
}
