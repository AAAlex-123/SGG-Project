#pragma once
#include "entity.h"

class Player : public Entity {
public:
	Player(float xpos, float ypos, float radius, std::string* sprites,
		int sprite_no, int vel, int damage, int health) :
		Entity(xpos, ypos, radius, sprites, sprite_no, vel, damage, health)
	{
		// set_projectile(new Projectile);
		set_path(new PlayerPath);
	}
};
