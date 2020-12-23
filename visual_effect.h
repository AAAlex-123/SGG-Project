#pragma once
#include "drawing.h"

class VisualEffect : public Drawing {
private:
	float duration, spf, elapsed, total_elapsed;
	const int sprite_no;

	VisualEffect(float xpos, float ypos, float angle, float vel, float size, std::string* sprites,
		int sprite_no, float duration, float fps);
	
	friend class GObjFactory;  //allow creation of entities only to the factory

public:
	// Returns false if the object is not alive; i.e. it must be destroyed
	virtual operator bool() const override;

	// Moved the effect and changes the current sprite based on the elapsed time
	void update(float ms);

	virtual VisualEffect* getDestructionVisualEffect() const;
	
	virtual ~VisualEffect();
};
