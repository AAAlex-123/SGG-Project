#pragma once
#include "drawing.h"
#include "Path.h"

// Superclass for all interactive objects in the game
class GameObject : public Drawing {
protected:
	int vel, damage, health;
	bool isDead;

	Path* movement;

public:
	GameObject(float xpos, float ypos, float angle, float radius, std::string* sprites,
		int sprite_no, int vel, int damage, int health, Path*);

	// Routine called by the update() main functions
	virtual void update(float ms) = 0;

	// Behavior when hit by another object
	void hit(GameObject& o2);

	// Calculates whether this object collides with another hitbox
	bool collides(GameObject& o2) const;
};
