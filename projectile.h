#pragma once
#include "gameobject.h"
#include "Path.h"

//Interface for all projectile classes. Different objects are built via composition.
class Projectile : public GameObject {

private:

	Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path*, int damage);

	virtual void update(float ms) override;
	friend class GObjFactory; //allow creation of projectiles only to the factory
public:
	virtual ~Projectile();
};
