#include "EntityManager.h"

unsigned int EntityManager::Add(std::unique_ptr<Entity> entity)
{
	entities.insert({ id, std::move(entity) });
	return id++;
}

Entity* EntityManager::Get(unsigned int id)
{
	
	auto iter = entities.find(id);
	if (iter != entities.end())
	{
		return iter->second.get();
	}
	
	return nullptr;
}

void EntityManager::UpdatePhysics(const Time& dt)
{
	
	for (auto& e : entities) {
		e.second->UpdatePhysics(dt);
	}
	
}

void EntityManager::RenderDepth(Graphics & gfx, const DirectX::XMMATRIX & camera, const DirectX::XMMATRIX & lightProjection, const Light & light)
{
	
	for (auto& e : entities)
	{
		e.second->RenderDepth(gfx, camera, lightProjection, light);
	}
	
}

void EntityManager::Render(Graphics & gfx, const DirectX::XMMATRIX & camera, const DirectX::XMMATRIX & lightView, const DirectX::XMMATRIX & lightProjection, const Light & light)
{
	
	for (auto& e : entities)
	{
		e.second->Render(gfx, camera, lightView, lightProjection, light);
	}
	
}


std::vector<unsigned int> EntityManager::Query(Entity::Type type)
{
	
	std::vector<unsigned int> query;
	for (auto& e : entities)
	{
		if (e.second->GetType() == type)
		{
			query.push_back(e.first);
		}
	}
	
	return std::move(query);
}

void EntityManager::Cull()
{
	
	for (auto& e : entities)
	{
		if (e.second->IsMarkedForDeath())
		{
			entities.erase(e.first);
		}
	}
	
}

void EntityManager::Clear()
{
	entities = std::unordered_map<unsigned int, std::unique_ptr<Entity>>();
}
