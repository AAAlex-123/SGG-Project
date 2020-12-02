#pragma once
#include "gameobject.h"

class VisualEffect : public Drawing
{
private:
	float duration, fps;

public:
	VisualEffect(float xpos, float ypos, float radius, std::string* sprites, int sprite_no,
		float duration, float fps) :
		Drawing(xpos, ypos, radius, sprites, sprite_no),
		duration(duration), fps(fps)
	{ ; }

	virtual void draw() const override
	{
		;
	}
};
