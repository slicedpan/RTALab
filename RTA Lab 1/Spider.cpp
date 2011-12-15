#define _USE_MATH_DEFINES

#include "math.h"
#include "Spider.h"
#include "glm\glm.h"
#include "Bezier.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "WebShot.h"
#include "EntityManager.h"
#include "AttackCollider.h"

Spider::Spider(Vec3 position) : Entity(0)
{
	nQ = gluNewQuadric();
	time = 0.0f;

	score = 0;
	health = 100;
	
	skullList = ModelManager::CurrentInstance()->GetList("skull");

	_position = position;

	attacking = false;
	attackCounter = 0.0f;

	velocity.MakeZero();

	lowerLegLength = 2.3f;
	upperLegLength = 1.5f;
	maxLegLength = lowerLegLength + upperLegLength;

	speed = 0.015f;
	maxSpeed = 0.05f;
	turningSpeed = 0.0f;
	maxTurningSpeed = 1.25f;
	endTransform.MakeDiag();

#pragma region Foot Position Limits
	minAngle[frontRight] = 90.0f;
	maxAngle[frontRight] = 115.0f;
	minDist[frontRight] = 1.5f;
	maxDist[frontRight] = 3.4f;

	minAngle[secondRight] = 135.0f;
	maxAngle[secondRight] = 160.0f;
	minDist[secondRight] = 2.0;
	maxDist[secondRight] = 3.0f;

	minAngle[thirdRight] = 180.0f;
	maxAngle[thirdRight] = 205.0f;
	minDist[thirdRight] = 2.0f;
	maxDist[thirdRight] = 3.0f;

	minAngle[backRight] = 215.0f;
	maxAngle[backRight] = 250.0f;
	minDist[backRight] = 1.5f;
	maxDist[backRight] = 3.4f;

	minAngle[frontLeft] = 65.0f;
	maxAngle[frontLeft] = 90.0f;
	minDist[frontLeft] = 1.5f;
	maxDist[frontLeft] = 3.4f;

	minAngle[secondLeft] = 20.0f;
	maxAngle[secondLeft] = 45.0f;
	minDist[secondLeft] = 2.0f;
	maxDist[secondLeft] = 3.0f;

	minAngle[thirdLeft] = 335.0f;
	maxAngle[thirdLeft] = 360.0f;
	minDist[thirdLeft] = 2.0f;
	maxDist[thirdLeft] = 3.0f;

	minAngle[backLeft] = 290.0f;
	maxAngle[backLeft] = 325.0f;
	minDist[backLeft] = 1.5f;
	maxDist[backLeft] = 3.4f;
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

	_yaw = 0.0f;
	targetYaw = _yaw;

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

	_lastPosition = _position;
	_lastYaw = _yaw;

#pragma endregion

}

BoundingSphere Spider::GetCollisionSphere()
{
	BoundingSphere sphere;
	sphere.Position = _position;
	sphere.Radius = 2.0f;
	return sphere;
}

Spider::~Spider(void)
{
}

void Spider::Advance()
{
	velocity += forwardVec * speed;
}

void Spider::GoBackwards()
{
	velocity -= forwardVec * speed;
}

void Spider::GoLeft()
{
	velocity -= rightVec * speed;
}

void Spider::GoRight()
{
	velocity += rightVec * speed;
}

void Spider::TurnLeft()
{
	turningSpeed += 0.1f;
}

void Spider::TurnRight()
{
	turningSpeed -= 0.1f;
}

void Spider::Update(float ticks)
{
	float t = (fmodf(time, 10.0f)) / 10.0f;
	//float t = (sinf(ticks) / 2.0f) + 0.5f;
	/*
	if (_curve)
	{
		Vec3 nextPos = _curve->Position(t + 0.01f);
		_position = _curve->Position(t);
		_yaw = atan2f(nextPos[0] - _position[0], nextPos[2] - _position[2]);
		_yaw *= (180.0f / M_PI);
	}
	*/
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
	Mat4 rotMat = HRot4(Vec3(0.0f, 1.0f, 0.0f), _yaw * (M_PI / 180.0f));
	forwardVec = proj(Vec4(0.0f, 0.0f, 1.0f, 1.0f) * rotMat);
	rightVec = proj(Vec4(-1.0f, 0.0f, 0.0f, 1.0f) * rotMat);
	computeFootPoints();	

	float vLength = len(velocity);
	if (vLength < 0.01f)
		velocity.MakeZero();
	else if (vLength > maxSpeed)
	{
		normalise(velocity);
		velocity *= maxSpeed;
	}	

	velocity[1] = 0.0f;
	if (!attacking)
		_position += velocity;
	else
	{
		attackCounter += 0.05f;
		if (attackCounter > 1.0f)
		{
			attacking = false;
			attackCounter = 0.0f;
		}
	}
	velocity *= 0.7f;

	if (targetYaw - _yaw > 180.0f)
		targetYaw -= 360.0f;
	else if (_yaw - targetYaw > 180.0f)
		targetYaw += 360.0f;

	float diff = _yaw - targetYaw;
	turningSpeed -= diff * 0.1f;

	if (fabs(turningSpeed) > maxTurningSpeed)
	{
		turningSpeed /= fabs(turningSpeed);
		turningSpeed *= maxTurningSpeed;
	}

	_yaw += turningSpeed;

	turningSpeed *= 0.75f;

	if (_yaw > 360.0f)
		_yaw -= 360.0f;
	else if(_yaw < 0.0f)
		_yaw += 360.0f; 

	float endYaw = (_yaw + (_yaw - _lastYaw) * 35.0f) / (180.0f / M_PI);
	Vec3 endSpiderPos = velocity * 35.0f + _position;

	endTransform = HRot4(Vec3(0.0, 1.0f, 0.0f), endYaw) * HTrans4(endSpiderPos);

	_lastPosition = _position;
	_lastYaw = _yaw;
	
}

