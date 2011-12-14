#define _USE_MATH_DEFINES

#include "math.h"
#include "Spider.h"
#include "glm\glm.h"
#include "Bezier.h"

Spider::Spider(Curve* curveToFollow)
{
	nQ = gluNewQuadric();
	time = 0.0f;
	GLMmodel* skullModel = glmReadOBJ("skull.obj");
	skullList = 21389213;
	skullList = glmList(skullModel, GLM_SMOOTH | GLM_MATERIAL);
	_curve = curveToFollow;

	lowerLegLength = 2.3f;
	upperLegLength = 1.5f;
	maxLegLength = lowerLegLength + upperLegLength;

	speed = 1.1f;

#pragma region Foot Position Limits
	minAngle[frontRight] = 90.0f;
	maxAngle[frontRight] = 115.0f;
	minDist[frontRight] = 1.5f;
	maxDist[frontRight] = 3.5f;

	minAngle[secondRight] = 135.0f;
	maxAngle[secondRight] = 160.0f;
	minDist[secondRight] = 2.0;
	maxDist[secondRight] = 4.0f;

	minAngle[thirdRight] = 180.0f;
	maxAngle[thirdRight] = 205.0f;
	minDist[thirdRight] = 2.0f;
	maxDist[thirdRight] = 4.0f;

	minAngle[backRight] = 215.0f;
	maxAngle[backRight] = 250.0f;
	minDist[backRight] = 1.5f;
	maxDist[backRight] = 3.45f;

	minAngle[frontLeft] = 65.0f;
	maxAngle[frontLeft] = 90.0f;
	minDist[frontLeft] = 1.5f;
	maxDist[frontLeft] = 3.5f;

	minAngle[secondLeft] = 20.0f;
	maxAngle[secondLeft] = 45.0f;
	minDist[secondLeft] = 2.0f;
	maxDist[secondLeft] = 4.0f;

	minAngle[thirdLeft] = 335.0f;
	maxAngle[thirdLeft] = 360.0f;
	minDist[thirdLeft] = 2.0f;
	maxDist[thirdLeft] = 4.0f;

	minAngle[backLeft] = 290.0f;
	maxAngle[backLeft] = 325.0f;
	minDist[backLeft] = 1.5f;
	maxDist[backLeft] = 3.45f;
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

#pragma endregion

#pragma region Foot Positions

	_transform = HRot4(Vec3(0.0f, 1.0f, 0.0f), _yaw / (180.0f / M_PI)) * HTrans4(_position);	

	float dist[8];

	angle[frontLeft] = ((maxAngle[frontLeft] - minAngle[frontLeft]) / 2.0f) + minAngle[frontLeft];
	dist[frontLeft] = maxDist[frontLeft];
	angle[frontRight] = ((maxAngle[frontRight] - minAngle[frontRight]) / 2.0f) + minAngle[frontRight];
	dist[frontRight] = minDist[frontRight];

	angle[backLeft] = ((maxAngle[backLeft] - minAngle[backLeft]) / 2.0f) + minAngle[backLeft];
	dist[backLeft] = minDist[backLeft];
	angle[backRight] = ((maxAngle[backRight] - minAngle[backRight]) / 2.0f) + minAngle[backRight];
	dist[backRight] = maxDist[backRight];

	angle[secondLeft] = maxAngle[secondLeft];
	dist[secondLeft] = ((maxDist[secondLeft] - minDist[secondLeft]) / 2.0f) + minDist[secondLeft];
	angle[secondRight] = minAngle[secondRight];
	dist[secondRight] = ((maxDist[secondRight] - minDist[secondRight]) / 2.0f) + minDist[secondRight];

	angle[thirdLeft] = minAngle[thirdLeft];
	dist[thirdLeft] = ((maxDist[thirdLeft] - minDist[thirdLeft]) / 2.0f) + minDist[thirdLeft];
	angle[thirdRight] = maxAngle[thirdRight];
	dist[thirdRight] = ((maxDist[thirdRight] - minDist[thirdRight]) / 2.0f) + minDist[thirdRight];

	for (int i = 0; i < 8; ++i)
	{		
		Vec4 point;		
		point[0] = cosf((angle[i]) / (180.0f / M_PI)) * dist[i] + pos[i][0];
		point[1] = 0.1f;
		point[2] = sinf((angle[i]) / (180.0f / M_PI)) * dist[i] + pos[i][2];
		point[3] = 1.0f;
		point *= _transform;		
		footPoint[i] = proj(point);	
		footPoint[i][1] = 0.1f;
		legSplineParam[i] = 0.0f;
		legMoving[i] = false;
	}

#pragma endregion

}

Spider::~Spider(void)
{
}

