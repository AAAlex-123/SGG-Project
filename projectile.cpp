#include "projectile.h"

Projectile::Projectile(float xpos, float ypos, float radius, std::string* sprites, int vel, int damage, Path* p) :
	GameObject(xpos, ypos, radius, sprites, 1, vel, damage, 1, p)
	{ ; }

void Projectile::update() 
{
	;
	// movement->move(this->x, this->y, this->vel);
	// if collides(GameObject&), do something
}

void Projectile::draw() const 
{
	;
}
