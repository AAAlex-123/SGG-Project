#pragma once
#include "drawing.h"

class VisualEffect : public Drawing {
private:
	float duration, spf, elapsed, total_elapsed;
	const int sprite_no;

public:
	VisualEffect(float xpos, float ypos, float angle, float vel, float radius, std::string* sprites,
		int sprite_no, float duration, float fps) :
		Drawing(xpos, ypos, angle, vel, radius, sprites, new StaticPath),
		sprite_no(sprite_no), duration(duration), spf(1 / fps), elapsed(0), total_elapsed(0)
	{}

	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const override {
		return total_elapsed < duration;
	}

	// Moved the effect and changes the current sprite based on the elapsed time
	void update(float ms) {
		this->movement->move(x, y, angle, vel, ms);

		total_elapsed += ms / 1000.0f;
		elapsed += ms / 1000.0f;

		curr_sprite = (((curr_sprite + 1) % sprite_no) * (elapsed > spf)) + (curr_sprite * !(elapsed > spf));
		elapsed = (0.0f * (elapsed > spf)) + (elapsed * !(elapsed > spf));
	}
	
	virtual ~VisualEffect() {
		delete[] sprites; 
	}
};
