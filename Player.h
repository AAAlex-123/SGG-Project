#pragma once
#include "entity.h"
#include "globals.h"

class Player final : public Entity{

public:

	Player(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* sprite_name, float dangle,float cooldown, Keyset keys,
		int health, int proj_type) :
		Entity(xpos, ypos, angle, vel, width, height, sprite_name, new KeyboardPath(dangle, cooldown, keys),INT_MAX, health,0, proj_type)
	{}

	// Changes the projectile type
	void setProjectile(int proj_type) {
		this->curr_projectile = proj_type;
	}

	void addHealth(int health) {
		curr_health = (curr_health + health > max_health) ? max_health : health;
	}

	virtual ~Player() = default;
};

