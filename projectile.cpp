#include "projectile.h"

Projectile::Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path* p, int damage) :
	GameObject(xpos, ypos, angle, vel, radius, sprites, p, damage, 1)
{}

Projectile::~Projectile(){}

void Projectile::update(float ms)
{
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
}
