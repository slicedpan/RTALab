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
		if  (entities[i]->readyToRemove)
			entitiesToRemove.push_back(entities[i]);
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
	CullEntities();
}

void EntityManager::AddEntity(Entity* entityToAdd)
{
	entities.push_back(entityToAdd);
}

void EntityManager::RemoveEntity(Entity* entityToRemove)
{
	entitiesToRemove.push_back(entityToRemove);
}

void EntityManager::CullEntities()
{
	int index;
	for (int j = 0; j < entitiesToRemove.size(); ++j)
	{
		index = -1;
		Entity* entityToRemove = entitiesToRemove[j];
		for (int i = 0; i < entities.size(); ++i)
		{
			if (entities[i] == entityToRemove)
				index = i;
		}
		if (index >= 0)
			entities.erase(entities.begin() + index);
	}
	entitiesToRemove.clear();
}
