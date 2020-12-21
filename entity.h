#pragma once
#include "gameobject.h"

//Entities really only differ in their movement and fire patterns. Since both can be described with already established algorithms (in the Path classes)
//it's only logical we use those instead of subclassing every single enemy/player class. This is why we used composition (and subsequently a Factory class
//that knows how to assemble each one).

class Projectile;	
// VisualEffect is forward declared in gameobject.h

//Interface for all interactive objects that can spawn other objects. Built using composition.
class Entity : public GameObject {
private:
	int curr_projectile;
	bool _hasFired;

	Entity(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* sprite_name, Path*,
			int damage, int health, int score, int proj_type);
	friend class Factory; //allow creation of entities only to the factory
	std::string shadow;

public:
	// Routine called by the update() main function; updated the object's state
	virtual void update(float ms) override;

	// Returns whether or not this object has fired
	bool hasFired() const;

	virtual void draw() override;
	// Returns a projectile of type `curr_projectile` using the friend GObjFactory class
	Projectile* getProjectile() const;
	VisualEffect* getFireVisualEffect() const;
	VisualEffect* getDestructionVisualEffect() const override;

	// Changes the projectile type
	void setProjectile(int proj_type);

	virtual ~Entity();
};
