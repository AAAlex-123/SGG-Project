#pragma once
#include "gameobject.h"
#include "Path.h"

class Projectile : public GameObject {
public:

	const static int STANDARD = 1;
	//...


	Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path*, int damage);

	virtual void update(float ms) override;

};
