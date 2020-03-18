#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "RenderTexture.h"
#include "Graphics.h"
#include <DirectXMath.h>
#include "Entity.h"


class EntityManager
{
public:
	EntityManager() = default;
	//EntityManager();
	// add an entity to the container
	unsigned int Add(std::unique_ptr<Entity> entity);
	// get a single entity given an id
	// will return nullptr if entity does not exist
	Entity* Get(unsigned int id);
	void UpdatePhysics();
	void RenderDepth(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightProjection, const Light& light);
	void Render(Graphics& gfx, const DirectX::XMMATRIX& camera, const DirectX::XMMATRIX& lightView, const DirectX::XMMATRIX& lightProjection, const Light& light);
	// get a vector with all the id's of a specific type i.e. Entity::Type::VEHICLE
	std::vector<unsigned int> Query(Entity::Type type);
	// delete all entities that are marked for death
	void Cull();
	// delete all entities
	void Clear();
private:
	std::unordered_map<unsigned int, std::unique_ptr<Entity>> entities;
	unsigned int id;
};
