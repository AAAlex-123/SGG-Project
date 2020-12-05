#pragma once
#include "graphics.h"
#include <string>
#include "graphics.h"

//Interface to allow uniform access to all drawable objects. Includes a standard drawing method all classes need to use.
class Drawing {

private:
	//Clients should override this method to make their object drawable 
	const virtual std::string* setNewFrame(float ms) const = 0;
	graphics::Brush br;

public:
	const std::string* const sprites;

	float x, y, angle, const radius;
	int curr_sprite;

	Drawing(float xpos, float ypos,float angle, float radius, const std::string* sprites);

	void draw();

	virtual ~Drawing() { 
		delete& x, y, radius, angle, sprites;
	};
};
