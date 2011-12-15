#pragma once

#include "Entity.h"
#include "glut.h"
#include "Curve.h"
#include "Material.h"

class Wasp : public Entity
{
public:
	Wasp(Curve* pCurve);
	~Wasp(void);
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
	float fallSpeed;
	Material eyeMaterial;
	Material bodyMaterial;
	Material wingMaterial;
	void Attack();
	void Retreat();
	bool attacking;
	Vec3 originalPos;
};