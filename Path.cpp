#include "Path.h"
#include <iostream>

void Path::move(float& x, float& y, float& angle, float& vel, float ms)
{
	x -= vel * (float)sin(angle) * (ms / 1000.0f);
	y -= vel * (float)cos(angle) * (ms / 1000.0f);
}

void AcceleratingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	vel += dvel * (ms / 1000.0f);
	_path->move(x, y, angle, vel, ms);
}

void RotatingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	angle += dangle * 2 * PI * (ms / 1000.0f);
	_path->move(x, y, angle, vel, ms);
}

void KeyboardPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	// speed doesn't matter here
	// when player presses forward, he just moves forward

	// angle
	if (graphics::getKeyState(rl)) angle += dangle * 2 * PI * (ms / 1000.0f);
	if (graphics::getKeyState(rr)) angle -= dangle * 2 * PI * (ms / 1000.0f);

	// position
	if (graphics::getKeyState(u)) {
		x -= vel * (float)sin(angle) * (ms / 1000.0f);
		y -= vel * (float)cos(angle) * (ms / 1000.0f);
	}
	if (graphics::getKeyState(d)) {
		x += vel * (float)sin(angle) * (ms / 1000.0f);
		y += vel * (float)cos(angle) * (ms / 1000.0f);
	}
	if (graphics::getKeyState(l)) {
		x -= vel * (float)sin(angle + PI / 2) * (ms / 1000.0f);
		y -= vel * (float)cos(angle + PI / 2) * (ms / 1000.0f);
	}
	if (graphics::getKeyState(r)) {
		x += vel * (float)sin(angle + PI / 2) * (ms / 1000.0f);
		y += vel * (float)cos(angle + PI / 2) * (ms / 1000.0f);
	}
}

void StaticPath::move(float& x, float& y, float& angle, float& vel, float ms)
{}
