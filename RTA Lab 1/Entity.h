#pragma once

#include "BoundingSphere.h"

class Entity
{
public:
	Entity(int entityType);
	virtual ~Entity(void);
	virtual void Draw();
	virtual void Update(float ticks);
	virtual void DrawDebug();
	virtual BoundingSphere GetCollisionSphere();
	virtual void Collide(Entity * other);
	bool readyToRemove;
	int EntityType;
};

