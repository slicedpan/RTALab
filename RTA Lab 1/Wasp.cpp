#include "Wasp.h"
#include "TextureManager.h"
#include "Material.h"
#include "Bezier.h"
#include "Spider.h"

Wasp::Wasp(Curve * pCurve) : Entity(5)
{
	wingTex = TextureManager::GetCurrentInstance()->GetTexture("flywing");
	wingTex50 = TextureManager::GetCurrentInstance()->GetTexture("flywing50");
	wingTex100 = TextureManager::GetCurrentInstance()->GetTexture("flywing100");
	nQ = gluNewQuadric();
	_transform.MakeDiag();
	_transform.MakeHTrans(Vec3(0.0f, 5.0f, 0.0f));
	wingAngle = 0.0f;
	wingSpan = 3.0f;
	scale = 1.0f;
	_pitch = 0.0f;
	_yaw = 0.0f;
	factor = 0.0f;
	curve = pCurve;
	_position = curve->Position(0.0f);	
	falling = false;
	fallSpeed = 0.0f;
	attacking = false;

	eyeMaterial.Diffuse[0] = 1.0f;
	eyeMaterial.Diffuse[1] = 1.0f;
	eyeMaterial.Diffuse[2] = 1.0f;

	wingMaterial.Diffuse[0] = 0.7f;
	wingMaterial.Diffuse[1] = 0.7f;
	wingMaterial.Diffuse[2] = 0.7f;
}

void Wasp::Collide(Entity* other)
{
	if (other->EntityType == 2)
	{
		falling = true;
		EntityType = 6;
	}
	else if (other->EntityType == 3)
	{
		Die();
	}
}

void Wasp::Die()
{
	readyToRemove = true;
}

Wasp::~Wasp(void)
{
}

void Wasp::Draw()
{
	glPushMatrix(); 
	glMultMatrixf(_transform.Ref()); //Fly's object space

	glScalef(scale, scale, scale);

	glPushMatrix();	//body
	glScalef(1.0f, 1.0f, 1.5f);
	bodyMaterial.SetMaterial();
	gluSphere(nQ, 0.5f, 10, 10);
	glPopMatrix();

	glPushMatrix();	//left eye
	glTranslatef(0.2f, 0.0f, 0.75f);
	eyeMaterial.SetMaterial();
	gluSphere(nQ, 0.1, 5, 5);
	glPopMatrix();

	glPushMatrix();	//right eye
	glTranslatef(-0.2f, 0.0f, 0.75f);
	gluSphere(nQ, 0.1, 5, 5);
	glPopMatrix();

	glPushMatrix();	//left wing
	wingMaterial.SetMaterial();
	glTranslatef(0.4f, 0.0f, 0.0f);
	glRotatef(wingAngle, 0.0f, 0.0f, 1.0f);
	DrawWing(wingTex50, false);
	glPopMatrix();

	glPushMatrix(); //right wing
	glScalef(-1.0f, 1.0f, 1.0f);
	glTranslatef(0.4f, 0.0f, 0.0f);
	glRotatef(wingAngle, 0.0f, 0.0f, 1.0f);
	DrawWing(wingTex50, false);
	glPopMatrix();

	glPopMatrix();
}

void Wasp::DrawWing(GLuint tex, bool reverse)
{
	if (falling)
		return;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDisable(GL_CULL_FACE);
	float offset = wingSpan;
	float left = 1.0f, right = 0.0f;
	if (reverse)
	{
		offset *= -1.0f;
		left = 0.0f;
		right = 1.0f;
	}
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, offset / 2.0f);
	glTexCoord2f(left, 0.0f);
	glVertex3f(offset, 0.0f, offset / 2.0f);
	glTexCoord2f(left, 1.0f);
	glVertex3f(offset, 0.0f, -offset / 2.0f);
	glTexCoord2f(right, 1.0f);
	glVertex3f(0.0f, 0.0f, -offset / 2.0f);
	glTexCoord2f(right, 0.0f);
	glEnd();
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_NORMALIZE);
	
}

