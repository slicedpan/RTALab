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

#pragma region Foot Position Limits
	minAngle[frontRight] = 90.0f;
	maxAngle[frontRight] = 115.0f;
	minDist[frontRight] = 0.7f;
	maxDist[frontRight] = 3.0f;

	minAngle[secondRight] = 135.0f;
	maxAngle[secondRight] = 180.0f;
	minDist[secondRight] = 1.0f;
	maxDist[secondRight] = 2.5f;

	minAngle[thirdRight] = 180.0f;
	maxAngle[thirdRight] = 225.0f;
	minDist[thirdRight] = 1.0f;
	maxDist[thirdRight] = 2.5f;

	minAngle[backRight] = 245.0f;
	maxAngle[backRight] = 270.0f;
	minDist[backRight] = 0.7f;
	maxDist[backRight] = 3.0f;

	minAngle[frontLeft] = 65.0f;
	maxAngle[frontLeft] = 90.0f;
	minDist[frontLeft] = 0.7f;
	maxDist[frontLeft] = 3.0f;

	minAngle[secondLeft] = 0.0f;
	maxAngle[secondLeft] = 45.0f;
	minDist[secondLeft] = 1.0f;
	maxDist[secondLeft] = 2.5f;

	minAngle[thirdLeft] = -45.0f;
	maxAngle[thirdLeft] = 0.0f;
	minDist[thirdLeft] = 1.0f;
	maxDist[thirdLeft] = 2.5f;

	minAngle[backLeft] = 270.0f;
	maxAngle[backLeft] = 295.0f;
	minDist[backLeft] = 0.7f;
	maxDist[backLeft] = 3.0f;
#pragma endregion

#pragma region Leg Stuff

	float width = 0.5f;	
	float length = 1.0f;
	float legAngle = 20.0f;

	memset((void*)footPoint, 0, sizeof(Vec3) * 8);
	memset((void*)pos, 0, 3 * 8 * sizeof(float));
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			colors[i][j] = 1.0f;
		}
	}

	pos[backLeft][0] = width;
	angle[backLeft] = 2 * legAngle;
	phase[backLeft] = 0.0f;	

	pos[backRight][0] = -width;
	angle[backRight] = 180.0f - (2.0f * legAngle);
	phase[backRight] = M_PI;

	pos[thirdRight][2] = length / 3.0f;
	pos[thirdRight][0] = -width * 1.1f;
	angle[thirdRight] = 180.0f - legAngle;
	phase[thirdRight] = 1.5f * M_PI;

	pos[thirdLeft][0] = width * 1.1f;
	pos[thirdLeft][2] = length / 3.0f;
	angle[thirdLeft] = legAngle;
	phase[thirdLeft] = 0.5f * M_PI;

	pos[secondLeft][0] = width * 1.1f;
	pos[secondLeft][2] = length / 1.5f;
	angle[secondLeft] = 0.0f;
	phase[secondLeft] = M_PI;

	pos[secondRight][0] = -1.1f * width;
	pos[secondRight][2] = length / 1.5f;
	angle[secondRight] = 180.0f;
	phase[secondRight] = 0.0f;

	pos[frontRight][0] = -width;
	pos[frontRight][2] = length;
	angle[frontRight] = 180.0f + legAngle;
	phase[frontRight] = 0.5f * M_PI;

	pos[frontLeft][0] = width;
	pos[frontLeft][2] = length;
	angle[frontLeft] = -legAngle;
	phase[frontLeft] = 1.5f * M_PI;

	Vec3 nextPos = _curve->Position(0.01f);
	_position = _curve->Position(0);
	_yaw = atan2f(nextPos[0] - _position[0], nextPos[2] - _position[2]);
	_yaw *= (180.0f / M_PI);	

	for (int i = 0; i < 8; ++i)
	{
		float* point = footPoint[i].Ref();
		angle[i] = (maxAngle[i] - minAngle[i]) / 2.0f;
		float dist = (maxDist[i] - minDist[i]) / 2.0f;
		point[0] = cosf(-_yaw + angle[i] / (180.0f / M_PI)) * dist + pos[i][0] + _position[0];
		point[1] = 0.1f;
		point[2] = sinf(-_yaw + angle[i] / (180.0f / M_PI)) * dist + pos[i][2] + _position[2];
	}

#pragma endregion

}

Spider::~Spider(void)
{
}

void Spider::Update(float ticks)
{
	time += 0.016f;
	float t = (fmodf(time, 10.0f)) / 10.0f;
	//float t = (sinf(ticks) / 2.0f) + 0.5f;	
	if (_curve)
	{
		Vec3 nextPos = _curve->Position(t + 0.01f);
		_position = _curve->Position(t);
		_yaw = atan2f(nextPos[0] - _position[0], nextPos[2] - _position[2]);
		_yaw *= (180.0f / M_PI);
	}
	for (int i = 0; i < 8; ++i)
	{
		float* point = footPoint[i].Ref();
		angle[i] = (maxAngle[i] - minAngle[i]) / 2.0f;
		float dist = (maxDist[i] - minDist[i]) / 2.0f;
		point[0] = cosf((-_yaw + angle[i]) / (180.0f / M_PI)) * dist + _position[0];
		point[1] = 0.1f;
		point[2] = sinf((-_yaw + angle[i]) / (180.0f / M_PI)) * dist + _position[2];
	}
}

