#pragma once
#include "gameobject.h"
#include "projectile.h"

class Entity : public GameObject
{
protected:
	// nope factory method all the way
	static Projectile* curr_projectile;

public:
	Entity(float xpos, float ypos, float radius, std::string* sprites,
				int sprite_no, int vel, int damage, int health) : 
		GameObject(xpos, ypos,radius, sprites, sprite_no, vel, damage, health)
	{ ; }

	// maybe they can be implemented at this level
	virtual void update() override
	{
		;
	}

	virtual void draw() const override
	{
		;
	}

	// changes current projectile
	void set_projectile(Projectile* p) { curr_projectile = p; }

	// fires
	void fire() const
	{
		;
	}
};
