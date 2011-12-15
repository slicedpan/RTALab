#pragma once

#include "Curve.h"
#include "glut.h"
#include "gl/glu.h"
#include "Bezier.h"
#include "Entity.h"

class Spider: public Entity
{
public:
	Spider(Vec3 position);
	~Spider(void);
	void Draw();
	void DrawDebug();
	void Update(float ticks);
	void Advance();
	void GoBackwards();
	void GoLeft();
	void GoRight();
	void TurnLeft();
	void TurnRight();	
	void SetTargetYaw(float yaw);
	void Fire(float yaw, float pitch);
	void Attack();
	void Collide(Entity* other);
	static const int frontLeft = 0;
	static const int frontRight = 1;
	static const int secondLeft = 2;
	static const int secondRight = 4;
	static const int thirdLeft = 3;
	static const int thirdRight = 5;
	static const int backLeft = 6;
	static const int backRight = 7;
	float maxLegLength;
	float upperLegLength;
	float lowerLegLength;
	Vec3 getPos();
	float getYaw();
	Mat4 getTransform();
	BoundingSphere GetCollisionSphere();
	int score;
	int health;
	static Spider* CurrentInstance();
private:
	static Spider* pInstance;
	void drawConstraints(float * color, Mat4 transform);
	void drawConstraints();
	void moveLegTo(int leg, Vec3 position, float curveHeight, float curveSpeed);
	bool breaksConstraints(int leg, Vec3 point, Mat4 transform);
	Curve* _curve;
	Vec3 _position;
	Vec3 _lastPosition;
	float _yaw;
	float _lastYaw;
	void renderLeg(float orientation, float * position, float phase);
	GLUquadric* nQ;
	float time;
	GLuint skullList;
	float pos[8][3];
	float angle[8];
	float phase[8];
	Vec3 footPoint[8];
	bool legMoving[8];
	float maxAngle[8];
	float minAngle[8];
	float minDist[8];
	float maxDist[8];
	float colors[8][3];
	void computeFootPoints();
	void drawFeet();
	void renderLegs();
	Mat4 _transform;
	float legSplineParam[8];
	Bezier legSpline[8];
	Vec3 forwardVec;
	Vec3 rightVec;
	Vec3 velocity;
	void setFootCurve(int i);
	float speed;
	float maxSpeed;
	float turningSpeed;
	float maxTurningSpeed;
	Mat4 endTransform;
	float targetYaw;
	bool attacking;
	float attackCounter;
	bool inPain;
	bool fired;
	int fireCounter;
	int painCounter;
	Material bodyMaterial;
	Material legMaterial;
};

