#include "Powerup.h"
#include "Player.h"
#include "GObjFactory.h"

// Powerup
Powerup::Powerup(float xpos, float ypos, float angle, const std::string* sprite, int score)
	: GameObject(xpos, ypos, angle, 50.f, 20, 20, sprite, new Path(), 0, 1, score)
{}

void Powerup::hit(GameObject* o2)
{
	GameObject::hit(o2);
	consume(dynamic_cast<Player*> (o2)); // it's guaranteed to be a Player* and the method will be used only a couple times in the game
}

VisualEffect* Powerup::getDestructionVisualEffect() const
{
	return GObjFactory::createVisualEffect(GObjFactory::EFFECT::NOEFFECT, x, y, 0.0f, 1.0f, 18.0f);
}

Powerup::~Powerup()
{
	graphics::playSound(sound_path + "powerup.mp3", 0.2f, false);
}


// HealthPowerup
HealthPowerup::HealthPowerup(float xpos, float ypos, float angle)
	: Powerup(xpos, ypos, angle, new std::string(image_path + "h_powerup.png"), score)
{}

void HealthPowerup::consume(Player* target) const
{
	target->addHealth(65);
}

Powerup* HealthPowerup::clone() const
{
	return new HealthPowerup(x, y, angle);
}


// ProjectilePowerup
ProjectilePowerup::ProjectilePowerup(float xpos, float ypos, float angle)
	: Powerup(xpos, ypos, angle, new std::string(image_path + "proj_powerup.png"), score)
{}

void ProjectilePowerup::consume(Player* target) const
{
	target->upgradeProjectile();
}

Powerup* ProjectilePowerup::clone() const
{
	return new ProjectilePowerup(x, y, angle);
}


// PointsPowerup
PointsPowerup::PointsPowerup(float xpos, float ypos, float angle)
	: Powerup(xpos, ypos, angle, new std::string(image_path + "points_powerup.png"), score)
{}

void PointsPowerup::consume(Player* target_not_used) const
{}

Powerup* PointsPowerup::clone() const
{
	return new PointsPowerup(x, y, angle);
}
