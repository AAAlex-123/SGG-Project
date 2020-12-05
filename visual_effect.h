#pragma once
#include "drawing.h"

class VisualEffect : public Drawing
{
private:
	float duration, spf, elapsed, total_elapsed;
	const int sprite_no;

public:
	VisualEffect(float xpos, float ypos, float angle, float radius, std::string* sprites,
		int sprite_no, float duration, float fps) :
		Drawing(xpos, ypos, angle, radius, sprites),
		sprite_no(sprite_no), duration(duration), spf(1 / fps), elapsed(0), total_elapsed(0)
	{}

	// true if the object should be destroyed
	operator bool() const
	{
		return total_elapsed > duration;
	}

	// changes the current sprite according to elapsed time
	void updateFrame()
	{
		float ms = graphics::getDeltaTime();
		total_elapsed += ms / 1000.0f;
		elapsed += ms / 1000.0f;

		curr_sprite = (((curr_sprite + 1) % sprite_no) * (elapsed > spf)) + (curr_sprite * !(elapsed > spf));
		elapsed = (0.0f * (elapsed > spf)) + (elapsed * !(elapsed > spf));
	}
};
