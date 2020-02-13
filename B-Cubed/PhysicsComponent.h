#pragma once

class PhysicsComponent
{
public:
	PhysicsComponent() = default;
	virtual ~PhysicsComponent() = default;
	virtual void Update(class Entity* entity) = 0;
private:
};
