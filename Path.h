#pragma once
#include "graphics.h"
#include "scancodes.h"
#include <cmath>

typedef graphics::scancode_t key;

//Interface for all Path classes
//Path is a Decorated Strategy changing its parent class' position, angle and velocity
class Path {
protected:
	const float PI = 3.14159265359f;
public:
	Path() {}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms);
};

// Decorator that changes the speed
class AcceleratingPath : public Path
{
public:
	AcceleratingPath(Path* p, float dvel) : _path(p), dvel(dvel) {}
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
	RotatingPath(Path* p, float dangle) : _path(p), dangle(dangle) {}
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
	KeyboardPath(float dangle, key u, key d, key l, key r, key rl, key rr)
		: dangle(dangle), u(u), d(d), l(l), r(r), rl(rl), rr(rr) {}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms);

private:
	// % of a full rotation per second
	const float dangle;
	const key u, d, l, r, rl, rr;
};

// Standalone Path that has no movement
class StaticPath : public Path
{
public:
	StaticPath() : Path() {}
	virtual void move(float& x, float& y, float& angle, float& vel, float ms) override;
};
