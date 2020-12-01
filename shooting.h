#pragma once
#include "projectile.h"

class Shooting
{
protected:
	Projectile curr_projectile;

public:
	void fire();
	void set_projectile(Projectile);
};
