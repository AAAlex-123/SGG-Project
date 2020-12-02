#pragma once
#include <string>

//Interface to allow uniform access to all drawable objects
class Drawing {

public:
	std::string* sprites;

	float x, y, radius;

	//amount of sprite strings in the array
	int sprite_no;

	Drawing(float xpos, float ypos, float radius, std::string* sprites, int sprite_no) :
		x(xpos), y(ypos),
		sprites(sprites),
		radius(radius),
		sprite_no(sprite_no)
	{};

	virtual void draw() const = 0;
};
