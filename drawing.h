#pragma once
#include "globals.h"
#include "Path.h"

//Interface to allow uniform access to all drawable objects. Includes a standard virtual drawing method.
class Drawing {

protected:
	static graphics::Brush br;
	const std::string* const sprites;

	float x, y, angle, vel;	// angle is measured in radiants and not in degrees
	const float width, height;
	int curr_sprite;

	Path* movement; //movement across the screen

public:
	Drawing(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* sprites, Path* = new StaticPath());

	// Updates the entities state (movement etc.)
	virtual void update(float) = 0;
	// Draws the object at [x, y] with `angle` orientation with its `curr_sprite`
	virtual void draw();

	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const = 0;

	float get_x() const { return x; }
	float get_y() const { return y; }

	virtual ~Drawing();
};
