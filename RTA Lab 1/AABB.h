#pragma once

#include "svl/svl.h"

struct extent
{
	float point; 
	float length;
	bool Intersects(const extent& other);
}

class AABB
{
public:
	void SetPosition(Vec3 position);
	void SetHeight(float height);
	void SetWidth(float width);
	void SetDepth(float depth);
	bool Intersects(const AABB& other);
	extent GetExtent(int i);
private:
	extent extents[3];	
};

