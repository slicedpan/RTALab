#include "WebShot.h"
#include "TextureManager.h"
#include "glut.h"


WebShot::WebShot(Vec3 position, Vec3 velocity) : Entity(2)
{
	_position = position;
	_velocity = velocity;
	tex = TextureManager::GetCurrentInstance()->GetTexture("web");
	nQ = gluNewQuadric();
}

void WebShot::Draw()
{
	float linearDist = len(Vec2(_velocity[0], _velocity[2]));
	float pitch, yaw;
	pitch = atan2f(_velocity[1], linearDist);
	yaw = atan2f(_velocity[0], _velocity[2]);
	glPushMatrix();
	glTranslatef(_position[0], _position[1], _position[2]);
	glRotatef(pitch * (180.0f / 3.1415f), 1.0f, 0.0f, 0.0f);
	glRotatef(yaw * (180.0f / 3.1415f), 0.0f, 1.0f, 0.0f);	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
}

void WebShot::DrawDebug()
{
	glPushMatrix();
	glTranslatef(_position[0], _position[1], _position[2]);
	gluSphere(nQ, GetCollisionSphere().Radius, 10, 10);
	glPopMatrix();
}

void WebShot::Update(float ticks)
{
	_position += _velocity;
	if (len(_position) > 100.0f)
	{
		readyToRemove = true;
	}
}

void WebShot::Collide(Entity* other)
{
	readyToRemove = true;
}

BoundingSphere WebShot::GetCollisionSphere()
{
	BoundingSphere sphere;
	sphere.Position = _position;
	sphere.Radius = 0.5f;
	return sphere;
}

WebShot::~WebShot(void)
{
}
