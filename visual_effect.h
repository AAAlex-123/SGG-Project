#pragma once
#include "drawing.h"

class VisualEffect : public Drawing
{
public:
	void update(float ms) override;
	
	virtual operator bool() const override;

	virtual VisualEffect* getDestructionVisualEffect() const;

	virtual ~VisualEffect();

private:
	float duration, sprites_per_second, elapsed, total_elapsed;
	const int sprite_no;

	VisualEffect(float xpos, float ypos, float angle, float vel, float size, std::string* sprites,
		int sprite_no, float duration, float fps);

	friend class GObjFactory;	//allow creation of entities only to the factory
};
