#pragma once
#include "globals.h"

typedef graphics::scancode_t key;

//Interface for all Path classes
//Path is a Decorated Strategy changing its parent class' position, angle and velocity
class Path
{
public:
	Path() {}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms);
};

// Decorator that changes the speed
class AcceleratingPath : public Path
{
public:
	AcceleratingPath(float dvel, Path* p) : dvel(dvel), _path(p)
	{}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	// raw velocity change per second
	const float dvel;
	Path* _path;
};

// Decorator that changes the angle
class RotatingPath : public Path
{
public:
	RotatingPath(float dangle, Path* p) : dangle(dangle), _path(p)
	{}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms) override;

private:
	// % of a full rotation per second
	const float dangle;
	Path* _path;
};

// Standalone Path that responds to Keyboard
class KeyboardPath : public Path
{
public:
	KeyboardPath(float dangle, Keyset keyset) : dangle(dangle), keyset(keyset)
	{}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms);

private:
	// % of a full rotation per second
	const float dangle;
	const Keyset keyset;
};

// Standalone Path that has no movement
class StaticPath : public Path
{
public:
	StaticPath() : Path()
	{}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms) override;
};
