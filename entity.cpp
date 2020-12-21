#include "entity.h"
#include "GObjFactory.h"
#include "graphics.h"
#include <cmath>
#include <iostream>
#include <string>

Entity::Entity(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* sprite_name, Path* path,
	int damage, int health,int score, int proj_type) :
	GameObject(xpos, ypos, angle, vel, width,height, new std::string((*sprite_name) + ".png"), path, damage, health,score),
	curr_projectile(proj_type), _hasFired(false)
{
	this->shadow = std::string(*sprite_name + "_shadow.png");
}

void Entity::update(float ms) {
	this->_hasFired = movement->move(this->x, this->y, this->angle, this->vel, ms);
}

bool Entity::hasFired() const {
	return _hasFired;
}

Projectile* Entity::getProjectile() const {
	return Factory::createProjectile(curr_projectile, x - (radius*sin(angle)), y - (radius*cos(angle)), angle);
}

void Entity::setProjectile(int proj_type) {
	this->curr_projectile = proj_type;
}

void Entity::draw() {
	br.texture = shadow;
	br.fill_opacity = 0.6f;
	graphics::setOrientation(angle / (2 * PI) * 360);
	graphics::drawRect(x - 15, y - 5, width, height, br);
	br.fill_opacity = 1.0f;
	Drawing::draw();
}
Entity::~Entity() {
	if (curr_health <= 0)
		graphics::playSound(sound_path + "entitydeath.mp3",0.5f);
}
