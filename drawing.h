#pragma once
#include <string>
#include "graphics.h"

//Interface to allow uniform access to all drawable objects. Includes a standard drawing method all classes need to use.
class Drawing {

protected:
	//Clients should override this method to make their object drawable 
	const virtual std::string* setNewFrame(float ms) const = 0;
	static graphics::Brush br;

public:
	const std::string* sprites;

	float x, y, angle, const radius;

	Drawing(float xpos, float ypos,float angle, float radius, const std::string* sprites);

	void draw() const;

	virtual ~Drawing() { 
		delete& x,y,radius,angle,sprites;
	};
};
