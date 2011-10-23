#include "svl/svl.h"
#include "Curve.h"

class Bezier : Curve
{
public:
	Vec3 Position(float t);
	Bezier(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4);
	void Draw();
	Vec3 p[4];
};