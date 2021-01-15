#include "Player.h"
#include "Factory.h"
#include "Path.h"
#include "UI.h" //access to UI's "hitbox"
#include <cmath>
#include <climits>

Player::Player(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* sprite_name, float dangle, float cooldown, Keyset keys,
    int health, Factory::BULLET proj_type)
    : Entity(xpos, ypos, angle, vel, width, height, sprite_name, new KeyboardPath(dangle, cooldown, keys), INT_MAX, health, 0, proj_type)
{}

void Player::upgradeProjectile()
{
    if (curr_projectile == Factory::BULLET::LIGHT_BULLET)
        curr_projectile = Factory::BULLET::STANDARD_BULLET;
    else if (curr_projectile == Factory::BULLET::STANDARD_BULLET)
        curr_projectile = Factory::BULLET::HEAVY_BULLET;   
}

void Player::addHealth(int health)
{
    curr_health += health;
    max_health = curr_health > max_health ? curr_health : max_health;
}

Player::KeyboardPath::KeyboardPath(float dangle, float cooldown, Keyset keyset) : FiringPath(cooldown, this), dangle(dangle), remaining(0.0f), keyset(keyset)
{}

bool Player::KeyboardPath::move(float& pl_x, float& pl_y, float& pl_angle, float& pl_vel, float ms)
{
    // angle
    if (graphics::getKeyState(keyset.rleft)) pl_angle += dangle * 2 * PI * (ms / 1000.0f);
    if (graphics::getKeyState(keyset.rright)) pl_angle -= dangle * 2 * PI * (ms / 1000.0f);
    
    this->curr_angle = pl_angle;

    // position
    float temp_x = pl_x;
    float temp_y = pl_y;

    if (graphics::getKeyState(keyset.up)) {
        temp_x -= pl_vel * (float)sin(pl_angle) * (ms / 1000.0f);
        temp_y -= pl_vel * (float)cos(pl_angle) * (ms / 1000.0f);
    }
    if (graphics::getKeyState(keyset.down)) {
        temp_x += pl_vel * (float)sin(pl_angle) * (ms / 1000.0f);
        temp_y += pl_vel * (float)cos(pl_angle) * (ms / 1000.0f);
    }
    if (graphics::getKeyState(keyset.left)) {
        temp_x -= pl_vel * (float)sin((double)pl_angle + PI / 2) * (ms / 1000.0f);
        temp_y -= pl_vel * (float)cos((double)pl_angle + PI / 2) * (ms / 1000.0f);
    }
    if (graphics::getKeyState(keyset.right)) {
        temp_x += pl_vel * (float)sin((double)pl_angle + PI / 2) * (ms / 1000.0f);
        temp_y += pl_vel * (float)cos((double)pl_angle + PI / 2) * (ms / 1000.0f);
    }

    // if the new position is outside of the screen, don't update it
    if (temp_x >= get_canvas_width() * 0.023f && temp_x <= get_canvas_width() * 0.977f)
        pl_x = temp_x;
    if (temp_y >= get_canvas_height() * 0.05f && temp_y <= get_canvas_height() - UI::box_height * 2.98f)
        pl_y = temp_y;

    // decrease or keep at 0
    remaining = (remaining <= 0.0f) ? (0.0f) : (remaining - (ms / 1000.0f));
    // reset if it is 0 and fire is being pressed
    remaining = (remaining == 0.0f && graphics::getKeyState(keyset.fire)) ? (period) : (remaining);

    return (graphics::getKeyState(keyset.fire)) && (remaining == period);
}
