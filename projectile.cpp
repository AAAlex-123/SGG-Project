#include "projectile.h"

Projectile::Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path* p, int damage) :
	GameObject(xpos, ypos, angle, vel, radius, sprites, p, damage, 1)
{}

void Projectile::update(float ms) 
{
	;
	// movement->move(this->x, this->y, this->vel);
	// if collides(GameObject&), do something
}
