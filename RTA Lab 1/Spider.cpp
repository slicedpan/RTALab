#define _USE_MATH_DEFINES

#include "Spider.h"
#include "math.h"

Spider::Spider(void)
{
}

Spider::~Spider(void)
{
}

void Spider::Update(float ticks)
{
	float t = sinf(ticks / 1000.0f);	
	if (_curve)
	{
		Vec3 nextPos = _curve->Position(t + 0.01f);
		_position = _curve->Position(t);
		_yaw = atan2f(nextPos[0] - _position[0], nextPos[2] - _position[2]);
		_yaw /= (180.0f / M_PI);
	}
}