void Spider::SetTargetYaw(float yaw)
{
	targetYaw = -yaw;
}

Vec3 Spider::getPos()
{
	return _position;
}

float Spider::getYaw()
{
	return _yaw;
}

Mat4 Spider::getTransform()
{
	return _transform;
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
	//glCallList(skullList);
	glPopMatrix(); // head

	renderLegs();

	glPopMatrix(); //legs

	glPopMatrix(); //spider

}

void Spider::DrawDebug()
{
	float col[3];
	col[0] = 1.0f;
	col[1] = 0.0f;
	col[2] = 0.0f;
	for (int i = 0; i < 8; ++i)
	{
		if (legMoving[i])
		{
			legSpline[i].Draw();			
		}
	}
	drawConstraints();
	drawConstraints(col, endTransform);
	drawFeet();
	glPushMatrix();
	Material::DebugMaterial()->SetMaterial();
	glColor3f(1.0f, 0.0f, 0.0f);
	BoundingSphere sphere = GetCollisionSphere();
	glTranslatef(sphere.Position[0], sphere.Position[1], sphere.Position[2]);
	gluSphere(nQ, sphere.Radius, 20, 20);
	glPopMatrix();
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
	float col[3];
	col[0] = 1.0f;
	col[1] = 0.5f;
	col[2] = 0.0f;
	drawConstraints(col, _transform);
}

