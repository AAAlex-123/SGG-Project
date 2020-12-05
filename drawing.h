#pragma once
#include "graphics.h"
#include <string>

//Interface to allow uniform access to all drawable objects. Includes a standard drawing method all classes need to use.
class Drawing {

private:
	graphics::Brush br;

public:
	const std::string* const sprites;

	float x, y, angle;
	const float radius;
	int curr_sprite;

	Drawing(float xpos, float ypos,float angle, float radius, const std::string* sprites);

	void draw();

	virtual ~Drawing() { 
		delete& x, y, radius, angle, sprites;
	};
};
