#include "drawing.h"

graphics::Brush Drawing::br;

Drawing::Drawing(float xpos, float ypos, float angle, float radius, const std::string* sprites) :
	x(xpos), y(ypos),
	angle(angle),
	sprites(sprites),
	radius(radius)
{};

void Drawing::draw() const{
	br.texture = *(setNewFrame(50));//idk what value you want to put here
	graphics::setOrientation(angle);
	graphics::drawRect(this->x, this->y, this->radius * 2, this->radius * 2, br);
}