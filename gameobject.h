#pragma once
#include "drawing.h"

// forward declare VisualEffect
class VisualEffect;

/**
 * Superclass for all interactive objects in the game
 * Includes methods for hitting other objects, reacting to being hit and placing special effects on death.
 */
class GameObject : public Drawing
{
public:
	GameObject(float xpos, float ypos, float angle, float vel, float width,float height, const std::string * const sprite, Path*, int damage, int health, int score);

	virtual void update(float ms) override;

	// Checks if this object is hit by another object and defines the appropriate behavior
	void collides(GameObject* o2);

	inline int getHealth() const { return curr_health; }
	inline int getMaxHealth() const { return max_health; }
	inline const std::string* const getSprite() const { return sprites; }

	virtual operator bool() const override;

	virtual VisualEffect* getDestructionVisualEffect() const = 0;

	virtual ~GameObject();

protected:
	const int damage, score;
	int max_health;

	const float radius;
	int curr_health;

	virtual void hit(GameObject* o2);
};