void Wasp::DrawDebug()
{
	glPushMatrix();
	Material::DebugMaterial()->SetMaterial();
	glColor3f(1.0f, 0.0f, 0.0f);
	BoundingSphere sphere = GetCollisionSphere();
	glTranslatef(sphere.Position[0], sphere.Position[1], sphere.Position[2]);
	gluSphere(nQ, sphere.Radius, 20, 20);
	glPopMatrix();
	if (curve != 0)
		curve->Draw();
}

void Wasp::Update(float ticks)
{
	wingAngle = sinf(ticks * 50.0f) * 10.0f;
	_transform.MakeDiag();
	_transform *= HRot4(Vec3(1.0f, 0.0f, 0.0f), _pitch);
	_transform *= HRot4(Vec3(0.0f, 1.0f, 0.0f), _yaw);
	_transform *= HTrans4(_position);
	if (falling)
	{
		_position[1] -= fallSpeed;
		fallSpeed += 0.05f;
		if (_position[1] - GetCollisionSphere().Radius < 0)
			_position[1] = GetCollisionSphere().Radius;
		return;
	}
	if (onCurve && curve != 0)
	{
		factor += 0.003f;
		if (factor > 1.0f)
		{
			factor = 1.0f;
			onCurve = false;
		}
		_position = curve->Position(factor - 0.01);
		_nextPosition = curve->Position(factor);
		float linearDist = len(Vec2(_nextPosition[0], _nextPosition[2]) - Vec2(_position[0], _position[2]));
		_pitch = atan2f(_position[1] - _nextPosition[1], linearDist);
		_yaw = atan2f(- _position[0] + _nextPosition[0], - _position[2] + _nextPosition[2]);		
	}
	else
	{
		if (attacking)
		{
			attacking = false;
			Retreat();
			factor = 0.0f;
			onCurve = true;
			return;
		}
		if (rand() % 2 == 0)
		{
			ChooseNewCurve();
			factor = 0.0f;
			onCurve = true;
		}
		else
		{
			Attack();
			onCurve= true;
			factor = 0.0f;
			attacking = true;
		}
	}
}

void Wasp::Attack()
{
	Vec3 p1, p2, p3, p4;
	p1 = _position;
	p2 = _nextPosition - _position;
	p2.Normalise();
	p2 *= 5.0f;
	p2 += p1;	
	p4 = Spider::CurrentInstance()->getPos();
	p4 -= (p2 - p1).Normalise() * 3.0f;
	p3 = p4 + p2;
	p3 /= 2.0f;
	curve = new Bezier(p1, p2, p3, p4);
	originalPos = _position;
}

void Wasp::Retreat()
{
	Vec3 p1, p2, p3, p4;
	p1 = _position;
	p2 = _position + ((originalPos - _position) * 0.33333f) + Vec3(0.0, 5.0, 0.0);
	p3 = _position + ((originalPos - _position) * 0.66666f) + Vec3(0.0, 2.5, 0.0);
	p4 = originalPos;
	curve = new Bezier(p1, p2, p3, p4);
}

void Wasp::ChooseNewCurve()
{
	Vec3 p1, p2, p3, p4;
	p1 = _position;
	p2 = _nextPosition - _position;
	p2.Normalise();
	p2 *= 5.0f;
	p2 += p1;
	float endHeight = (rand() % 10000) / 10000.0f;
	endHeight *= 5.0f;
	endHeight += 1.0f;
	p3[0] = (rand() % 10000 / 10000.0f) * 40.0f - 20.0f;
	p3[1] = endHeight;
	p3[2] = (rand() % 10000 / 10000.0f) * 40.0f - 20.0f;

	printf("p3: (%f, %f, %f)", p3[0], p3[1], p3[2]);

	Vec3 diff = p3 - p2;
	diff.Normalise();
	diff *= 5.0f;

	p4 = p3 + diff;

	curve = new Bezier(p1, p2, p3, p4);

}

void Wasp::Think()
{

}

BoundingSphere Wasp::GetCollisionSphere()
{
	BoundingSphere sphere;
	sphere.Position = _position;
	sphere.Radius = 1.0f;
	return sphere;
}

