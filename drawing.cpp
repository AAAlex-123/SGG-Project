#include "drawing.h"

Drawing::Drawing(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* sprites, Path* movement) :
	sprites(sprites),
	x(xpos), y(ypos), angle(angle), vel(vel),
	width(width), height(height),
	curr_sprite(0),
	movement(movement)
{
	br.outline_opacity = 0.0f;
}

void Drawing::draw() {
	br.fill_opacity = 1.0f;
	setColor(br, 'W');
	br.texture = sprites[curr_sprite];
	graphics::setOrientation(angle / (2 * PI) * 360);
	graphics::drawRect(this->x, this->y, this->width, this->height, br);


	setColor(br, 'R');
	graphics::drawDisk(x, y, std::min(width, height) / 2.0f, br);
}

Drawing::~Drawing() {
	delete movement, sprites;
}
