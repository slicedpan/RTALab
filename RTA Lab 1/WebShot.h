#pragma once

#include "Entity.h"
#include "glut.h"

class WebShot : public Entity
{
public:
	WebShot(Vec3 position, Vec3 velocity);
	~WebShot(void);
	void Update(float time);
	void Draw();
	void DrawDebug();
	BoundingSphere GetCollisionSphere();
	void Collide(Entity* other);
private:
	Vec3 _position;
	Vec3 _velocity;
	unsigned int tex;
	GLUquadric* nQ;
};

