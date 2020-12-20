#pragma once
#include "drawing.h"

class VisualEffect;

// Superclass for all interactive objects in the game
class GameObject : public Drawing {
protected:
	const int damage,score;
	const float radius;
	int health;
	void hit(const GameObject& o2);

public:
	GameObject(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* sprites, Path*, int damage, int health, int score);

	// Routine called by the update() main function; updated the object's state
	virtual void update(float ms) override;

	// Checks if, and controls behavior when, hit by another object
	void collides(GameObject* o2);

	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const override;

	virtual VisualEffect* getDestructionVisualEffect() const = 0;

	virtual ~GameObject();
};
