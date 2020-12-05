#pragma once
#include "gameobject.h"
#include "projectile.h"
#include "Path.h"

class Entity : public GameObject {
private:
	int curr_projectile;
	Entity(float xpos, float ypos, float angle, float radius, std::string* sprites,
		int vel, int damage, int health, Path* p, int proj_type);
	friend class GObjFactory; //allow creation of projectiles only to the factory

public:
	
	virtual void update(float ms) override;

	// fires
	Projectile& fire() const;

	void setProjectile(int proj_type);

	virtual ~Entity();
};
