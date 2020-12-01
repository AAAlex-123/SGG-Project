#pragma once
#include "projectile.h"

class Shooting
{
protected:
	projectile curr_projectile;

public:
	void fire();
	void set_projectile(projectile);
};
