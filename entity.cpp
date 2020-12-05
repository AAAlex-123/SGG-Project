#include "entity.h"

Entity::Entity(float xpos, float ypos, float angle, float radius, std::string* sprites,
	int sprite_no, int vel, int damage, int health, Path* path, Projectile* proj) :
	GameObject(xpos, ypos, angle, radius, sprites, sprite_no, vel, damage, health, path),
	curr_projectile(proj)
{
	;
}

void Entity::update(float ms)
{
	;
}

void Entity::fire() const
{
	;
}
