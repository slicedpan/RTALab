#define _USE_MATH_DEFINES

#include "math.h"
#include "Spider.h"
#include "glm\glm.h"

Spider::Spider(Curve* curveToFollow)
{
	nQ = gluNewQuadric();
	time = 0.0f;
	GLMmodel* skullModel = glmReadOBJ("skull.obj");
	skullList = 21389213;
	skullList = glmList(skullModel, GLM_SMOOTH | GLM_MATERIAL);
	_curve = curveToFollow;
}

Spider::~Spider(void)
{
}

void Spider::Update(float ticks)
{
	float t = (sinf(ticks) / 2.0f) + 0.5f;	
	if (_curve)
	{
		Vec3 nextPos = _curve->Position(t + 0.01f);
		_position = _curve->Position(t);
		_yaw = atan2f(nextPos[0] - _position[0], nextPos[2] - _position[2]);
		//_yaw /= (180.0f / M_PI);
	}
	time = ticks;
}

void Spider::Draw()
{
	float width = 0.5f;	
	float length = 1.0f;
	float legAngle = 20.0f;
	float pos[3];

	//spider
	glPushMatrix();

	glTranslatef(_position[0], _position[1], _position[2]);
	glRotatef(_yaw, 0.0f, 1.0f, 0.0f); 
	//legs
	glPushMatrix();

	pos[0] = width;
	pos[1] = 0.0f;
	pos[2] = 0.0f;
	renderLeg(2 * legAngle, pos, 0.0f);
	pos[0] = -width;
	renderLeg(180.0f - (2.0f * legAngle), pos, M_PI);

	pos[2] = length / 3.0f;
	pos[0] = -width * 1.1f;
	renderLeg(180.0f - legAngle, pos, 1.5f * M_PI);

	pos[0] = width * 1.1f;
	renderLeg(legAngle, pos, 0.5f * M_PI);

	pos[2] *= 2.0f;
	renderLeg(0.0f, pos, M_PI);

	pos[0] = - width * 1.1f;
	renderLeg(180.0f, pos, 0.0f);

	pos[2] = length;
	pos[0] = -width;
	renderLeg(180.0f + legAngle, pos, 0.5f * M_PI);

	pos[0] = width;
	renderLeg(-legAngle, pos, 1.5f * M_PI);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.5f);
	glScalef(1.0f, 1.2f, 2.0f);
	gluSphere(nQ, 0.5f, 15, 15);
	glPopMatrix();

	glTranslatef(0.0f, 0.35f, -0.8f);
	gluSphere(nQ, 0.8f, 15, 15);

	glPopMatrix();
	//head
	glPushMatrix();
	glTranslatef(0.0f, 0.1f, 1.3f);
	glScalef(0.8f, 0.8f, 0.8f);	
	glCallList(skullList);
	glPopMatrix();

	glPopMatrix();
}

void Spider::renderLeg(float orientation, float * position, float phase)
{
	float speed = 5.0f;
	glPushMatrix();	

	glTranslatef(*position, *(position + 1), *(position + 2));
	glRotatef(orientation, 0.0f, 1.0f, 0.0f);
	glRotatef(22.5f * sinf(time * speed + phase) + 45.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

	gluCylinder(nQ, 0.15, 0.15, 1.5, 30, 5);
	gluSphere(nQ, 0.15, 5, 5);

	glTranslatef(0.0f, 0.0f, 1.5f);
	glRotatef(20.0f * cosf(time * speed + phase) + 75.0f, 1.0f, 0.0f, 0.0f);

	gluCylinder(nQ, 0.15, 0.15, 2.0f, 30, 5);
	gluSphere(nQ, 0.15, 5, 5);

	glTranslatef(0.0f, 0.0f, 2.0f);
	glRotatef(-10.0f * cosf(time * speed + phase) - 20.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(nQ, 0.15, 0.03, 0.75f, 30, 5);
	gluSphere(nQ, 0.16, 5, 5);

	glPopMatrix();
}
