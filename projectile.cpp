#include "projectile.h"
#include "GObjFactory.h"
#include "graphics.h"
#include "globals.h"

Projectile::Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path* p, int damage) :
	GameObject(xpos, ypos, angle, vel, radius,radius, sprites, p, damage, 1,0) //width == height -> square
{}


void Projectile::update(float ms){
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
}

void Projectile::hit(GameObject* o2) {
	GameObject::hit(o2);
	graphics::playSound(sound_path + "hit.mp3", 0.2f, false);
}

VisualEffect* Projectile::getDestructionVisualEffect() const {
	return GObjFactory::createVisualEffect(GObjFactory::NOEFFECT, 0, 0, 0, 0, 0);
}

Projectile::~Projectile() {}
