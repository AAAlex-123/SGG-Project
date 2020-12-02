#pragma once
#include "drawing.h"
#include "Path.h"


//Superclass for all interactive objects in the game
class GameObject : public Drawing{
protected:
	int vel, damage, health;
	bool isDead = false;
	static Path* movement;

public:

	GameObject(float xpos, float ypos, float size, std::string* sprites, int frames, int vel, int damage, int health) :
		Drawing(xpos, ypos, size, sprites, frames),
		vel(vel),
		damage(damage),
		health(health)
	{};

	//Routine called by the update() main function
	virtual void update() = 0;

	//Behavior when hit by another object
	virtual void hit(GameObject& o2) { //does this need to be virtual? 
		this->health =- o2.damage;
		if (health <= 0)  isDead = true;
	}

	//Calculates whether this has collided with another hitbox
	bool wasHit(GameObject& o2) const {
		float d = sqrt(pow((o2.x - this->x), 2) + pow((o2.y - this->y), 2)); //overflow will be fine...probably?
		return (d - this->size - o2.size) < 0;
	}

	virtual void draw() const override;
};
