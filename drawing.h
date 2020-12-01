#pragma once
#include <string>

class Drawing
{
protected:
	std::string* sprites;

	float xpos, ypos, angle, size;

public:
	virtual void draw() const;
};
