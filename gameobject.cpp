#include "gameobject.h"
#include "globals.h"
#include "game_data.h"

GameObject::GameObject(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* sprites, Path* p, int damage, int health,int score) :
	Drawing(xpos, ypos, angle, vel, width, height, sprites, p),
	damage(damage), health(health),score(score), radius(std::min(width, height) / 2)
{}

void GameObject::hit(const GameObject& o2) {
	this->health -= o2.damage;
}

void GameObject::collides(GameObject* o2) {
	if ((((o2->x - this->x) * (o2->x - this->x)) + ((o2->y - this->y) * (o2->y - this->y))) < (this->radius - o2->radius) * (this->radius - o2->radius)) {
		hit(*(o2));
		o2->hit(*this);
	}
}

void GameObject::update(float ms) {
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
}

GameObject::operator bool() const {
	return (this->x < get_canvas_width() && this->x > 0 && this->y < get_canvas_height() && this->y > 0) && (this->health > 0);
}

GameObject::~GameObject() {
	if (health <= 0 && score>0) { //ignore redundant 'score += 0' calls
		GameData* gd = reinterpret_cast<GameData*>(graphics::getUserData());
		gd->addScore(score);
	}
		
}