void Spider::drawConstraints(float * color, Mat4 transform)
{
	glDisable(GL_LIGHTING);
	for (int i = 0; i < 8; ++i)
	{
		glPushMatrix();
		glMultMatrixf(transform.Ref());
		float* point = pos[i];
		glTranslatef(point[0], point[1] - 1.49f, point[2]);
		float curAngle = -_yaw + minAngle[i];
		float angleOffset = (maxAngle[i] - minAngle[i]) / 11.0f;
		glBegin(GL_LINES);
		glColor3f(*color, *(color + 1), *(color + 2));
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
			if (breaksConstraints(i, footPoint[i], endTransform) && breaksConstraints(i, footPoint[i], _transform))
			{
				/*
				Vec3 endPoint;
				float legAngle = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
				float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
				endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[i][0];
				endPoint[1] = 0.1f;	
				endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[i][2];
				endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * _transform);
				endPoint[1] = 0.0f;
				moveLegTo(i, endPoint, 0.0f, 1.0f);
				*/
			}
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

void Spider::moveLegTo(int leg, Vec3 position, float curveHeight, float speed)
{
	Vec3 diff = position - footPoint[leg];
	Vec3 height(0.0f, curveHeight, 0.0f);
	legMoving[leg] = true;
	legSplineParam[leg] = 0.0f;
	legSpline[leg].SetPoints(footPoint[leg], footPoint[leg] + 0.3333f * diff + height, footPoint[leg] + 0.6666f * diff + height, position);
}

bool Spider::breaksConstraints(int leg, Vec3 point, Mat4 transform)
{
	Vec3 localPoint = proj(Vec4(footPoint[leg][0], footPoint[leg][1], footPoint[leg][2], 1.0f) * inv(transform));
	float linearDist = sqrtf(powf(pos[leg][0] - localPoint[0], 2) + powf(pos[leg][2] - localPoint[2], 2));
	float angle = atan2f(localPoint[2] - pos[leg][2], localPoint[0] - pos[leg][0]);
	angle *= 180.0f / M_PI;
	if (angle < 0)
		angle += 360.0f;
	return (linearDist < minDist[leg] || angle < minAngle[leg] || angle > maxAngle[leg] || linearDist > maxDist[leg]);
}

void Spider::setFootCurve(int i)
{
	Vec3 endPoint;
	Vec3 worldPos = proj(Vec4(pos[i][0], pos[i][1], pos[i][2], 1.0f) * _transform);	

	float t = (fmodf(time, 10.0f)) / 10.0f;
	t += 0.032f * speed;

	//float endYaw = atan2f(endSpiderNext[0] - endSpiderPos[0], endSpiderNext[2] - endSpiderPos[2]);	
	
	if (i == 0 || i == 1) //front legs
	{		
		if (breaksConstraints(i, footPoint[i], _transform))
		{
			float legAngle = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
			float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
			endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[i][0];
			endPoint[1] = 0.1f;	
			endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[i][2];
			endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			endPoint[1] = 0.0f;
			moveLegTo(i, endPoint, 1.0f, 1.0f);
		}
	}
	else if (i == 2 || i == 3) //left legs
	{
		if (breaksConstraints(i, footPoint[i], _transform))
		{
			float legAngle = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
			float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
			endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[i][0];
			endPoint[1] = 0.1f;	
			endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[i][2];
			endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			endPoint[1] = 0.0f;
			moveLegTo(i, endPoint, 1.0f, 1.0f);
		}
	}
	else if (i == 4 || i == 5) //right legs
	{
		if (breaksConstraints(i, footPoint[i], _transform))
		{	
			float legAngle = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
			float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
			endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[i][0];
			endPoint[1] = 0.1f;	
			endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[i][2];
			endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			endPoint[1] = 0.0f;
			moveLegTo(i, endPoint, 1.0f, 1.0f);
		}
	}
	else //backlegs
	{
		if (breaksConstraints(i, footPoint[i], _transform))
		{
			float angle = ((maxAngle[i] - minAngle[i]) / 2.0f) + minAngle[i];
			float dist = ((maxDist[i] - minDist[i]) / 2.0f) + minDist[i];
			endPoint[0] = cosf((angle) / (180.0f / M_PI)) * dist + pos[i][0];
			endPoint[1] = 0.1f;	
			endPoint[2] = sinf((angle) / (180.0f / M_PI)) * dist + pos[i][2];
			endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * endTransform);
			endPoint[1] = 0.0f;
			moveLegTo(i, endPoint, 1.0f, 1.0f);
		}
	}	
}

void Spider::Fire(float yaw, float pitch)
{
	Vec3 wsPos, wsVel;	
	Mat4 transform = HRot4(Vec3(1.0, 0.0, 0.0), -pitch / (180.0f / M_PI));
	transform *= HRot4(Vec3(0.0, 1.0, 0.0), -yaw / (180.0f / M_PI));
	WebShot* webShot;

	wsVel = proj(Vec4(0.0, 0.0, 1.0, 1.0) * transform);
	wsPos = _position + wsVel * 2.5f;

	webShot = new WebShot(wsPos, wsVel);
	EntityManager::CurrentInstance()->AddEntity(webShot);
}

void Spider::Attack()
{
	attacking = true;
	Vec3 forwardVec = proj(Vec4(0.0, 0.0, 1.0, 1.0) * HRot4(Vec3(0.0, 1.0, 0.0), _yaw / (180.0f / M_PI))) * 2.0f;
	Vec3 endPoint;
	float legAngle = ((maxAngle[frontLeft] - minAngle[frontLeft]) / 2.0f) + minAngle[frontLeft];
	float dist = ((maxDist[frontLeft] - minDist[frontLeft]) / 2.0f) + minDist[frontLeft];
	endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[frontLeft][0];
	endPoint[1] = 0.1f;	
	endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[frontLeft][2];
	endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * _transform);
	endPoint[1] = 0.0f;
	legSpline[frontLeft].SetPoints(footPoint[frontLeft], footPoint[frontLeft] + Vec3(0.0, 3.0, 0.0) - forwardVec, footPoint[frontLeft] + Vec3(0.0, 3.0, 0.0), endPoint);
	legSplineParam[frontLeft] = 0.0f;
	legMoving[frontLeft] = true;

	legAngle = ((maxAngle[frontRight] - minAngle[frontRight]) / 2.0f) + minAngle[frontRight];
	endPoint[0] = cosf((legAngle) / (180.0f / M_PI)) * dist + pos[frontRight][0];
	endPoint[1] = 0.1f;	
	endPoint[2] = sinf((legAngle) / (180.0f / M_PI)) * dist + pos[frontRight][2];
	endPoint = proj(Vec4(endPoint[0], endPoint[1], endPoint[2], 1.0f) * _transform);
	endPoint[1] = 0.0f;
	legSpline[frontRight].SetPoints(footPoint[frontRight], footPoint[frontRight] + Vec3(0.0, 3.0, 0.0) - forwardVec, footPoint[frontRight] + Vec3(0.0, 3.0, 0.0), footPoint[frontRight]);
	legSplineParam[frontRight] = 0.0f;
	legMoving[frontRight] = true;

	EntityManager::CurrentInstance()->AddEntity(new AttackCollider(_position + forwardVec, this));

}
