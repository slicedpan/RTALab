#pragma once

#include "svl/svl.h"

struct extent
{
	float point; 
	float length;
	bool Intersects(extent& other);
};

class AABB
{
public:
	AABB(Vec3 position, float width, float height, float depth);
	void SetPosition(Vec3 position);
	void SetHeight(float height);
	void SetWidth(float width);
	void SetDepth(float depth);
	bool Intersects(AABB& other);
	extent GetExtent(int i);
private:
	extent extents[3];	
};

