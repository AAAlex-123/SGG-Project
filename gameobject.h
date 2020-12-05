#pragma once
#include "drawing.h"
#include "Path.h"

// Superclass for all interactive objects in the game
class GameObject : public Drawing {

protected:
	const int vel, damage;
	int health;
	bool isDead; //replace with bool op

	Path* movement;

public:
	GameObject(float xpos, float ypos, float angle, float radius, const std::string* sprites, int vel, int damage, int health, Path*);

	// Routines called by the update(), draw() main functions
	virtual void update(float ms) = 0;

	// Behavior when hit by another object
	void hit(GameObject& o2);

	// Calculates whether this object collides with another hitbox
	bool collides(GameObject& o2) const;

	virtual ~GameObject();
};
