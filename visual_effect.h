#pragma once
#include "gameobject.h"

class VisualEffect : public Drawing
{
private:
	float duration, fps;

public:
	VisualEffect(float xpos, float ypos, float angle, float radius, std::string* sprites, int sprite_no,
		float duration, float fps) :
		Drawing(xpos, ypos, angle, radius, sprites, sprite_no),
		duration(duration), fps(fps)
	{ ; }

	// changes the current sprite according to elapsed time
	void update(float ms)
	{
		;
	}
};
