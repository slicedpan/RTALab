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

#pragma region Leg Stuff

	float width = 0.5f;	
	float length = 1.0f;
	float legAngle = 20.0f;

	memset((void*)footPoint, 0, sizeof(Vec3) * 8);
	memset((void*)pos, 0, 3 * 8 * sizeof(float));

	pos[0][0] = width;
	angle[0] = 2 * legAngle;
	phase[0] = 0.0f;

	pos[1][0] = -width;
	angle[1] = 180.0f - (2.0f * legAngle);
	phase[1] = M_PI;

	pos[2][2] = length / 3.0f;
	pos[2][0] = -width * 1.1f;
	angle[2] = 180.0f - legAngle;
	phase[2] = 1.5f * M_PI;

	pos[3][0] = width * 1.1f;
	pos[3][2] = length / 3.0f;
	angle[3] = legAngle;
	phase[3] = 0.5f * M_PI;

	pos[4][0] = width * 1.1f;
	pos[4][2] = length / 1.5f;
	angle[4] = 0.0f;
	phase[4] = M_PI;

	pos[5][0] = -1.1f * width;
	pos[5][2] = length / 1.5f;
	angle[5] = 180.0f;
	phase[5] = 0.0f;

	pos[6][0] = -width;
	pos[6][2] = length;
	angle[6] = 180.0f + legAngle;
	phase[6] = 0.5f * M_PI;

	pos[7][0] = width;
	pos[7][2] = length;
	angle[7] = -legAngle;
	phase[7] = 1.5f * M_PI;

#pragma endregion

}



Spider::~Spider(void)
{
}

void Spider::Update(float ticks)
{
	float t = (fmodf(ticks, 10.0f)) / 10.0f;
	//float t = (sinf(ticks) / 2.0f) + 0.5f;	
	if (_curve)
	{
		Vec3 nextPos = _curve->Position(t + 0.01f);
		_position = _curve->Position(t);
		_yaw = atan2f(nextPos[0] - _position[0], nextPos[2] - _position[2]);
		_yaw *= (180.0f / M_PI);
	}
	time = ticks;
}

void Spider::Draw()
{	

	glPushMatrix(); //spider

	glTranslatef(_position[0], _position[1], _position[2]);
	glRotatef(_yaw, 0.0f, 1.0f, 0.0f); 
	glPushMatrix(); //legs

	for (int i = 0; i < 8; ++i)
	{
		//renderLeg(angle[i], pos[i], phase[i]);	
	}
	
	glPushMatrix(); //body
	glTranslatef(0.0f, 0.0f, 0.5f);
	glScalef(1.0f, 1.2f, 2.0f);
	gluSphere(nQ, 0.5f, 15, 15);
	glPopMatrix(); //body

	glTranslatef(0.0f, 0.35f, -0.8f);
	gluSphere(nQ, 0.8f, 15, 15);

	glPopMatrix(); //legs

	glPushMatrix(); //head
	glTranslatef(0.0f, 0.1f, 1.3f);
	glScalef(0.8f, 0.8f, 0.8f);	
	glCallList(skullList);
	glPopMatrix(); // head

	glPopMatrix(); //spider

	for (int i = 0; i < 8; ++i)
	{
		float * point = footPoint[i].Ref();
		float footAngle = -_yaw + angle[i];
		point[0] = cosf(footAngle / (180.0f / M_PI)) * 3.0f + _position[0];

		point[1] = 0.1f;
		point[2] = sinf(footAngle / (180.0f / M_PI)) * 3.0f + _position[2];

		glPushMatrix();
		glTranslatef(point[0], point[1], point[2]);
		gluSphere(nQ, 0.1f, 5, 5);
		glPopMatrix();
	}
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
