#include "Entity.h"
#include "DirectXMath.h"
#include "Gui.h"



Entity::Entity()
	:
	transform(DirectX::XMMatrixIdentity())
{
}

void Entity::AddRenderable(std::unique_ptr<Renderable> pRenderable)
{
	renderable = std::move(pRenderable);
}

void Entity::AddPhysics(std::unique_ptr<PhysicsComponent> pPhysics)
{
	physicsComponent = std::move(pPhysics);
}

void Entity::Render(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightView, const DirectX::XMMATRIX& lightProjection, const Light& light)
{
	if (renderable != nullptr)
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
			transform * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z),
			camera,
			DirectX::XMMatrixPerspectiveLH(1.0f, float(gfx.GetHeight()) / float(gfx.GetWidth()), 0.5f, 700.0f),
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
}

void Entity::RenderDepth(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightProjection, const Light& light)
{
	if (renderable != nullptr)
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
			transform * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z),
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
}

void Entity::UpdatePhysics()
{
	if (physicsComponent != nullptr)
		physicsComponent->Update(this);
}

void Entity::SetType(Entity::Type typeIn)
{
	type = typeIn;
}

Entity::Type Entity::GetType()
{
	return type;
}

void Entity::MarkForDeath()
{
	markedForDeath = true;
}

bool Entity::IsMarkedForDeath()
{
	return markedForDeath;
}

void Entity::SetPosition(float x, float y, float z)
{
	pos = DirectX::XMFLOAT3(x,y,z);
}

const DirectX::XMFLOAT3 & Entity::GetPosition() const
{
	return pos;
}

void Entity::SetTransform(const DirectX::XMMATRIX & transform_in)
{
	transform = transform_in;
}

const DirectX::XMMATRIX & Entity::GetTransform() const
{
	return transform;
}

bool Entity::CanParry()
{
	if (barrierTime < 10) {
		return true;
	}

	return false;
}

bool Entity::CanBlock()
{
	if (barrierTime < 60) {
		return true;
	}
	return false;
}

void Entity::ResetBarrier()
{
	barrierTime = 0;
}

void Entity::CountBarrier()
{
	barrierTime++;
}

void Entity::IncreaseHit()
{
	hit++;
}

const int Entity::NumberofHits() const
{
	return hit;
}


