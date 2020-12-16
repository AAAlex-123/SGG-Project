#pragma once
#include "gameobject.h"
#include "projectile.h"

//Interface for all interactive objects that can spawn other objects. Built using composition.
class Entity : public GameObject {
private:
	int curr_projectile;
	bool _hasFired;
	Entity(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path*,
			int damage, int health, int proj_type);

	friend class GObjFactory; //allow creation of entities only to the factory

public:
	// Routine called by the update() main function; updated the object's state
	virtual void update(float ms) override;

	// Returns whether or not this object has fired
	bool hasFired() const;

	// Returns a projectile of type `curr_projectile` using the friend GObjFactory class
	Projectile* getProjectile() const;

	// Changes the projectile type
	void setProjectile(int proj_type);

	virtual ~Entity();
};
