#pragma once
#include "gameobject.h"
#include "Path.h"

class Projectile : public GameObject {
public:
	Projectile(float xpos, float ypos, float radius, std::string* sprites, int vel, int damage, Path*);

	virtual void update() override;

	virtual void draw() const override;
};
