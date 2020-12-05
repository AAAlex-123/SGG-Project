#include "gameobject.h"


GameObject::GameObject(float xpos, float ypos, float agnle, float radius, const std::string* sprites, int vel, int damage, int health, Path* p) :
	Drawing(xpos, ypos, angle, radius, sprites, 1),
	vel(vel), damage(damage), health(health), isDead(false), movement(p)
{ ; }

void GameObject::hit(GameObject& o2)
{
	this->health = -o2.damage;
	isDead = health <= 0;
}

bool GameObject::collides(GameObject& o2) const
{
	return ((((o2.x - this->x) * (o2.x - this->x)) + ((o2.y - this->y) * (o2.y - this->y)))
		< (this->radius - o2.radius) * (this->radius - o2.radius));
}

const std::string* GameObject::setNewFrame(float ms)const {
	return this->sprites;
}
GameObject::~GameObject() {
	delete& vel, damage, health;
	delete movement;
}

