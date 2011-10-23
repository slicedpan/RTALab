#pragma once

#include "Curve.h"

class Spider
{
public:
	Spider(void);
	~Spider(void);
	void Draw();
	void Update(float ticks);
private:
	Curve* _curve;
	Vec3 _position;
	float _yaw;
};

