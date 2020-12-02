#pragma once
#include "drawing.h"
#include "Path.h"

// Superclass for all interactive objects in the game
class GameObject : public Drawing {
protected:
	int vel, damage, health;
	bool isDead;

	static Path* movement;

public:
	GameObject(float xpos, float ypos, float radius, std::string* sprites,
				int sprite_no, int vel, int damage, int health) :
		Drawing(xpos, ypos, radius, sprites, sprite_no),
		vel(vel), damage(damage), health(health), isDead(false)
	{ ; }

	// Routines called by the update(), draw() main functions
	virtual void update() = 0;
	virtual void draw() const = 0;

	// sets the class' path, normally will be called once
	// in the constructor of the first object of a class
	///// MAYBE VIRTUAL SO SUBCLASSES SET THEIR OWN STATIC MEMBER? /////
	void set_path(Path* p) { this->movement = p; }

	// Behavior when hit by another object
	void hit(GameObject& o2) {
		this->health =- o2.damage;
		isDead = health <= 0;
	}

	// Calculates whether this has collided with another hitbox
	// yes this is more optimized, compiler might do this by itself though
	bool collides(GameObject& o2) const {
		return ((((o2.x - this->x) * (o2.x - this->x)) + ((o2.y - this->y) * (o2.y - this->y)))
			< (this->radius - o2.radius) * (this->radius - o2.radius));
	}
};
