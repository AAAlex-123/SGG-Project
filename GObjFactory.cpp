#include "GObjFactory.h"
#include "Path.h"
#include "Player.h"
#include "drawing.h"
#include "entity.h"
#include "projectile.h"
#include "visual_effect.h"
#include <iostream>

using namespace std;

const std::list<Entity*>* GObjFactory::playerLs = nullptr; //declared here as well because c++ is dumb

const float GObjFactory::b_plane_sp = 100.f;
const float GObjFactory::b_plane_size = 20.f;
const int GObjFactory::b_plane_dmg = 25;
const int GObjFactory::b_plane_hp = 20;
const int GObjFactory::b_plane_score = 100;

const float GObjFactory::player_sp = 200.f;
const float GObjFactory::player_w = b_plane_size * 1.5f / 1.2f;
const float GObjFactory::player_h = b_plane_size * 3.f / 1.2f;
const int GObjFactory::player_hp = 150;
const float GObjFactory::pl_dangle = PI / 4.0f;

const float GObjFactory::b_proj_sp = 350.f;
const float GObjFactory::b_proj_size = 8.f;
const int GObjFactory::b_proj_dmg = 10;

// spaghetti that will stay here forever
bool GObjFactory::atLeastOne = false;

const Keyset GObjFactory::pl1_kset = Keyset(key::SCANCODE_W, key::SCANCODE_S, key::SCANCODE_A, key::SCANCODE_D, key::SCANCODE_Q, key::SCANCODE_E, key::SCANCODE_X);
const Keyset GObjFactory::pl2_kset = Keyset(key::SCANCODE_UP, key::SCANCODE_DOWN, key::SCANCODE_LEFT, key::SCANCODE_RIGHT, key::SCANCODE_PERIOD, key::SCANCODE_COMMA, key::SCANCODE_SPACE);
//unused: Keyset(key::SCANCODE_T, key::SCANCODE_G, key::SCANCODE_F, key::SCANCODE_H, key::SCANCODE_R, key::SCANCODE_Y, key::SCANCODE_B);

// ===== ENTITY =====

Entity* GObjFactory::createEntity(int type, float x, float y, float angle) {
	switch (type) {
	case GObjFactory::PLAYER: return createPlayer(x, y, angle, pl_dangle);
	case GObjFactory::SIMPLE_ENEMY: return createSimpleEnemy(x, y, angle);
	case GObjFactory::SIMPLE_ENEMY2: return createSimpleFiringEnemy(x, y, angle);
	case GObjFactory::ROTATING_ENEMYD: return createRotatingEnemyD(x, y, angle);
	case GObjFactory::ROTATING_ENEMYC: return createRotatingEnemyC(x, y, angle);
	case GObjFactory::ROTATING_ENEMYCA: return createRotatingEnemyCA(x, y, angle);
	case GObjFactory::ACCELERATING_ENEMY: return createAcceleratingEnemy(x, y, angle);
	case GObjFactory::TANK_ENEMY: return createTankEnemy(x, y, angle);
	case GObjFactory::HOMING_ENEMY: return createHomingEnemy(x, y, angle);
	default:
		std::cerr << "Invalid constant, creating enemy1 as placeholder" << std::endl;
		return createSimpleEnemy(x, y, angle);
	}
}

Player* GObjFactory::createPlayer(float x, float y, float angle, float dangle) {
	if (!atLeastOne) {
		//the factory needs the player data in order to supply created objects with necessary information
		if (playerLs == nullptr) 
			throw std::logic_error("No player list detected, use the setPlayerData() method before using the factory");

		atLeastOne = true;
		return new Player(x, y, angle, player_sp, player_w, player_h, new string(image_path + "player1"), dangle, 0.1f, pl1_kset, player_hp, GObjFactory::LIGHT_BULLET);
	}
	else
		return new Player(x, y, angle, player_sp, player_w, player_h, new string(image_path + "player2"), dangle, 0.1f, pl2_kset, player_hp, GObjFactory::LIGHT_BULLET);
}

// SIMPLE_ENEMY = 1,		SIMPLE_ENEMY2 = 7,		ROTATING_ENEMYD = 2,	ROTATING_ENEMYCA = 6
// ROTATING_ENEMYC = 8,		ACCELERATING_ENEMY = 3, TANK_ENEMY = 4,			HOMING_ENEMY = 5;

