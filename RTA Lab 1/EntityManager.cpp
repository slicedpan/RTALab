#include "EntityManager.h"

EntityManager* EntityManager::pInstance;

EntityManager::EntityManager(void)
{

}

EntityManager::~EntityManager(void)
{

}

EntityManager* EntityManager::CurrentInstance()
{
	if (pInstance == 0)
		pInstance = new EntityManager();
	return pInstance;
}

void EntityManager::Draw()
{
	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i]->Draw();
	}
}

void EntityManager::DrawDebug()
{
	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i]->DrawDebug();
	}
}

void EntityManager::Update(float ticks)
{
	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i]->Update(ticks);
	}
	for (int i = 0; i < entities.size(); ++i)
	{
		for (int j = i + 1; j < entities.size(); ++j)
		{
			if (entities[i]->GetCollisionSphere().Intersects(entities[j]->GetCollisionSphere()))
			{
				entities[i]->Collide(entities[j]);
				entities[j]->Collide(entities[i]);
			}
		}
	}
}

void EntityManager::AddEntity(Entity* entityToAdd)
{
	entities.push_back(entityToAdd);
}

void EntityManager::RemoveEntity(Entity* entityToRemove)
{
	for (int i = 0;
}
