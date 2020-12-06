#pragma once
#include "graphics.h"
#include "path.h"
#include <string>

//Interface to allow uniform access to all drawable objects. Includes a standard drawing method all classes need to use.
class Drawing {

private:
	graphics::Brush br;
	const float PI = 3.14159265359f;

protected:
	const std::string* const sprites;

	float x, y, angle, vel;
	const float radius;
	int curr_sprite;

	Path* movement;

public:
	Drawing(float xpos, float ypos, float angle, float vel, float radius, const std::string* sprites, Path*);

	void draw();

	virtual ~Drawing() { 
		delete& x, y, radius, angle, sprites;
	};
};
