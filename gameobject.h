#pragma once
#include "drawing.h"

// Superclass for all interactive objects in the game
class GameObject : public Drawing {

protected:
	const int damage;
	int health;
	bool isDead; //replace with bool op

public:
	GameObject(float xpos, float ypos, float angle, float vel, float radius, const std::string* sprites, Path*, int damage, int health);

	// Routines called by the update(), draw() main functions
	virtual void update(float ms) = 0;

	// Behavior when hit by another object
	void hit(GameObject& o2);

	// Calculates whether this object collides with another hitbox
	bool collides(GameObject& o2) const;

	virtual ~GameObject();
};
