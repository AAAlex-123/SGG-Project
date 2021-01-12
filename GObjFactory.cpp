#include "GObjFactory.h"
#include "Path.h"
#include "Player.h"
#include "drawing.h"
#include "entity.h"
#include "projectile.h"
#include "visual_effect.h"
#include <iostream>

using namespace std;
const std::list<Player*>* GObjFactory::playerLs = nullptr; //declared here as well because c++ is dumb :(

const float GObjFactory::b_plane_speed = 100.f;
const float GObjFactory::b_plane_size = 20.f;
const float GObjFactory::b_plane_dmg = 25.0f;
const float GObjFactory::b_plane_hp = 20.0f;
const float GObjFactory::b_plane_score = 100.0f;

const float GObjFactory::player_speed = 200.f;
const float GObjFactory::player_w = b_plane_size * 1.5f / 1.2f;
const float GObjFactory::player_h = b_plane_size * 3.f / 1.2f;
const float GObjFactory::player_hp = 150.0f;
const float GObjFactory::player_dangle = PI / 4.0f;

const float GObjFactory::b_proj_speed = 350.f;
const float GObjFactory::b_proj_size = 8.f;
const float GObjFactory::b_proj_dmg = 10.0f;

bool GObjFactory::atLeastOne = false;

const Keyset GObjFactory::pl1_kset = Keyset(key::SCANCODE_W, key::SCANCODE_S, key::SCANCODE_A, key::SCANCODE_D, key::SCANCODE_Q, key::SCANCODE_E, key::SCANCODE_X);
const Keyset GObjFactory::pl2_kset = Keyset(key::SCANCODE_UP, key::SCANCODE_DOWN, key::SCANCODE_LEFT, key::SCANCODE_RIGHT, key::SCANCODE_PERIOD, key::SCANCODE_COMMA, key::SCANCODE_SPACE);

void GObjFactory::reset()
{
	atLeastOne = false;
}

const std::list<Player*>* const GObjFactory::getPlayerData()
{
	return GObjFactory::playerLs;
}

void GObjFactory::setPlayerData(const std::list<Player*>* const ls)
{
	GObjFactory::playerLs = ls;
}

// ===== ENTITY =====

Entity* GObjFactory::createEntity(GObjFactory::ENEMY type, float x, float y, float angle) {
	switch (type) 
	{
	case GObjFactory::ENEMY::PLAYER:				return createPlayer(x, y, angle, player_dangle);
	case GObjFactory::ENEMY::SIMPLE_ENEMY:			return createSimpleEnemy(x, y, angle);
	case GObjFactory::ENEMY::SIMPLE_ENEMY_F:		return createSimpleFiringEnemy(x, y, angle);
	case GObjFactory::ENEMY::ROTATING_ENEMY_D:		return createDonutEnemy(x, y, angle);
	case GObjFactory::ENEMY::ROTATING_ENEMY_C:		return createRotatingEnemyClockwise(x, y, angle);
	case GObjFactory::ENEMY::ROTATING_ENEMY_CA:		return createRotatingEnemyAnticlockwise(x, y, angle);
	case GObjFactory::ENEMY::ACCELERATING_ENEMY:	return createAcceleratingEnemy(x, y, angle);
	case GObjFactory::ENEMY::TANK_ENEMY:			return createTankEnemy(x, y, angle);
	case GObjFactory::ENEMY::HOMING_ENEMY:			return createHomingEnemy(x, y, angle);
	default:
		std::cerr << "GObjFactory::createEntity: what the actual fuck how did this even happen" << std::endl;
		return createSimpleEnemy(x, y, angle);
	}
}

Player* GObjFactory::createPlayer(float x, float y, float angle, float dangle) {
	if (playerLs == nullptr)
		throw std::logic_error("No player list detected, use the setPlayerData() method before using the factory");

	switch (playerLs->size())
	{
	case 0:
		return new Player(x, y, angle, player_speed, player_w, player_h, new string(image_path + "player1"), dangle, 0.1f, pl1_kset, player_hp, GObjFactory::BULLET::LIGHT_BULLET);
	case 1:
		return new Player(x, y, angle, player_speed, player_w, player_h, new string(image_path + "player2"), dangle, 0.1f, pl2_kset, player_hp, GObjFactory::BULLET::LIGHT_BULLET);
	default:
		throw std::logic_error("GObjFactory::createPlayer: what the actual fuck how did this even happen");
	}
}

Entity* GObjFactory::createSimpleEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed, b_plane_size, b_plane_size, new string(image_path + "plane1"), new Path(), b_plane_dmg * 0.8f, b_plane_hp * 0.5f, b_plane_score, GObjFactory::BULLET::STANDARD_BULLET);
}

Entity* GObjFactory::createSimpleFiringEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed, b_plane_size, b_plane_size, new string(image_path + "plane1"), new FiringPath(1.8f, new Path()), b_plane_dmg, b_plane_hp * 0.5f, b_plane_score, GObjFactory::BULLET::STANDARD_BULLET);
}

