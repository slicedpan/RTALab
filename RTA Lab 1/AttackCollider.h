#pragma once

#include "Spider.h"
#include "glut.h"

class AttackCollider : public Entity
{
public:
	AttackCollider(Vec3 position, Spider* owner);
	~AttackCollider(void);
	void Update(float ticks);
	void DrawDebug();
	void Collide(Entity* other);
	BoundingSphere GetCollisionSphere();
private:
	GLUquadric* nQ;
	Vec3 _position;
	Spider* _owner;
	int frameCounter;
};

