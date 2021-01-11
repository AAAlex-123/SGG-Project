#include "Player.h"
#include "GObjFactory.h"
#include "Path.h"
#include "UI.h" //access to UI's "hitbox"

Player::Player(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* sprite_name, float dangle, float cooldown, Keyset keys,
    int health, GObjFactory::BULLET proj_type)
    : Entity(xpos, ypos, angle, vel, width, height, sprite_name, new KeyboardPath(dangle, cooldown, keys), INT_MAX, health, 0, proj_type)
{}

void Player::incrementProjectile() {
    if (curr_projectile == GObjFactory::BULLET::LIGHT_BULLET)
        curr_projectile = GObjFactory::BULLET::STANDARD_BULLET;
    else if (curr_projectile == GObjFactory::BULLET::STANDARD_BULLET)
        curr_projectile = GObjFactory::BULLET::HEAVY_BULLET;   
}

void Player::addHealth(int health) {
    curr_health += health;
    max_health = curr_health > max_health ? curr_health : max_health;
}

bool Player::KeyboardPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
    // angle
    if (graphics::getKeyState(keyset.rleft)) angle += dangle * 2 * PI * (ms / 1000.0f);
    if (graphics::getKeyState(keyset.rright)) angle -= dangle * 2 * PI * (ms / 1000.0f);
    this->curr_angle = angle;

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
    if (temp_y >= 0 && temp_y <= get_canvas_height() - UI::box_height * 2)
        y = temp_y;


    // decrease or keep at 0
    remaining = ((0.0f) * (remaining <= 0.0f)) + ((remaining - (ms / 1000.0f)) * (remaining > 0.0f));
    // reset if it is 0 and fire is being pressed
    remaining = ((period) * (remaining == 0.0f && graphics::getKeyState(keyset.fire))) + ((remaining) * !(remaining == 0.0f && graphics::getKeyState(keyset.fire)));

    return (graphics::getKeyState(keyset.fire)) && (remaining == period);
}
