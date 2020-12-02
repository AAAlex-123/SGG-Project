#pragma once
#include "projectile.h"

class Shooting
{
protected:
	Projectile curr_projectile; //this should probably be a constant which the object would use to determine
								//which class of projectile it should use in fire()

public:
	void fire();
	void set_projectile(Projectile); 
};
