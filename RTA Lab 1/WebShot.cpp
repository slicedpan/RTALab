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
	glRotatef(pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(pitch, 0.0f, 1.0f, 0.0f);
	glTranslatef(_position[0], _position[1], _position[2]);
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
}

void WebShot::Collide(Entity* other)
{

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
