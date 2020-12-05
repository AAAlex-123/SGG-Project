#pragma once
#include "gameobject.h"
#include "projectile.h"
#include "Path.h"

class Entity : public GameObject {
protected:
	Projectile* curr_projectile;

public:
	Entity(float xpos, float ypos, float angle, float radius, std::string* sprites,
		int sprite_no, int vel, int damage, int health, Path*, Projectile*);

	// updates
	virtual void update(float ms) override;

	// fires
	void fire() const;
};