void Spider::Update(float ticks)
{
	time += 0.016f * speed;
	float t = (fmodf(time, 10.0f)) / 10.0f;
	//float t = (sinf(ticks) / 2.0f) + 0.5f;	
	if (_curve)
	{
		Vec3 nextPos = _curve->Position(t + 0.01f);
		_position = _curve->Position(t);
		_yaw = atan2f(nextPos[0] - _position[0], nextPos[2] - _position[2]);
		_yaw *= (180.0f / M_PI);
	}
	_transform = HRot4(Vec3(0.0f, 1.0f, 0.0f), _yaw / (180.0f / M_PI)) * HTrans4(_position);
	for (int i = 0; i < 8; ++i)
	{		
		if (legMoving[i])
		{
			legSplineParam[i] += 0.05f;
		}
		/*
		Vec4 point;
		angle[i] = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
		float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
		point[0] = cosf((angle[i]) / (180.0f / M_PI)) * dist + pos[i][0];
		point[1] = 0.1f;
		point[2] = sinf((angle[i]) / (180.0f / M_PI)) * dist + pos[i][2];
		point[3] = 1.0f;
		point *= _transform;		
		//footPoint[i] = proj(point);	
		footPoint[i][1] = 0.1f;
		*/
	}
	forwardVec = proj(Vec4(0.0f, 0.0f, 1.0f, 1.0f) * _transform);
	forwardVec.Normalise();
	computeFootPoints();
}

void Spider::Draw()
{	

	glPushMatrix(); //spider

	/*
	glTranslatef(_position[0], _position[1], _position[2]);
	glRotatef(_yaw, 0.0f, 1.0f, 0.0f); 
	*/
	glMultMatrixf(_transform.Ref());
	glPushMatrix(); //legs

	for (int i = 0; i < 8; ++i)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colors[i]);
		//renderLeg(angle[i], pos[i], phase[i]);	
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
	renderLegs();

	glPopMatrix(); //legs

	glPopMatrix(); //spider

	
	for (int i = 0; i < 8; ++i)
	{
		if (legMoving[i])
		{
			//legSpline[i].Draw();			
		}
	}
	

	//drawFeet();
	
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

void Spider::renderLegs()
{
	for (int i = 0; i < 8; ++i)
	{		

		Vec3 shoulderWorld;
		float dist;
		float elbowAngle, shoulderX, shoulderY;		

		shoulderWorld = proj(Vec4(pos[i][0], pos[i][1], pos[i][2], 1.0f) * _transform);
		dist = len(shoulderWorld - footPoint[i]);
		float zx;
		if (dist > maxLegLength)
		{
			elbowAngle = 0.0f;
			shoulderX = 0.0f;
			shoulderY = 0.0f;
		}
		else
		{
			float* p = shoulderWorld.Ref();
			float* p2 = footPoint[i].Ref();
			elbowAngle = acosf((pow(upperLegLength, 2) + pow(lowerLegLength, 2) - pow(dist, 2)) / (2 * lowerLegLength * upperLegLength));			
			shoulderY = atan2f(p2[0] - p[0], p2[2] - p[2]);			
			shoulderY -= _yaw / (180.0f / M_PI);
			zx = sqrtf(powf(p2[0] - p[0], 2) + powf(p2[2] - p[2], 2));
			shoulderX = - atan2f(p2[1] - p[1], zx);	
			shoulderX -= asinf((lowerLegLength * sinf(elbowAngle)) / dist);
			elbowAngle = M_PI - elbowAngle;						
		}

		glPushMatrix();

		glTranslatef(pos[i][0], pos[i][1], pos[i][2]);
		glRotatef(shoulderY * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
		glRotatef(shoulderX * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);
		gluCylinder(nQ, 0.15f, 0.15f, upperLegLength, 30, 5);
		gluSphere(nQ, 0.15f, 5, 5);
		glTranslatef(0.0f, 0.0f, upperLegLength);
		glRotatef(elbowAngle * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);

		gluCylinder(nQ, 0.15f, 0.05f, lowerLegLength, 30, 5);
		gluSphere(nQ, 0.15f, 5, 5);
		glTranslatef(0.0f, 0.0f, lowerLegLength);
		gluSphere(nQ, 0.05, 5, 5);

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
			if (legSplineParam[i] > 1.0f)
			{
				legSplineParam[i] = 1.0f;
				legMoving[i] = false;
				footPoint[i] = legSpline[i].Position(1.0f);
				footPoint[i][1] = 0.0f;
			}
			else
			{
				footPoint[i] = legSpline[i].Position(legSplineParam[i]);			
			}
		}
		else
		{
			Vec3 worldPos = proj(Vec4(pos[i][0], pos[i][1], pos[i][2], 1.0f) * _transform);
			float linearDist = len(worldPos - footPoint[i]);
			setFootCurve(i);
			/*
			if (linearDist < minDist[i] || linearDist > maxDist[i])
			{
				legMoving[i] = true;
				legSpline[i].SetPoints(footPoint[i], footPoint[i] + forwardVec * 0.66666f + Vec3(0.0f, 1.0f, 0.0f), footPoint[i] + forwardVec * 1.33333f + Vec3(0.0f, 1.0f, 0.0f), footPoint[i] + 2 * forwardVec);
				legSplineParam[i] = 0.0f;
			}
			*/
		}		
	}
}

