#include "entity.h"

Entity::Entity(float xpos, float ypos, float angle, float radius,std::string* sprites, int vel, int damage, int health, Path* path, int proj_type) :
	GameObject(xpos, ypos,angle, radius, sprites, vel, damage, health, path),
	curr_projectile(proj_type)
{}

void Entity::update(float ms)
{
	//do stuff lol
}

void Entity::setProjectile(int proj_type) {
	this->curr_projectile = proj_type;
}

/*
Projectile& Entity::fire() const
{
	//return Factory.create(this.proj_type)
}
*/

Entity::~Entity() {
	delete& curr_projectile;
}
