#include "Path.h"
#include "drawing.h"
#include "Player.h"
#include "GObjFactory.h"
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


bool FiringPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
    this->curr_angle = angle;
    elapsed += (ms / 1000.0f);
    elapsed = (0.0f * (elapsed >= period)) + (elapsed * (elapsed < period));

    _path->move(x, y, angle, vel, ms);

    return elapsed == 0.0f;
}

float FiringPath::getProjAngle(float x, float y) {
    return this->curr_angle;
}

//A function that picks the closest player in relation to the entity
const Player* const find_target(float x, float y,const std::list<Player*>* const ls) {

    auto distance = [](float x1, float y1, float x2, float y2) {
        return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1));
    };

    if (ls->size() == 1)
        return ls->front();

    Player* pl1 = ls->front();
    Player* pl2 = ls->back();
    return (distance(pl1->get_x(), pl1->get_y(), x, y) <= distance(pl2->get_x(), pl2->get_y(), x, y)) ? pl1 : pl2;
    
}

float TargetedFiringPath::getProjAngle(float x, float y) {
    const Player* const d = find_target(x, y, GObjFactory::getPlayerData());
    return atan2((d->get_x() - x), (d->get_y() - y)) + PI;
}

bool HomingPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
    // mafs
    const Player* const followee = find_target(x, y, GObjFactory::getPlayerData());

    float slope = atan2((followee->get_x() - x), (followee->get_y() - y));

    float dangle = -(angle - slope + PI);

    angle += (dangle < -PI) ? ((dangle + 2 * PI) * perc) : (dangle * perc);

    return _path->move(x, y, angle, vel, ms);
}

bool StaticPath::move(float& x, float& y, float& angle, float& vel, float ms)
{
    return false;
}
