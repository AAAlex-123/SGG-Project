#include "GObjFactory.h"
#include "Path.h"
#include "Player.h"
#include "drawing.h"
#include "entity.h"
#include "projectile.h"
#include "visual_effect.h"
#include <iostream>

using namespace std;
const std::list<Player*>* Factory::playerLs = nullptr; // declared here as well because c++ is dumb :(

const int Factory::b_plane_speed = 100;
const int Factory::b_plane_size = 20;
const int Factory::b_plane_dmg = 25;
const int Factory::b_plane_hp = 20;
const int Factory::b_plane_score = 100;

const int Factory::player_speed = 200;
const int Factory::player_w = (int)(b_plane_size * 3 / 2.4f);
const int Factory::player_h = (int)(b_plane_size * 3 / 1.2f);
const int Factory::player_hp = 150;
const float Factory::player_dangle = PI / 4.0f;

const int Factory::b_proj_speed = 350;
const int Factory::b_proj_size = 8;
const int Factory::b_proj_dmg = 10;

bool Factory::atLeastOne = false;

const Keyset Factory::pl1_kset = Keyset(key::SCANCODE_W, key::SCANCODE_S, key::SCANCODE_A, key::SCANCODE_D, key::SCANCODE_Q, key::SCANCODE_E, key::SCANCODE_SPACE);
const Keyset Factory::pl2_kset = Keyset(key::SCANCODE_UP, key::SCANCODE_DOWN, key::SCANCODE_LEFT, key::SCANCODE_RIGHT, key::SCANCODE_COMMA, key::SCANCODE_SLASH, key::SCANCODE_PERIOD);

void Factory::reset()
{
	atLeastOne = false;
}

const std::list<Player*>* const Factory::getPlayerData()
{
	return Factory::playerLs;
}

void Factory::setPlayerData(const std::list<Player*>* const ls)
{
	Factory::playerLs = ls;
}

// ===== ENTITY =====

Entity* Factory::createEntity(Factory::ENEMY type, float x, float y, float angle) {
	switch (type) 
	{
	case Factory::ENEMY::PLAYER:				return createPlayer(x, y, angle, player_dangle);
	case Factory::ENEMY::SIMPLE_ENEMY:			return createSimpleEnemy(x, y, angle);
	case Factory::ENEMY::SIMPLE_ENEMY_F:		return createSimpleFiringEnemy(x, y, angle);
	case Factory::ENEMY::ROTATING_ENEMY_D:		return createDonutEnemy(x, y, angle);
	case Factory::ENEMY::ROTATING_ENEMY_C:		return createRotatingEnemyClockwise(x, y, angle);
	case Factory::ENEMY::ROTATING_ENEMY_CA:		return createRotatingEnemyAnticlockwise(x, y, angle);
	case Factory::ENEMY::ACCELERATING_ENEMY:	return createAcceleratingEnemy(x, y, angle);
	case Factory::ENEMY::TANK_ENEMY:			return createTankEnemy(x, y, angle);
	case Factory::ENEMY::HOMING_ENEMY:			return createHomingEnemy(x, y, angle);
	default:
		std::cerr << "Factory::createEntity: what the actual fuck how did this even happen" << std::endl;
		return createSimpleEnemy(x, y, angle);
	}
}

Player* Factory::createPlayer(float x, float y, float angle, float dangle) {
	if (playerLs == nullptr)
		throw std::logic_error("No player list detected, use the setPlayerData() method before using the factory");

	switch (playerLs->size())
	{
	case 0:
		return new Player(x, y, angle, player_speed, player_w, player_h, new string(image_path + "player1"), dangle, 0.1f, pl1_kset, player_hp, Factory::BULLET::LIGHT_BULLET);
	case 1:
		return new Player(x, y, angle, player_speed, player_w, player_h, new string(image_path + "player2"), dangle, 0.1f, pl2_kset, player_hp, Factory::BULLET::LIGHT_BULLET);
	default:
		throw std::logic_error("Factory::createPlayer: what the actual fuck how did this even happen");
	}
}

Entity* Factory::createSimpleEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed, b_plane_size, b_plane_size, new string(image_path + "plane1"), new Path(), (int)(b_plane_dmg * 0.8f), (int)(b_plane_hp * 0.5f), b_plane_score, Factory::BULLET::STANDARD_BULLET);
}

Entity* Factory::createSimpleFiringEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed, b_plane_size, b_plane_size, new string(image_path + "plane1"), new FiringPath(1.8f, new Path()), b_plane_dmg, (int)(b_plane_hp * 0.5f), b_plane_score, Factory::BULLET::STANDARD_BULLET);
}

Entity* Factory::createDonutEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, player_speed * 0.3f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.5f, new RotatingPath(1.0f / 4.0f, new Path())), (int)(b_plane_dmg * 3), b_plane_hp * 2, (int)(b_plane_score * 0.5), Factory::BULLET::LIGHT_BULLET);
}

