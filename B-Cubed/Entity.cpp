#include "Entity.h"
#include "DirectXMath.h"
#include "Gui.h"



void Entity::AddRenderable(std::unique_ptr<Renderable> pRenderable)
{
	renderable = std::move(pRenderable);
}

void Entity::Render(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightView, const DirectX::XMMATRIX& lightProjection, const Light& light)
{
	struct VertexConstant
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX perspective;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProjection;
		DirectX::XMFLOAT4 lightPosition;
	};

	struct PixelConstant
	{
		DirectX::XMFLOAT4 light;
	};
	
	VertexConstant vc
	{
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z),
		camera,
		DirectX::XMMatrixPerspectiveLH(1.0f, float(gfx.GetHeight()) / float(gfx.GetWidth()), 1.0f, 200.0f),
		lightView,
		lightProjection,
		light.GetPosition()
	};

	PixelConstant pc
	{
		light.GetPosition()
	};


	renderable->UpdateVertex(gfx, vc);
	renderable->UpdatePixel(gfx, pc);

	renderable->Render(gfx);
}

void Entity::RenderDepth(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightProjection, const Light& light)
{
	struct VertexConstant
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX perspective;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProjection;
		DirectX::XMFLOAT4 lightPosition;
	};

	struct PixelConstant
	{
		DirectX::XMFLOAT4 light;
	};

	VertexConstant vc
	{
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z),
		camera,
		lightProjection
	};

	PixelConstant pc
	{
		light.GetPosition()
	};

	renderable->UpdateVertex(gfx, vc);
	renderable->UpdatePixel(gfx, pc);

	renderable->RenderDepth(gfx);
}

void Entity::SetPosition(float x, float y, float z)
{
	pos = DirectX::XMFLOAT3(x,y,z);
}
