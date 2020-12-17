#pragma once
#include "graphics.h"
#include "globals.h"
#include "Path.h"
#include <string>

//Interface to allow uniform access to all drawable objects. Includes a standard drawing method all classes need to use.
class Drawing {
private:
	graphics::Brush br;

protected:
	const std::string* const sprites;

	float x, y, angle, vel;	// angle is measured in radiants and not in degrees
	const float width, height;
	int curr_sprite;

	Path* movement; //movement across the screen

public:
	Drawing(float xpos, float ypos, float angle, float vel, float width,float height, const std::string* sprites, Path*);

	// Updates the entities state (movement etc.)
	virtual void update(float) = 0;
	// Draws the object at [x, y] with `angle` orientation with its `curr_sprite`
	virtual void draw() final;

	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const = 0;

	virtual ~Drawing();
};
