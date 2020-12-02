#pragma once
#include "gameobject.h"
#include "Path.h"

//Superclass for all projectiles
class Projectile : public GameObject {
public:
	Projectile(float xpos, float ypos, float radius, std::string* sprites, int vel, int damage) :
		GameObject(xpos, ypos, radius, sprites, 1, vel, damage, 1)
	{ 
		set_path(new StraightPath);
	}

	virtual void update() override
	{
		// movement->move(this->x, this->y, this->vel);
		// if collides(GameObject&), do something
	}

	virtual void draw() const override
	{
		;
	}
};
