#include "projectile.h"

Projectile::Projectile(float xpos, float ypos, float angle, float radius, std::string* sprites, int vel, int damage, Path* p) :
	GameObject(xpos, ypos, angle, radius, sprites, vel, damage, 1, p)
  {}

void Projectile::update(float ms) 
{
	;
	// movement->move(this->x, this->y, this->vel);
	// if collides(GameObject&), do something
}
