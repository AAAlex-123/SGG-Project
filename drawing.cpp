#include "drawing.h"

Drawing::Drawing(float xpos, float ypos, float angle, float radius, const std::string* sprites) :
	x(xpos), y(ypos),
	angle(angle),
	sprites(sprites),
	radius(radius),
	curr_sprite(0)
{}

void Drawing::draw() {
	br.texture = *(setNewFrame(50));//idk what value you want to put here
	graphics::setOrientation(angle);
	graphics::drawRect(this->x, this->y, this->radius * 2, this->radius * 2, br);
}
