#include "Path.h"
#include "UI.h" //access to UI's "hitbox"
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
    float temp_x = x;
    float temp_y = y;
    if (graphics::getKeyState(keyset.up)) {
        temp_x -= vel * (float)sin(angle) * (ms / 1000.0f);
        temp_y -= vel * (float)cos(angle) * (ms / 1000.0f);
    }
    if (graphics::getKeyState(keyset.down)) {
        temp_x += vel * (float)sin(angle) * (ms / 1000.0f);
        temp_y += vel * (float)cos(angle) * (ms / 1000.0f);
    }
    if (graphics::getKeyState(keyset.left)) {
        temp_x -= vel * (float)sin(angle + PI / 2) * (ms / 1000.0f);
        temp_y -= vel * (float)cos(angle + PI / 2) * (ms / 1000.0f);
    }
    if (graphics::getKeyState(keyset.right)) {
        temp_x += vel * (float)sin(angle + PI / 2) * (ms / 1000.0f);
        temp_y += vel * (float)cos(angle + PI / 2) * (ms / 1000.0f);
    }

    if (temp_x >= 0 && temp_x <= get_canvas_width())
        x = temp_x;
    if (temp_y >= 0 && temp_y <= get_canvas_height() - UI::box_height *2)
        y = temp_y;

    // decrease or keep at 0
    remaining = ((0.0f) * (remaining <= 0.0f)) + ((remaining - (ms / 1000.0f)) * (remaining > 0.0f));
    // reset if it is 0 and fire is being pressed
    remaining = ((cooldown) * (remaining == 0.0f && graphics::getKeyState(keyset.fire))) + ((remaining) * !(remaining == 0.0f && graphics::getKeyState(keyset.fire)));

    return (graphics::getKeyState(keyset.fire)) && (remaining == cooldown);
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