bool Spider::violatesConstraints(int i, float dist, float angle)
{	
	return (dist + 0.0001f < minDist[i] || dist > maxDist[i] + 0.0001f || angle + 0.0001f < minAngle[i] || angle > maxAngle[i] + 0.0001f);
}

void Spider::setFootCurve(int i)
{
	Vec3 endPoint;
	Vec3 height (0.0f, 1.0f, 0.0f);
	Vec3 worldPos = proj(Vec4(pos[i][0], pos[i][1], pos[i][2], 1.0f) * _transform);	
	Mat4 endTransform;
	float t = (fmodf(time, 10.0f)) / 10.0f;
	t += 0.032f * speed;
	Vec3 endSpiderPos = _curve->Position(t);
	Vec3 endSpiderNext = _curve->Position(t + 0.001f);
	float endYaw = atan2f(endSpiderNext[0] - endSpiderPos[0], endSpiderNext[2] - endSpiderPos[2]);
	endTransform = HRot4(Vec3(0.0, 1.0f, 0.0f), endYaw) * HTrans4(endSpiderPos);

	float linearDist = sqrtf(powf(worldPos[0] - footPoint[i][0], 2) + powf(worldPos[2] - footPoint[i][2], 2));
	float angle = atan2f(footPoint[i][2] - worldPos.Ref()[2], footPoint[i][0] - worldPos.Ref()[0]);
	angle *= 180.0f / M_PI;
	angle += _yaw;
	if (angle < 0)
		angle += 360.0f;
	
	if (i == 0 || i == 1) //front legs
	{		
		if (violatesConstraints(i, linearDist, angle))
		{			 
			legMoving[i] = true;
			legSplineParam[i] = 0.0f;
			float legAngle = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
			float dist = maxDist[i] - 0.05f;
			endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[i][0];
			endPoint[1] = 0.1f;	
			endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[i][2];
			endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			endPoint[1] = 0.0f;
			Vec3 diff = endPoint - footPoint[i];			 
			legSpline[i].SetPoints(footPoint[i], footPoint[i] + 0.3333 * diff + height, footPoint[i] + 0.6666 * diff + height, endPoint);
		}
	}
	else if (i == 2 || i == 3) //left legs
	{
		if (violatesConstraints(i, linearDist, angle))
		{			 
			legMoving[i] = true;
			legSplineParam[i] = 0.0f;
			float legAngle = maxAngle[i];
			float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
			endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[i][0];
			endPoint[1] = 0.1f;	
			endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[i][2];
			endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			endPoint[1] = 0.0f;
			Vec3 diff = endPoint - footPoint[i];			 
			legSpline[i].SetPoints(footPoint[i], footPoint[i] + 0.3333 * diff + height, footPoint[i] + 0.6666 * diff + height, endPoint);
		}
	}
	else if (i == 4 || i == 5) //right legs
	{
		if (violatesConstraints(i, linearDist, angle))
		{			 
			legMoving[i] = true;
			legSplineParam[i] = 0.0f;
			float legAngle = minAngle[i];
			float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
			endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[i][0];
			endPoint[1] = 0.1f;	
			endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[i][2];
			endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			endPoint[1] = 0.0f;
			Vec3 diff = endPoint - footPoint[i];			 
			legSpline[i].SetPoints(footPoint[i], footPoint[i] + 0.3333 * diff + height, footPoint[i] + 0.6666 * diff + height, endPoint);
		}
	}
	else //backlegs
	{
		if (violatesConstraints(i, linearDist, angle))
		 {
			 legMoving[i] = true;
			 legSplineParam[i] = 0.0f;
			 float angle = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
			 float dist = minDist[i] + 0.05f;
			 endPoint[0] = cosf((angle) / (180.0f / M_PI)) * dist + pos[i][0];
			 endPoint[1] = 0.1f;	
			 endPoint[2] = sinf((angle) / (180.0f / M_PI)) * dist + pos[i][2];
			 endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			 endPoint[1] = 0.0f;
			 Vec3 diff = endPoint - footPoint[i];			 
			 legSpline[i].SetPoints(footPoint[i], footPoint[i] + 0.3333 * diff + height, footPoint[i] + 0.6666 * diff + height, endPoint);
		 }
	}
}