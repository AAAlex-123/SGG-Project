#pragma once
#include "gameobject.h"

//Interface for all projectile classes. Different objects are built via composition.
class Projectile : public GameObject
{
private:
	Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path*, int damage);
	
	friend class GObjFactory; //allow creation of projectiles only to the factory

public:
	virtual void update(float ms) override;

	virtual VisualEffect* getDestructionVisualEffect() const;
	
	virtual ~Projectile();
};
