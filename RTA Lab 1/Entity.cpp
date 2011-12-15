#include "Entity.h"


Entity::Entity(int entityType)
{
	readyToRemove = false;
	EntityType = entityType;
}


Entity::~Entity(void)
{
}

void Entity::Draw()
{

}

void Entity::DrawDebug()
{

}

void Entity::Update(float ticks)
{

}

void Entity::Collide(Entity* other)
{

}

BoundingSphere Entity::GetCollisionSphere()
{
	BoundingSphere sphere;
	sphere.Position = Vec3();
	sphere.Radius = 1.0f;
	return sphere;
}