#include "BoundingSphere.h"


BoundingSphere::BoundingSphere(void)
{
}

BoundingSphere::BoundingSphere(Vec3 position, float radius)
{
	Position = position;
	Radius = radius;
}

BoundingSphere::~BoundingSphere(void)
{
}

bool BoundingSphere::Intersects(BoundingSphere& other)
{
	float dist = len(other.Position - Position);
	if (dist < (other.Radius + Radius))
		return true;
	return false;
}
