#pragma once
#include "globals.h"

typedef graphics::scancode_t key;

//Interface for all Path classes
//Path is a Decorated Strategy changing its parent class' position, angle and velocity
class Path {
public:
	Path() {}
	// updates the position based on angle and velocity
	// returns bool if the object should fire
	// only used by Entities, other classes dismiss this value
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms);
};

// Decorator that changes the speed
class AcceleratingPath : public Path {
public:
	AcceleratingPath(float dvel, Path* p) : dvel(dvel), _path(p)
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	// raw velocity change per second
	const float dvel;
	Path* _path;
};

// Decorator that changes the angle
class RotatingPath : public Path {
public:
	RotatingPath(float dangle, Path* p) : dangle(dangle), _path(p)
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	// % of a full rotation per second
	const float dangle;
	Path* _path;
};

// Standalone Path that responds to Keyboard
class KeyboardPath : public Path {
public:
	KeyboardPath(float dangle, float cooldown, Keyset keyset) : dangle(dangle), cooldown(cooldown), remaining(0.0f), keyset(keyset)
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	// % of a full rotation per second
	const float dangle, cooldown;
	float remaining;
	const Keyset keyset;
};

// Decorator that returns bool if the object should fire
class FiringPath : public Path {
public:
	FiringPath(float period, Path* p) : period(period), elapsed(0), _path(p)
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	const float period;
	float elapsed;
	Path* _path;
};

// Standalone Path that has no movement
class StaticPath : public Path {
public:
	StaticPath() : Path()
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;
};
