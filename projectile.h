#pragma once
#include "gameobject.h"

/**
 * A class defining a projectile thrown by the player or an enemy
 */ 
class Projectile : public GameObject
{
public:
	virtual void update(float ms) override;

	virtual VisualEffect* getDestructionVisualEffect() const;

	virtual void hit(GameObject* o2) override;

	virtual ~Projectile() = default;

private:
	Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path*, int damage);
	
	friend class GObjFactory;	// allow creation of projectiles only to the factory
};
