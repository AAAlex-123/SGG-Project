#include "gameobject.h"
#include "constants.h"

GameObject::GameObject(float xpos, float ypos, float angle, float vel, float radius, const std::string* sprites, Path* p, int damage, int health) :
	Drawing(xpos, ypos, angle, vel, radius, sprites, p),
	damage(damage), health(health), isDead(false)
{}

void GameObject::hit(GameObject& o2){
	this->health = -o2.damage;
	isDead = health <= 0;
}

bool GameObject::collides(GameObject& o2) const{
	return ((((o2.x - this->x) * (o2.x - this->x)) + ((o2.y - this->y) * (o2.y - this->y)))
		< (this->radius - o2.radius) * (this->radius - o2.radius));
}

void GameObject::update(float ms){
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
	if (this->x > CANVAS_WIDTH || this->x<0 || this->y >CANVAS_HEIGHT || this->y < 0)
		isDead = true;
}

GameObject::operator bool() const{
	return !this->isDead;
}

GameObject::~GameObject(){
	delete movement,sprites;
}
