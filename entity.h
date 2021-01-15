#pragma once
#include "gameobject.h"
#include "GObjFactory.h"

class Projectile;

// Entities really only differ in their movement and fire patterns. Since both can be described with already established algorithms
// (defined in the Path classes) it's only logical we use those instead of subclassing every single enemy/player class.
// This is why we used composition (and subsequently a Factory  that knows how to assemble each one).

/**
* Interface for all interactive objects that can spawn other objects. Built using composition.
*/
class Entity : public GameObject
{
public:
	virtual void update(float ms) override;

	bool hasFired() const;

	// override draw() to add shadows
	virtual void draw() override;

	// Returns a projectile of type `curr_projectile` using the friend Factory class
	Projectile* getProjectile() const;
	VisualEffect* getFireVisualEffect() const;
	VisualEffect* getDestructionVisualEffect() const override;

	virtual ~Entity();

protected:
	Factory::BULLET curr_projectile;
	bool _hasFired;

	Entity(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* const, Path*,
		int damage, int health, int score, Factory::BULLET proj_type);

	friend class Factory; // allow creation of entities only to the factory

	std::string shadow;
};
