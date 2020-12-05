#pragma once
#include "drawing.h"
#include "Path.h"
#include "graphics.h"

// Superclass for all interactive objects in the game
class GameObject : public Drawing {

protected:
	int const vel, const damage;
	int health;
	bool isDead; //replace with bool op

	Path* movement;

public:
	GameObject(float xpos, float ypos, float angle, float radius, const std::string* sprites, int vel, int damage, int health, Path*);

	// Routines called by the update(), draw() main functions
	virtual void update() = 0;
	const virtual std::string* setNewFrame(float ms) const override;

	// Behavior when hit by another object
	void hit(GameObject& o2);

	// Calculates whether this object collides with another hitbox
	bool collides(GameObject& o2) const;

	virtual ~GameObject();
};
