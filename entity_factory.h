#pragma once
#include "entity.h"
#include <string>

class EntityFactory
{
public:
	Entity* createPlayer(float xpos, float ypos, float radius, std::string* sprites,
		int sprite_no, int vel, int damage, int health, Path*, Projectile*);

	Entity* createEnemy1(float xpos, float ypos, float radius, std::string* sprites,
		int sprite_no, int vel, int damage, int health, Path*, Projectile*);

	Entity* createEnemy2(float xpos, float ypos, float radius, std::string* sprites,
		int sprite_no, int vel, int damage, int health, Path*, Projectile*);
};
