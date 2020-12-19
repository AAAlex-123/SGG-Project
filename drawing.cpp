#include "drawing.h"

Drawing::Drawing(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* sprites, Path* movement) :
	sprites(sprites),
	x(xpos), y(ypos), angle(angle), vel(vel),
	width(width), height(height),
	curr_sprite(0),
	movement(movement)
{}

void Drawing::draw() {
	br.texture = sprites[curr_sprite];
	graphics::setOrientation(angle / (2 * PI) * 360);
	graphics::drawRect(this->x, this->y, this->width, this->height, br);
}

Drawing::~Drawing() {
	delete movement, sprites;
}
