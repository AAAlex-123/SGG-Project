#pragma once
#include "drawing.h"

class GameObject : public Drawing
{
protected:
	float health, vel, damage;
	// ...

public:
	virtual void update() = 0;
	virtual void hit() = 0;
	virtual void destroy() = 0;
};
