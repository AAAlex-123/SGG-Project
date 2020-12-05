#pragma once
#include "gameobject.h"
#include "Path.h"

class Projectile : public GameObject {
public:

	const static int STANDARD = 1;
	//...

	virtual void update(float ms) override;

private:
	Projectile(float xpos, float ypos, float angle, float radius, std::string* sprites, int vel, int damage, Path* p);
	friend class GObjFactory; //allow creation of projectiles only to the factory
};
