#include "AABB.h"

bool extent::Intersects(const extent& other)
{
	float otherHalf = other.length / 2.0f;
	float half = length / 2.0f;
	if (pion
}

AABB::AABB(void)
{
}

bool AABB::Intersects(const AABB& other)
{
	for (int i = 0; i < 3; ++i)
	{
		
	}
}

extent AABB::GetExtent(int i)
{
	return extents[i];
}


