#pragma once
#include <string>

//Interface to allow uniform access to all drawable objects
class Drawing{

public:
	std::string* sprites;

	float x, y, size;
	//amount of sprite strings in the array
	int frames;

	Drawing(float xpos, float ypos, float size, std::string* sprites, int frames) :
		x(xpos),
		y(ypos),
		sprites(sprites),
		size(size),
		frames(frames)
	{};

	virtual void draw() const = 0;
};
