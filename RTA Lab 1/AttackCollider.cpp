#include "AttackCollider.h"

AttackCollider::AttackCollider(Vec3 position, Spider* owner) : Entity(4)
{
	nQ = gluNewQuadric();
	_position = position;
	_owner = owner;
	frameCounter = 0;
}

AttackCollider::~AttackCollider(void)
{

}

void AttackCollider::Update(float ticks)
{
	++frameCounter;
	if (frameCounter > 20)
		readyToRemove = true;
}

void AttackCollider::Collide(Entity* other)
{
	if (other->EntityType == 1)
	{
		other->Die();
		_owner->health += 10;
		if (_owner->health > 100)
			_owner->health = 100;
	}
	else if (other->EntityType == 5)
	{
		other->Die();
		_owner->score += 10;
	}
}

void AttackCollider::DrawDebug()
{
	glPushMatrix();
	glTranslatef(_position[0], _position[1], _position[2]);
	gluSphere(nQ, GetCollisionSphere().Radius, 10, 10);
	glPopMatrix();
}

BoundingSphere AttackCollider::GetCollisionSphere()
{
	BoundingSphere sphere;
	sphere.Position = _position;
	sphere.Radius = 2.0f;
	return sphere;
}