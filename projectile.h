#pragma once
#include "gameobject.h"

class Projectile : public GameObject
{
public:
	virtual void update();
	virtual void hit();
	virtual void destroy();
};
