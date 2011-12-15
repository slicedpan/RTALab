#pragma once

#include "Entity.h"
#include "glut.h"
#include "Curve.h"

class Fly : public Entity
{
public:
	Fly(Curve* pCurve);
	~Fly(void);
	void Update(float ticks);
	void Draw();
	void DrawDebug();
	BoundingSphere GetCollisionSphere();
	void Collide(Entity* other);
private:
	void DrawWing(GLuint tex, bool reverse);
	GLuint wingTex;
	GLuint wingTex50;
	GLuint wingTex100;
	GLUquadric* nQ;
	Mat4 _transform;
	Vec3 _position;
	Vec3 _nextPosition;
	float wingAngle;
	float wingSpan;
	float scale;
	bool onCurve;
	bool falling;
	Curve* curve;
	float _pitch;
	float _yaw;
	float factor;
	void Think();
	void ChooseNewCurve();
	void Die();
};