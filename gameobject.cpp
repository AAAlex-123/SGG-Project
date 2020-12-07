#include "gameobject.h"

GameObject::GameObject(float xpos, float ypos, float angle, float vel, float radius, const std::string* sprites, Path* p, int damage, int health) :
	Drawing(xpos, ypos, angle, vel, radius, sprites, p),
	damage(damage), health(health)
{}

void GameObject::hit(GameObject& o2){
	this->health -= o2.damage;
}

bool GameObject::collides(GameObject& o2) const{
	return ((((o2.x - this->x) * (o2.x - this->x)) + ((o2.y - this->y) * (o2.y - this->y)))
		< (this->radius - o2.radius) * (this->radius - o2.radius));
}

void GameObject::update(float ms){
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
}

GameObject::operator bool() const{
	return (this->x < get_canvas_width() && this->x > 0 && this->y < get_canvas_height() && this->y > 0) && (health > 0);
}

GameObject::~GameObject(){
	delete movement, sprites;
}