void Spider::Draw()
{	

	glPushMatrix(); //spider

	glTranslatef(_position[0], _position[1], _position[2]);
	glRotatef(_yaw, 0.0f, 1.0f, 0.0f); 
	glPushMatrix(); //legs

	for (int i = 0; i < 8; ++i)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors[i]);
		renderLeg(angle[i], pos[i], phase[i]);	
	}
	
	glPushMatrix(); //body
	glTranslatef(0.0f, 0.0f, 0.5f);
	glScalef(1.0f, 1.2f, 2.0f);
	gluSphere(nQ, 0.5f, 15, 15);
	glPopMatrix(); //body

	glPushMatrix();
	glTranslatef(0.0f, 0.35f, -0.8f);
	gluSphere(nQ, 0.8f, 15, 15);
	glPopMatrix();

	glPushMatrix(); //head
	glTranslatef(0.0f, 0.1f, 1.3f);
	glScalef(0.8f, 0.8f, 0.8f);	
	glCallList(skullList);
	glPopMatrix(); // head

	drawConstraints();	

	glPopMatrix(); //legs

	glPopMatrix(); //spider

	drawFeet();
}

void Spider::drawFeet()
{
	for (int i = 0; i < 8; ++i)
	{
		glPushMatrix();
		glTranslatef(footPoint[i][0], footPoint[i][1], footPoint[i][2]);
		gluSphere(nQ, 0.2f, 5, 5);
		glPopMatrix();
	}
}

void Spider::drawConstraints()
{
	glDisable(GL_LIGHTING);
	for (int i = 0; i < 8; ++i)
	{
		glPushMatrix();
		float* point = pos[i];
		glTranslatef(point[0], point[1] - 1.49f, point[2]);
		float curAngle = -_yaw + minAngle[i];
		float angleOffset = (maxAngle[i] - minAngle[i]) / 11.0f;
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex3f(cosf(minAngle[i] / (180.0f / M_PI)) * minDist[i], 0.0f, sinf(minAngle[i] / (180.0f / M_PI)) * minDist[i]);
		for (int j = 1; j < 11; ++j)
		{
			float px = cosf((minAngle[i] + (j * angleOffset)) / (180.0f / M_PI)) * minDist[i];
			float pz = sinf((minAngle[i] + (j * angleOffset)) / (180.0f / M_PI)) * minDist[i];
			glVertex3f(px, 0.0f, pz);
			glVertex3f(px, 0.0f, pz);
		}
		glVertex3f(cosf(maxAngle[i] / (180.0f / M_PI)) * minDist[i], 0.0f, sinf(maxAngle[i] / (180.0f / M_PI)) * minDist[i]);

		glVertex3f(cosf(minAngle[i] / (180.0f / M_PI)) * maxDist[i], 0.0f, sinf(minAngle[i] / (180.0f / M_PI)) * maxDist[i]);
		for (int j = 1; j < 10; ++j)
		{
			float px = cosf((minAngle[i] + (j * angleOffset)) / (180.0f / M_PI)) * maxDist[i];
			float pz = sinf((minAngle[i] + (j * angleOffset)) / (180.0f / M_PI)) * maxDist[i];
			glVertex3f(px, 0.0f, pz);
			glVertex3f(px, 0.0f, pz);
		}
		glVertex3f(cosf(maxAngle[i] / (180.0f / M_PI)) * maxDist[i], 0.0f, sinf(maxAngle[i] / (180.0f / M_PI)) * maxDist[i]);

		glVertex3f(cosf(minAngle[i] / (180.0f / M_PI)) * minDist[i], 0.0f, sinf(minAngle[i] / (180.0f / M_PI)) * minDist[i]);
		glVertex3f(cosf(minAngle[i] / (180.0f / M_PI)) * maxDist[i], 0.0f, sinf(minAngle[i] / (180.0f / M_PI)) * maxDist[i]);

		glVertex3f(cosf(maxAngle[i] / (180.0f / M_PI)) * minDist[i], 0.0f, sinf(maxAngle[i] / (180.0f / M_PI)) * minDist[i]);
		glVertex3f(cosf(maxAngle[i] / (180.0f / M_PI)) * maxDist[i], 0.0f, sinf(maxAngle[i] / (180.0f / M_PI)) * maxDist[i]);

		glEnd();
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
}

void Spider::renderLeg(float orientation, float * position, float phase)
{
	float speed = 5.0f;
	glPushMatrix();	

	glTranslatef(*position, *(position + 1), *(position + 2));
	glRotatef(orientation, 0.0f, 1.0f, 0.0f);
	glRotatef(22.5f * sinf(time * speed + phase) + 45.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

	//gluCylinder(nQ, 0.15, 0.15, 1.5, 30, 5);	
	gluSphere(nQ, 0.15, 5, 5);

	glTranslatef(0.0f, 0.0f, 1.5f);
	glRotatef(20.0f * cosf(time * speed + phase) + 75.0f, 1.0f, 0.0f, 0.0f);

	//gluCylinder(nQ, 0.15, 0.15, 2.0f, 30, 5);
	//gluSphere(nQ, 0.15, 5, 5);

	glTranslatef(0.0f, 0.0f, 2.0f);
	glRotatef(-10.0f * cosf(time * speed + phase) - 20.0f, 1.0f, 0.0f, 0.0f);
	//gluCylinder(nQ, 0.15, 0.03, 0.75f, 30, 5);
	//gluSphere(nQ, 0.16, 5, 5);

	glPopMatrix();
}

void Spider::computeFootPoints()
{
	for (int i = 0; i < 8; ++i)
	{
		if (legMoving[i])
		{

		}
		else
		{
			float linearDist = sqrt(pow(pos[i][0] - footPoint[i][0], 2) + pow(pos[i][2] - footPoint[i][2], 2));
			if (linearDist < minDist[i])
			{
				legMoving[i] = true;
			}			
		}
	}
}
