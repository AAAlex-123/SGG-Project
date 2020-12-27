#include "Path.h"
#include "drawing.h"
#include <cmath>

bool Path::move(float& x, float& y, float& angle, float& vel, float ms)
{
	if (angle > 180.0f)
		angle -= 360.0f;
	else if (angle < -180.0f)
		angle += 360.0f;
	x -= vel * (float)sin(angle) * (ms / 1000.0f);
	y -= vel * (float)cos(angle) * (ms / 1000.0f);
	return false;
}

bool AcceleratingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	vel += dvel * (ms / 1000.0f);

	return _path->move(x, y, angle, vel, ms);
}

bool RotatingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	angle += dangle * 2 * PI * (ms / 1000.0f);

	return _path->move(x, y, angle, vel, ms);
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
	// decrease cooldown or keep at 0
	remaining = ((0.0f) * (remaining <= 0.0f)) + ((remaining - (ms / 1000.0f)) * (remaining > 0.0f));
	// reset if it is 0 and fire is being pressed
	remaining = ((cooldown) * (remaining == 0.0f && graphics::getKeyState(keyset.fire))) + ((remaining) * !(remaining == 0.0f && graphics::getKeyState(keyset.fire)));
	
	return (graphics::getKeyState(keyset.fire)) && (remaining == cooldown);
}

bool FiringPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	elapsed += (ms / 1000.0f);
	elapsed = (0.0f * (elapsed >= period)) + (elapsed * (elapsed < period));

	_path->move(x, y, angle, vel, ms);

	return elapsed == 0.0f;
}

bool TargetedFiringPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	elapsed += (ms / 1000.0f);
	elapsed = (0.0f * (elapsed >= period)) + (elapsed * (elapsed < period));

	_path->move(x, y, angle, vel, ms);

	return elapsed == 0.0f;
}

bool HomingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	// mafs

	float slope = atan2((followee->get_x() - x), (followee->get_y() - y));

	float dangle = -(angle - slope + PI);

	angle += (dangle < -PI) ? ((dangle + 2 * PI) * perc) : (dangle * perc);

	return _path->move(x, y, angle, vel, ms);
}

bool StaticPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
	return false;
}
