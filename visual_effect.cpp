#include "visual_effect.h"
#include "GObjFactory.h"

VisualEffect::VisualEffect(float xpos, float ypos, float angle, float vel, float size, std::string* sprites,
	int sprite_no, float duration, float fps) :
	Drawing(xpos, ypos, angle, vel, size, size, sprites, new StaticPath),
	duration(duration), sprites_per_second(1 / fps), elapsed(0), total_elapsed(0), sprite_no(sprite_no)
{}

// Moved the effect and changes the current sprite based on the elapsed time
void VisualEffect::update(float ms)
{
	this->movement->move(x, y, angle, vel, ms);

	total_elapsed += ms / 1000.0f;
	elapsed += ms / 1000.0f;

	curr_sprite = (((curr_sprite + 1) % sprite_no) * (elapsed > sprites_per_second)) + (curr_sprite * !(elapsed > sprites_per_second));
	elapsed = (0.0f * (elapsed > sprites_per_second)) + (elapsed * !(elapsed > sprites_per_second));
}

VisualEffect::operator bool() const
{
	return total_elapsed < duration;
}

VisualEffect* VisualEffect::getDestructionVisualEffect() const
{
	return Factory::createVisualEffect(Factory::EFFECT::NOEFFECT, 0, 0, 0, 0, 0);
}

VisualEffect::~VisualEffect()
{
	delete[] sprites;
}
