#pragma once
#include "globals.h"
#include <list>

typedef graphics::scancode_t key;

class Drawing;
class Entity;

//Interface for all Path classes
//Path is a Decorated Strategy changing its parent class' position, angle and velocity
class Path {
public:
	Path() {}
	// Updates the position based on angle and velocity
	// Returns bool if the object should fire (only used by Entities, other classes dismiss this value)
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms);
};

// Decorator that accelerates the object in relation to time
class AcceleratingPath : public Path {
public:
	AcceleratingPath(float dvel, Path* p) : dvel(dvel), _path(p)
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	// raw velocity change per second
	const float dvel;
	Path* const _path;
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
	Path* const _path;
};

// Decorator that returns bool if the object should fire
class FiringPath : public Path {
public:
	FiringPath(float period, Path* p) : period(period), elapsed(0), _path(p),curr_angle(0)
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

	virtual float getProjAngle(float x, float y);

protected:
	const float period;
	float elapsed;
	Path* const _path;
	float curr_angle;
};

// Decorator that returns bool if the object should fire
class TargetedFiringPath : public FiringPath {
public:
	TargetedFiringPath(float period, Path* p) : FiringPath(period,p)
	{}
	virtual float getProjAngle(float x, float y) override;
};

// Decorator that changes angle to follow an entity
class HomingPath : public Path {
public:
	HomingPath(float perc, Path* p)
		: perc(perc), _path(p)
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	// percentage of angle turn relative to target, 0-1
	const float perc;
	Path* const _path;
};

// Standalone Path that has no movement
class StaticPath : public Path {
public:
	StaticPath() : Path()
	{}
	virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;
};
