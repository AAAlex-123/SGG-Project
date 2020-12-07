#pragma once
#include "drawing.h"

// Superclass for all interactive objects in the game
class GameObject : public Drawing
{
protected:
	const int damage;
	int health;

public:
	GameObject(float xpos, float ypos, float angle, float vel, float radius, const std::string* sprites, Path*, int damage, int health);

	// Routine called by the update() main function; updated the object's state
	virtual void update(float ms);

	// Behavior when hit by another object
	void hit(GameObject& o2);

	// Calculates whether this object collides with another hitbox
	bool collides(GameObject& o2) const;

	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const override;

	virtual ~GameObject();
};
