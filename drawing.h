#pragma once
#include "globals.h"
#include "Path.h"

/**
* Interface to allow uniform access to all drawable objects. Includes a standard virtual drawing method.
* A Drawing is any object that can be drawn and can move on the screen.
*/
class Drawing
{
public:
	Drawing(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* const sprites, Path* = new StaticPath());

	// Updates the entity's state
	virtual void update(float) = 0;
	virtual void draw();

	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const = 0;

	inline float get_x() const { return x; }
	inline float get_y() const { return y; }

	virtual ~Drawing();

protected:
	static graphics::Brush br;

	// the set of the object's sprites. all objects except VisualEffects use just one sprite
	const std::string* const sprites; // this could either be an array or a single string, internally it's handled the same way

	float x, y, angle, vel;	// angle is measured in radiants, not in degrees
	const float width, height;
	int curr_sprite;

	Path* const movement;
};
