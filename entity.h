#pragma once
#include "gameobject.h"
#include "projectile.h"
#include "Path.h"

//Interface for all interactive objects that can spawn other objects. Built using composition.
class Entity : public GameObject {
private:
	int curr_projectile;
	Entity(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path*,
			int damage, int health, int proj_type);
  
	friend class GObjFactory; //allow creation of projectiles only to the factory

public:

	virtual void update(float ms) override;

	// fires
	Projectile& fire() const;

	void setProjectile(int proj_type);

	virtual ~Entity();
};
