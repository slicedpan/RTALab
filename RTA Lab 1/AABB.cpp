#include "AABB.h"

bool extent::Intersects(extent& other)
{
	float otherHalf = other.length / 2.0f;
	float half = length / 2.0f;
	if (point + half > other.point - otherHalf)
		return true;
	else if (point - half < other.point + otherHalf)
		return true;	
	return false;
}

AABB::AABB(Vec3 position, float width, float height, float depth)
{

}

bool AABB::Intersects(AABB& other)
{
	for (int i = 0; i < 3; ++i)
	{
		if (extents[i].Intersects(other.GetExtent(i)))
			return true;
	}
	return false;
}

extent AABB::GetExtent(int i)
{
	return extents[i];
}