Entity* GObjFactory::createSimpleEnemy(float x, float y, float angle) {
	return new Entity(x, y, angle, b_plane_sp, b_plane_size, b_plane_size, new string(image_path + "plane1"), new Path(), b_plane_dmg * 0.8f, b_plane_hp * 0.5f, b_plane_score, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createSimpleFiringEnemy(float x, float y, float angle) {
	return new Entity(x, y, angle, b_plane_sp, b_plane_size, b_plane_size, new string(image_path + "plane1"), new FiringPath(1.8f, new Path()), b_plane_dmg, b_plane_hp * 0.5f, b_plane_score, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createRotatingEnemyD(float x, float y, float angle) {
	return new Entity(x, y, angle, player_sp * 0.3f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.5f, new RotatingPath(1.0f / 4.0f, new Path())), b_plane_dmg * 3, b_plane_hp * 2.0f, b_plane_score * 0.5, GObjFactory::LIGHT_BULLET);
}

Entity* GObjFactory::createRotatingEnemyC(float x, float y, float angle) {
	return new Entity(x, y, angle, player_sp * 0.7f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.8f, new RotatingPath(-1.0f / 12.0f, new Path())), b_plane_dmg * 1.2f, b_plane_hp * 2, b_plane_score * 5.0f, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createRotatingEnemyCA(float x, float y, float angle) {
	return new Entity(x, y, angle, player_sp * 0.7f, player_w, player_h, new string(image_path + "plane3"), new TargetedFiringPath(1.8f, new RotatingPath(1.0f / 12.0f, new Path())), b_plane_dmg * 1.2f, b_plane_hp * 2, b_plane_score * 5.0f, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createAcceleratingEnemy(float x, float y, float angle) {
	return new Entity(x, y, angle, b_plane_sp * 0.4f, b_plane_size * 1.2f, b_plane_size * 1.2f, new string(image_path + "plane2"), new AcceleratingPath(100.0f, new Path()), b_plane_dmg * 0.7f, b_plane_hp * 0.3f, b_plane_score * 3, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createTankEnemy(float x, float y, float angle) {
	return new Entity(x, y, angle, b_plane_sp * 0.4f, b_plane_size * 1.3f, b_plane_size * 2.0f, new string(image_path + "balloon"), new TargetedFiringPath(3.5f, new Path()), b_plane_dmg * 3.0f, b_plane_hp * 4, b_plane_score * 2, GObjFactory::HEAVY_BULLET);
}

Entity* GObjFactory::createHomingEnemy(float x, float y, float angle) {
	Entity* target = rand() % (playerLs->size()) == 0 ? playerLs->front() : playerLs->back();
	return new Entity(x, y, angle, b_plane_sp * 0.65f, b_plane_size * 1.2f, b_plane_size * 1.2f, new string(image_path + "bomb"), new HomingPath(0.05f, new Path()), b_plane_dmg * 4.0f, b_plane_hp * 2.5f, b_plane_score * 1.5f, GObjFactory::STANDARD_BULLET);
}


// ===== PROJECTILE =====

Projectile* GObjFactory::createProjectile(int type, float x, float y, float angle) {
	switch (type) {
	case GObjFactory::STANDARD_BULLET: return createStandardBullet(x, y, angle);
	case GObjFactory::HEAVY_BULLET: return createHeavyBullet(x, y, angle);
	case GObjFactory::LIGHT_BULLET: return createLightBullet(x, y, angle);
	default:
		std::cerr << "Invalid constant, creating standard bullet as placeholder" << std::endl;
		return createStandardBullet(x, y, angle);
	}
}

Projectile* GObjFactory::createStandardBullet(float x, float y, float angle) {			   																  
	return new Projectile(x, y, angle, b_proj_sp * 1.0f, b_proj_size * 1.1f, new string(image_path + "bullet1.png"), new Path(), b_proj_dmg * 0.9f);
}

Projectile* GObjFactory::createHeavyBullet(float x, float y, float angle) {
	return new Projectile(x, y, angle, b_proj_sp * 0.6f, b_proj_size * 1.3f, new string(image_path + "bullet3.png"), new Path(), b_proj_dmg * 2.2f);
}

Projectile* GObjFactory::createLightBullet(float x, float y, float angle) {
	return new Projectile(x, y, angle, b_proj_sp * 1.2f, b_proj_size * 0.8f, new string(image_path + "bullet2.png"), new Path(), b_proj_dmg * 0.5f);
}

// ===== VISUAL EFFECT =====

VisualEffect* GObjFactory::createVisualEffect(int type, float x, float y, float angle, float duration, float fps) {
	switch (type) {
	case GObjFactory::EXPLOSION_1: return createExplosion1(x, y, angle, duration, fps);
	case GObjFactory::SMOKE: return createSmoke(x, y, angle, duration);
	case GObjFactory::NOEFFECT: return createNoEffect();
	default:
		std::cerr << "Invalid constant, creating smoke as placeholder" << std::endl;
		return createSmoke(x, y, angle, duration);
	}
}

VisualEffect* GObjFactory::createExplosion1(float x, float y, float angle, float duration, float fps) {
	return new VisualEffect(x, y, angle, 0, 50,
		new string[7]{ image_path + "expl1.png", image_path + "expl2.png", image_path + "expl3.png",
		image_path + "expl4.png", image_path + "expl5.png", image_path + "expl6.png", image_path + "expl7.png"
		}, 7, duration, fps);
}

VisualEffect* GObjFactory::createSmoke(float x, float y, float angle, float duration) {
	return new VisualEffect(x, y, angle, 0, 40,
		new string[1]{ image_path + "expl6.png"
		}, 1, duration, 0.01f);
}

VisualEffect* GObjFactory::createNoEffect() {
	return new VisualEffect(0, 0, 0, 0, 0, new string[1]{ "" }, 0, 0.01f, 1.0f);
}
