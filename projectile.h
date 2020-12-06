#pragma once
#include "gameobject.h"
#include "Path.h"

//Interface for all projectile classes. Different objects are built via composition.
class Projectile : public GameObject {
private:

	Projectile(float xpos, float ypos, float angle, float radius, std::string* sprites, int vel, int damage, Path* p);

	virtual void update(float ms) override;
	friend class GObjFactory; //allow creation of projectiles only to the factory
};
