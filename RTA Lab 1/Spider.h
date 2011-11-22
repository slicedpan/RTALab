#pragma once

#include "Curve.h"
#include "glut.h"
#include "gl/glu.h"
#include "Bezier.h"

class Spider
{
public:
	Spider(Curve* curveToFollow);
	~Spider(void);
	void Draw();
	void Update(float ticks);
	void Advance();
	void GoBackwards();
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

private:
	void drawConstraints(float * color, Mat4 transform);
	void drawConstraints();
	void moveLegTo(int leg, Vec3 position, float curveHeight, float curveSpeed);
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
	void setFootCurve(int i);
	float speed;
	Mat4 endTransform;
};

