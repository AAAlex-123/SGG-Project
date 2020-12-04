#include "entity.h"

Entity::Entity(float xpos, float ypos, float radius, std::string* sprites,
	int sprite_no, int vel, int damage, int health, Path* path, Projectile* proj) :
	GameObject(xpos, ypos, radius, sprites, sprite_no, vel, damage, health, path),
	curr_projectile(proj)
{
	;
}

void Entity::update()
{
	;
}

void Entity::draw() const
{
	;
}

void Entity::fire() const
{
	;
}
