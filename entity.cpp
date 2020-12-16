#include "entity.h"
#include "GObjFactory.h"
#include <cmath>

Entity::Entity(float xpos, float ypos, float angle, float vel, float radius,std::string* sprites, Path* path, int damage, int health, int proj_type) :
	GameObject(xpos, ypos, angle, vel, radius, sprites, path, damage, health),
	curr_projectile(proj_type), _hasFired(false)
{}

void Entity::update(float ms) {
	this->_hasFired = movement->move(this->x, this->y, this->angle, this->vel, ms);
}

bool Entity::hasFired() const {
	return _hasFired;
}

Projectile* Entity::getProjectile() const {
	return GObjFactory::createProjectile(curr_projectile, x - (radius*sin(angle)), y - (radius*cos(angle)), angle);
}

void Entity::setProjectile(int proj_type) {
	this->curr_projectile = proj_type;
}

Entity::~Entity() {}
