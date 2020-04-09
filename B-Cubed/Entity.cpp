#include "Entity.h"
#include "DirectXMath.h"
#include "Gui.h"
#include "Game.h"

int Entity::haveWon = 0;

Entity::Entity()
	:
	transform(DirectX::XMMatrixIdentity())
{
}

Entity::Entity(std::unique_ptr<Renderable> pRenderable, std::unique_ptr<PhysicsComponent> pPhysics, const DirectX::XMFLOAT3 & pos_in, const DirectX::XMMATRIX & transform_in, Entity::Type type_in)
	:
	renderable(std::move(pRenderable)),
	physicsComponent(std::move(pPhysics)),
	pos(pos_in),
	transform(transform_in),
	type(type_in)
{
}

Entity::Entity(Entity && rhs)
	:
	Entity()
{
	renderable = std::move(rhs.renderable);
	physicsComponent = std::move(rhs.physicsComponent);
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

		struct Effect
		{
			int i;
			int j[3];
		}effect;
	
		VertexConstant vc
		{
			transform * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z),
			camera,
			DirectX::XMMatrixPerspectiveLH(1.0f, float(gfx.GetHeight()) / float(gfx.GetWidth()), 0.5f, 700.0f),
			lightView,
			lightProjection,
			light.GetPosition()
		};

		if (CanParry() && type == Type::VEHICLE)
		{
			effect.i = 1;
		}
		else if (CanBlock() && type == Type::VEHICLE)
		{
			effect.i = 2;
		}
		else
		{
			effect.i = 0;
		}

		renderable->UpdateVertex(gfx, vc);
		renderable->UpdatePixel(gfx, effect);

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
		//renderable->UpdatePixel(gfx, pc);

		renderable->RenderDepth(gfx);
	}
}

void Entity::UpdatePhysics(const Time& dt)
{
	if (physicsComponent != nullptr)
		physicsComponent->Update(this, dt);
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

bool Entity::IsAI() {
	return AI;
}

const DirectX::XMFLOAT3 & Entity::GetForward() const
{
	return (DirectX::XMFLOAT3&)transform.r[2];
}

void Entity::SetPosition(float x, float y, float z)
{
	pos = DirectX::XMFLOAT3(x,y,z);
	physicsComponent->SetPosition(DirectX::XMFLOAT3(x, y, z));
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

void Entity::SetVelocity(const DirectX::XMFLOAT3 & velocity_in)
{
	velocity = velocity_in;
	physicsComponent->SetVelocity(velocity_in);
}

const DirectX::XMFLOAT3 & Entity::GetVelocity() const
{
	return velocity;
}

bool Entity::CanParry()
{
	if (barrierTime < setParryTime) {
		return true;
	}

	return false;
}

bool Entity::CanBlock()
{
	if (barrierTime < setBarrierTime) {
		return true;
	}
	return false;
}

void Entity::ResetBarrier(float gameTime)
{
	startBarrierTimer = gameTime;
}

void Entity::CountBarrier(float gameTime)
{
	barrierTime = gameTime - startBarrierTimer;
}

void Entity::SetBounceBack(bool set)
{
	bounceBack = set;
}

void Entity::SetSpinOut(bool set)
{
	spinOut = set;
}

bool Entity::GetBounceBack() const
{
	return bounceBack;
}

void Entity::SetBounceDir(physx::PxVec3 dir)
{
	bounceDir = dir;
}

physx::PxVec3 Entity::GetBounceDir() const
{
	return bounceDir;
}


bool Entity::GetSpinOut() const
{
	return spinOut;
}

int Entity::getNumChargesBoost()
{
	return numChargesBoost;
}

int Entity::getNumChargesBarrier()
{
	return numChargesBarrier;
}

int Entity::getNumChargesBlast()
{
	return numChargesBlast;
}

void Entity::setNumChargesBoost(int x)
{
	numChargesBoost = x;
}

void Entity::setNumChargesBarrier(int x)
{
	numChargesBarrier = x;
}

void Entity::setNumChargesBlast(int x)
{
	numChargesBlast = x;
}

int Entity::GetNumLaps()
{
	return numLaps;
}

void Entity::CountNumLaps()
{
	numLaps++;
}

int Entity::getFinishedIn()
{
	return finishedin;
}

void Entity::setFinishedIn(int placement)
{
	finishedin = placement;
}

void Entity::IncreaseHit()
{
	hit++;
}

void Entity::ResetHit()
{
	hit = 0;
}

const int Entity::NumberofHits() const
{
	return hit;
}


