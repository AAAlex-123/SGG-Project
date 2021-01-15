#include "projectile.h"
#include "GObjFactory.h"

Projectile::Projectile(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites, Path* p, int damage)
	: GameObject(xpos, ypos, angle, vel, radius, radius, sprites, p, damage, 1, 0)
{}


void Projectile::update(float ms)
{
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
}

void Projectile::hit(GameObject* o2)
{
	GameObject::hit(o2);
	graphics::playSound(sound_path + "hit.mp3", 0.2f, false);
}

VisualEffect* Projectile::getDestructionVisualEffect() const
{
	return Factory::createVisualEffect(Factory::EFFECT::NOEFFECT, 0, 0, 0, 0, 0);
}
