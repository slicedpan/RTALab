#pragma once

#include "svl/svl.h"

class BoundingSphere
{
public:
	BoundingSphere(void);
	BoundingSphere(Vec3 position, float radius);
	~BoundingSphere(void);
	Vec3 Position;
	float Radius;
	bool Intersects(BoundingSphere& other);
};

