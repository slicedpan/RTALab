#pragma once

#include "Curve.h"
#include "glut.h"
#include "gl/glu.h"

class Spider
{
public:
	Spider(Curve* curveToFollow);
	~Spider(void);
	void Draw();
	void Update(float ticks);
	static const int frontLeft = 0;
	static const int frontRight = 1;
	static const int secondLeft = 2;
	static const int secondRight = 3;
	static const int thirdLeft = 4;
	static const int thirdRight = 5;
	static const int backLeft = 6;
	static const int backRight = 7;

private:
	void DrawConstraints();
	Curve* _curve;
	Vec3 _position;
	float _yaw;
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
};

