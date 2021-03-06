#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Renderable.h"
#include "Light.h"
#include "PhysicsComponent.h"

class Entity
{
public:
	enum class Type
	{
		MISSILE,
		VEHICLE,
		FLOOR,
		DEFAULT
	};

public:
	Entity();
	Entity(std::unique_ptr<Renderable> pRenderable, std::unique_ptr<PhysicsComponent> pPhysics, 
		   const DirectX::XMFLOAT3& pos_in, const DirectX::XMMATRIX& transform_in, Entity::Type type_in );
	// move constructor
	Entity(Entity&& rhs);
	void AddRenderable(std::unique_ptr<Renderable> pRenderable);
	void AddPhysics(std::unique_ptr<PhysicsComponent> pPhysics);
	void Render(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightView, const DirectX::XMMATRIX& lightProjection, const Light& light);
	void RenderDepth(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightProjection, const Light& light);
	void UpdatePhysics(const Time& dt);
	void SetType(Entity::Type typeIn);
	Entity::Type GetType();
	void SetPosition(float x, float y, float z);
	const DirectX::XMFLOAT3& GetPosition() const;
	void SetTransform(const DirectX::XMMATRIX& transform_in);
	const DirectX::XMMATRIX& GetTransform() const;
	void SetVelocity(const DirectX::XMFLOAT3& velocity_in);
	const DirectX::XMFLOAT3& GetVelocity() const;
	void MarkForDeath();
	bool IsMarkedForDeath();
	const DirectX::XMFLOAT3& GetForward() const;

	// -----------------------------
	void IncreaseHit();
	void ResetHit();
	const int NumberofHits() const;
	bool CanParry();
	bool CanBlock();
	void ResetBarrier(float gameTime);
	void CountBarrier(float gameTime);
	void SetBounceBack(bool set);
	void SetSpinOut(bool set);
	bool GetBounceBack() const;
	void SetBounceDir(physx::PxVec3 dir);
	physx::PxVec3 GetBounceDir() const;
	bool GetSpinOut() const;
	int getNumChargesBoost();
	int getNumChargesBarrier();
	int getNumChargesBlast();
	void setNumChargesBoost(int x);
	void setNumChargesBarrier(int x);
	void setNumChargesBlast(int x);
	int GetNumLaps();
	void CountNumLaps();
	bool IsAI();
	int GetNum();
	void SetNum(int x);
public:
	int lastHitCounter = 0;
	//----------------------------------
private:
	std::unique_ptr<PhysicsComponent> physicsComponent = nullptr;
	std::unique_ptr<Renderable> renderable = nullptr;
private:
	// position vector and rotation matrix for object
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMMATRIX transform;
	Entity::Type type = Entity::Type::DEFAULT;
	bool markedForDeath = false;
	// ------------------------
	int hit = 0;
	int	barrierTime = 99;
	bool spinOut = false;
	bool bounceBack = false;
	int numChargesBoost = 0;
	int numChargesBarrier = 0;
	int numChargesBlast = 0;
	physx::PxVec3 bounceDir;
	int numLaps = 0;
	float setParryTime = 0.5f;
	float startParryTimer = 0.f;
	float setBarrierTime = 4.f;
	float startBarrierTimer = 0.f;
	int finishedin = -1;
	bool AI = false;
	int num = -1;
	// ---------------------
};