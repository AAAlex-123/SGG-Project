#pragma once
#include "drawing.h"

class VisualEffect;

// Superclass for all interactive objects in the game
class GameObject : public Drawing {
protected:
	const int damage, score, max_health;
	const float radius;
	int curr_health;
	void hit(const GameObject& o2);

public:
	GameObject(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* sprite, Path*, int damage, int health, int score);

	// Routine called by the update() main function; updated the object's state
	virtual void update(float ms) override;

	// Checks if, and controls behavior when, hit by another object
	void collides(GameObject* o2);

	int getHealth() const { return curr_health; }
	int getMaxHealth() const { return max_health; }
	const std::string* getSprite() const {return sprites;}

	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const override;

	virtual VisualEffect* getDestructionVisualEffect() const = 0;

	virtual ~GameObject();
};
