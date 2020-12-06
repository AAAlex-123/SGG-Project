#pragma once
#include "gameobject.h"
#include "projectile.h"
#include "Path.h"

class Entity : public GameObject {
protected:
	int curr_projectile;

public:
	Entity(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path*,
			int damage, int health, int proj_type);

	virtual void update(float ms) override;

	// fires
	Projectile& fire() const;

	void setProjectile(int proj_type);

	virtual ~Entity();
};
