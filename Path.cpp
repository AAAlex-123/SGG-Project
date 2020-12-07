#include "Path.h"
#include "graphics.h"
#include <cmath>

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
	// angle
	if (graphics::getKeyState(keyset.rleft)) angle += dangle * 2 * PI * (ms / 1000.0f);
	if (graphics::getKeyState(keyset.rright)) angle -= dangle * 2 * PI * (ms / 1000.0f);

	// position
	if (graphics::getKeyState(keyset.up)) {
		x -= vel * (float)sin(angle) * (ms / 1000.0f);
		y -= vel * (float)cos(angle) * (ms / 1000.0f);
	}
	if (graphics::getKeyState(keyset.down)) {
		x += vel * (float)sin(angle) * (ms / 1000.0f);
		y += vel * (float)cos(angle) * (ms / 1000.0f);
	}
	if (graphics::getKeyState(keyset.left)) {
		x -= vel * (float)sin(angle + PI / 2) * (ms / 1000.0f);
		y -= vel * (float)cos(angle + PI / 2) * (ms / 1000.0f);
	}
	if (graphics::getKeyState(keyset.right)) {
		x += vel * (float)sin(angle + PI / 2) * (ms / 1000.0f);
		y += vel * (float)cos(angle + PI / 2) * (ms / 1000.0f);
	}
}

void StaticPath::move(float& x, float& y, float& angle, float& vel, float ms)
{}
