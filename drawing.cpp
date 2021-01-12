#include "drawing.h"

graphics::Brush Drawing::br;

Drawing::Drawing(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* const sprites, Path* movement) :
	sprites(sprites),
	x(xpos), y(ypos), angle(angle), vel(vel),
	width(width), height(height),
	curr_sprite(0),
	movement(movement)
{
	br.outline_opacity = 0.0f; //this should be done once in compile-time but c++ doesn't provide us with static blocks
}

void Drawing::draw()
{
	br.texture = sprites[curr_sprite];
	graphics::setOrientation(angle / (2 * PI) * 360);
	graphics::drawRect(this->x, this->y, this->width, this->height, br);
}

Drawing::~Drawing()
{
	// sprites can either be a single pointer or an array
	// therefore subclasses are responsible for deleting it
	delete movement;
}