Entity* Factory::createRotatingEnemyClockwise(float x, float y, float angle)
{
	return new Entity(x, y, angle, player_speed * 0.7f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.8f, new RotatingPath(-1.0f / 12.0f, new Path())), (int)(b_plane_dmg * 1.2f), b_plane_hp * 2, b_plane_score * 5, Factory::BULLET::STANDARD_BULLET);
}

Entity* Factory::createRotatingEnemyAnticlockwise(float x, float y, float angle)
{
	return new Entity(x, y, angle, player_speed * 0.7f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.8f, new RotatingPath(1.0f / 12.0f, new Path())), (int)(b_plane_dmg * 1.2f), b_plane_hp * 2, b_plane_score * 5, Factory::BULLET::STANDARD_BULLET);
}

Entity* Factory::createAcceleratingEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed * 0.4f, b_plane_size * 1.2f, b_plane_size * 1.2f, new string(image_path + "plane2"), new AcceleratingPath(100.0f, new Path()), (int)(b_plane_dmg * 0.7f), (int)(b_plane_hp * 0.3f), b_plane_score * 3, Factory::BULLET::STANDARD_BULLET);
}

Entity* Factory::createTankEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed * 0.4f, b_plane_size * 1.3f, b_plane_size * 2, new string(image_path + "balloon"), new TargetedFiringPath(3.5f, new Path()), b_plane_dmg * 3, b_plane_hp * 4, b_plane_score * 2, Factory::BULLET::HEAVY_BULLET);
}

Entity* Factory::createHomingEnemy(float x, float y, float angle)
{
	return new Entity(x, y, angle, b_plane_speed * 0.65f, b_plane_size * 1.2f, b_plane_size * 1.2f, new string(image_path + "plane4"), new HomingPath(0.05f, new Path()), b_plane_dmg * 4, (int)(b_plane_hp * 2.5f), (int)(b_plane_score * 1.5f), Factory::BULLET::STANDARD_BULLET);
}


// ===== PROJECTILE =====

Projectile* Factory::createProjectile(Factory::BULLET type, float x, float y, float angle) {
	switch (type)
	{
	case Factory::BULLET::STANDARD_BULLET:	return createStandardBullet(x, y, angle);
	case Factory::BULLET::HEAVY_BULLET:		return createHeavyBullet(x, y, angle);
	case Factory::BULLET::LIGHT_BULLET:		return createLightBullet(x, y, angle);
	default:
		std::cerr << "Factory::createProjectile: what the actual fuck how did this even happen" << std::endl;
		return createStandardBullet(x, y, angle);
	}
}

Projectile* Factory::createStandardBullet(float x, float y, float angle)
{
	return new Projectile(x, y, angle, b_proj_speed * 1, b_proj_size * 1.1f, new string(image_path + "bullet1.png"), new Path(), (int)(b_proj_dmg * 0.9f));
}

Projectile* Factory::createHeavyBullet(float x, float y, float angle)
{
	return new Projectile(x, y, angle, b_proj_speed * 0.6f, b_proj_size * 1.3f, new string(image_path + "bullet3.png"), new Path(), (int)(b_proj_dmg * 2.2f));
}

Projectile* Factory::createLightBullet(float x, float y, float angle)
{
	return new Projectile(x, y, angle, b_proj_speed * 1.2f, b_proj_size * 0.8f, new string(image_path + "bullet2.png"), new Path(), (int)(b_proj_dmg * 0.5f));
}

// ===== VISUAL EFFECT =====

VisualEffect* Factory::createVisualEffect(Factory::EFFECT type, float x, float y, float angle, float duration, float fps) {
	switch (type)
	{
	case Factory::EFFECT::NOEFFECT:		return createNoEffect();
	case Factory::EFFECT::EXPLOSION:	return createExplosion(x, y, angle, duration, fps);
	case Factory::EFFECT::SMOKE:		return createSmoke(x, y, angle, duration);
	default:
		std::cerr << "Factory::createVisualEffect: what the actual fuck how did this even happen" << std::endl;
		return createNoEffect();
	}
}

VisualEffect* Factory::createExplosion(float x, float y, float angle, float duration, float fps)
{
	return new VisualEffect(x, y, angle, 0, 50,
		new string[7]{ image_path + "expl1.png", image_path + "expl2.png", image_path + "expl3.png",
		image_path + "expl4.png", image_path + "expl5.png", image_path + "expl6.png", image_path + "expl7.png"
		}, 7, duration, fps);
}

VisualEffect* Factory::createSmoke(float x, float y, float angle, float duration)
{
	return new VisualEffect(x, y, angle, 0, 40, new string[1]{ image_path + "expl6.png"}, 1, duration, 0.01f);
}

VisualEffect* Factory::createNoEffect()
{
	return new VisualEffect(0, 0, 0, 0, 0, new string[1]{ "" }, 0, 0.01f, 1.0f);
}
