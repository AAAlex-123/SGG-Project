#include "Path.h"
#include <cmath>

bool Path::move(float& x, float& y, float& angle, float& vel, float ms)
{
	x -= vel * (float)sin(angle) * (ms / 1000.0f);
	y -= vel * (float)cos(angle) * (ms / 1000.0f);
	return false;
}

bool AcceleratingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	vel += dvel * (ms / 1000.0f);
	_path->move(x, y, angle, vel, ms);
	return false;
}

bool RotatingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	angle += dangle * 2 * PI * (ms / 1000.0f);
	_path->move(x, y, angle, vel, ms);
	return false;
}

bool KeyboardPath::move(float& x, float& y, float& angle, float& vel, float ms)
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
	
	return graphics::getKeyState(keyset.fire);
}

bool FiringPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	elapsed += (ms / 1000.0f);
	float e = elapsed;
	elapsed = (0 * (elapsed >= period)) + (elapsed * (elapsed < period));

	_path->move(x, y, angle, vel, ms);

	return e >= period;
}

bool StaticPath::move(float& x, float& y, float& angle, float& vel, float ms) { return false; }
