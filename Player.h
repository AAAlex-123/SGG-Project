#pragma once
#include "entity.h"

class FiringPath;

/**
 * A subclass of Entity that can interact with powerups and has access to a unique path.
 */
class Player final : public Entity
{
public:
    Player(float xpos, float ypos, float angle, float vel, float width, float height, const std::string* sprite_name, float dangle, float cooldown,
        Keyset keys, int health, Factory::BULLET proj_type);

    // Changes the projectile type
    void upgradeProjectile();
    // Adds health, sometimes also buffs the max health
    void addHealth(int health);

    virtual ~Player() = default;

private:
    // Standalone Path that responds to Keyboard
    class KeyboardPath : public FiringPath
    {
    public:
        KeyboardPath(float dangle, float cooldown, Keyset keyset);
        virtual bool move(float& x, float& y, float& angle, float& vel, float ms) override;

    private:
        // % of a full rotation per second
        const float dangle;
        float remaining;
        const Keyset keyset;
    };
};
