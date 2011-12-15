#pragma once

#include <vector>
#include "Entity.h"

using namespace std;

class EntityManager
{
public:
	EntityManager(void);
	~EntityManager(void);
	void Draw();
	void DrawDebug();
	void Update(float ticks);
	void AddEntity(Entity* entityToAdd);
	void RemoveEntity(Entity* entityToRemove);
	static EntityManager* CurrentInstance();
	int Count();
private:
	static EntityManager* pInstance;
	vector<Entity*> entities;
	void CullEntities();
	vector<Entity*> entitiesToRemove;
};

