#include "gameobject.h"

GameObject::GameObject(float xpos, float ypos, float angle, float radius, std::string* sprites,
	int sprite_no, int vel, int damage, int health, Path* p) :
	Drawing(xpos, ypos, angle, radius, sprites, sprite_no),
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
