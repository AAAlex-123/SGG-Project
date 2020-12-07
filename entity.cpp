#include "entity.h"
#include "GObjFactory.h"

Entity::Entity(float xpos, float ypos, float angle, float vel, float radius,std::string* sprites, Path* path, int damage, int health, int proj_type) :
	GameObject(xpos, ypos, angle, vel, radius, sprites, path, damage, health),
	curr_projectile(proj_type)
{}

void Entity::update(float ms)
{
	movement->move(this->x, this->y, this->angle, this->vel, ms);
}

Projectile& Entity::fire() const
{
	return GObjFactory::createProjectile(this->curr_projectile, this->y + this->radius + 2, this->y, this->angle);
}

void Entity::setProjectile(int proj_type)
{
	this->curr_projectile = proj_type;
}

Entity::~Entity() {}
