#pragma once
#include "gameobject.h"
#include "projectile.h"
#include "Path.h"

class Entity : public GameObject {
protected:
	int curr_projectile;

public:
	Entity(float xpos, float ypos, float angle, float radius, std::string* sprites, int vel, int damage, int health, Path*, int proj_type);

	virtual void update() override;

	// fires
	Projectile& fire() const;

	void setProjectile(int proj_type);

	virtual ~Entity();
};
