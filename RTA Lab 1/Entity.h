#pragma once
class Entity
{
public:
	Entity(void);
	virtual ~Entity(void);
	virtual void Draw();
	virtual void Update(float ticks);
	virtual void DrawDebug();
};

