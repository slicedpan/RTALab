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
private:
	Curve* _curve;
	Vec3 _position;
	float _yaw;
	void renderLeg(float orientation, float * position, float phase);
	GLUquadric* nQ;
	float time;
	GLuint skullList;
};

