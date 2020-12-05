#pragma once
#include "graphics.h"
#include <string>

//Interface to allow uniform access to all drawable objects
class Drawing {

private:
graphics::Brush br;

public:
	std::string* sprites;

	float x, y, angle, radius;

	//amount of sprite strings in the array
	int sprite_no, curr_sprite;

	virtual void draw()
	{
		br.texture = sprites[curr_sprite];
		graphics::setOrientation(angle);
		graphics::drawRect(x, y, 2 * radius, 2 * radius, br);
	}

	Drawing(float xpos, float ypos, float angle, float radius, std::string* sprites, int sprite_no) :
		br(),
		sprites(sprites),
		x(xpos), y(ypos), angle(angle), radius(radius),
		sprite_no(sprite_no),
		curr_sprite(0)
	{ ; }
};