Entity* GObjFactory::createDonutEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, player_speed * 0.3f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.5f, new RotatingPath(1.0f / 4.0f, new Path())), b_plane_dmg * 3, b_plane_hp * 2.0f, b_plane_score * 0.5, GObjFactory::BULLET::LIGHT_BULLET);
}

Entity* GObjFactory::createRotatingEnemyClockwise(float x, float y, float angle)
{
	return new Entity(x, y, angle, player_speed * 0.7f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.8f, new RotatingPath(-1.0f / 12.0f, new Path())), b_plane_dmg * 1.2f, b_plane_hp * 2, b_plane_score * 5.0f, GObjFactory::BULLET::STANDARD_BULLET);
}

Entity* GObjFactory::createRotatingEnemyAnticlockwise(float x, float y, float angle)
{
	return new Entity(x, y, angle, player_speed * 0.7f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.8f, new RotatingPath(1.0f / 12.0f, new Path())), b_plane_dmg * 1.2f, b_plane_hp * 2, b_plane_score * 5.0f, GObjFactory::BULLET::STANDARD_BULLET);
}

Entity* GObjFactory::createAcceleratingEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed * 0.4f, b_plane_size * 1.2f, b_plane_size * 1.2f, new string(image_path + "plane2"), new AcceleratingPath(100.0f, new Path()), b_plane_dmg * 0.7f, b_plane_hp * 0.3f, b_plane_score * 3, GObjFactory::BULLET::STANDARD_BULLET);
}

Entity* GObjFactory::createTankEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed * 0.4f, b_plane_size * 1.3f, b_plane_size * 2.0f, new string(image_path + "balloon"), new TargetedFiringPath(3.5f, new Path()), b_plane_dmg * 3.0f, b_plane_hp * 4, b_plane_score * 2, GObjFactory::BULLET::HEAVY_BULLET);
}

Entity* GObjFactory::createHomingEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed * 0.65f, b_plane_size * 1.2f, b_plane_size * 1.2f, new string(image_path + "plane4"), new HomingPath(0.05f, new Path()), b_plane_dmg * 4.0f, b_plane_hp * 2.5f, b_plane_score * 1.5f, GObjFactory::BULLET::STANDARD_BULLET);
}


// ===== PROJECTILE =====

Projectile* GObjFactory::createProjectile(GObjFactory::BULLET type, float x, float y, float angle) {
	switch (type)
	{
	case GObjFactory::BULLET::STANDARD_BULLET:	return createStandardBullet(x, y, angle);
	case GObjFactory::BULLET::HEAVY_BULLET:		return createHeavyBullet(x, y, angle);
	case GObjFactory::BULLET::LIGHT_BULLET:		return createLightBullet(x, y, angle);
	default:
		std::cerr << "GObjFactory::createProjectile: what the actual fuck how did this even happen" << std::endl;
		return createStandardBullet(x, y, angle);
	}
}

Projectile* GObjFactory::createStandardBullet(float x, float y, float angle)
{
	return new Projectile(x, y, angle, b_proj_speed * 1.0f, b_proj_size * 1.1f, new string(image_path + "bullet1.png"), new Path(), b_proj_dmg * 0.9f);
}

Projectile* GObjFactory::createHeavyBullet(float x, float y, float angle)
{
	return new Projectile(x, y, angle, b_proj_speed * 0.6f, b_proj_size * 1.3f, new string(image_path + "bullet3.png"), new Path(), b_proj_dmg * 2.2f);
}

Projectile* GObjFactory::createLightBullet(float x, float y, float angle)
{
	return new Projectile(x, y, angle, b_proj_speed * 1.2f, b_proj_size * 0.8f, new string(image_path + "bullet2.png"), new Path(), b_proj_dmg * 0.5f);
}

// ===== VISUAL EFFECT =====

VisualEffect* GObjFactory::createVisualEffect(GObjFactory::EFFECT type, float x, float y, float angle, float duration, float fps) {
	switch (type)
	{
	case GObjFactory::EFFECT::NOEFFECT:		return createNoEffect();
	case GObjFactory::EFFECT::EXPLOSION:	return createExplosion(x, y, angle, duration, fps);
	case GObjFactory::EFFECT::SMOKE:		return createSmoke(x, y, angle, duration);
	default:
		std::cerr << "GObjFactory::createVisualEffect: what the actual fuck how did this even happen" << std::endl;
		return createNoEffect();
	}
}

VisualEffect* GObjFactory::createExplosion(float x, float y, float angle, float duration, float fps)
{
	return new VisualEffect(x, y, angle, 0, 50,
		new string[7]{ image_path + "expl1.png", image_path + "expl2.png", image_path + "expl3.png",
		image_path + "expl4.png", image_path + "expl5.png", image_path + "expl6.png", image_path + "expl7.png"
		}, 7, duration, fps);
}

VisualEffect* GObjFactory::createSmoke(float x, float y, float angle, float duration)
{
	return new VisualEffect(x, y, angle, 0, 40, new string[1]{ image_path + "expl6.png"}, 1, duration, 0.01f);
}

VisualEffect* GObjFactory::createNoEffect()
{
	return new VisualEffect(0, 0, 0, 0, 0, new string[1]{ "" }, 0, 0.01f, 1.0f);
}
