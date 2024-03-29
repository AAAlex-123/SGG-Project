#include "gameobject.h"
#include "game_data.h"

GameObject::GameObject(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* const sprite, Path* p, int damage, int max_health, int score)
	: Drawing(xpos, ypos, angle, vel, width, height, sprite, p),
	damage(damage), score(score), max_health(max_health), radius(std::min(width, height) / 2), curr_health(max_health)
{}

void GameObject::hit(GameObject* o2)
{
	this->curr_health -= o2->damage;
}

void GameObject::collides(GameObject* o2) {
	if ((((o2->x - this->x) * (o2->x - this->x)) + ((o2->y - this->y) * (o2->y - this->y))) < (this->radius + o2->radius) * (this->radius + o2->radius))
	{
		hit(o2);
		o2->hit(this);
	}
}

void GameObject::update(float ms)
{
	this->movement->move(this->x, this->y, this->angle, this->vel, ms);
}

GameObject::operator bool() const
{
	return ((this->x < get_canvas_width() * 1.2f) && (this->x > get_canvas_width() * (-0.2f))
		&& (this->y < get_canvas_height() * 0.9f) && (this->y > get_canvas_width() * (-0.2f)))
		&& (this->curr_health > 0);
}

GameObject::~GameObject()
{
	if (curr_health <= 0 && score > 0) // ignore redundant 'score += 0' calls
	{
		GameData* gd = reinterpret_cast<GameData*>(graphics::getUserData());
		gd->addScore(score);
	}
	delete sprites;
}
