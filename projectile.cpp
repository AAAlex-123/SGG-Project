#include "projectile.h"
#include "GObjFactory.h"

Projectile::Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path* p, int damage) :
	GameObject(xpos, ypos, angle, vel, radius,radius, sprites, p, damage, 1,0) //width == height -> square
{}

void Projectile::update(float ms){
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
}

VisualEffect* Projectile::getDestructionVisualEffect() const {
	return GObjFactory::createVisualEffect(GObjFactory::NOEFFECT, 0, 0, 0, 0, 0);
}

Projectile::~Projectile() {}
