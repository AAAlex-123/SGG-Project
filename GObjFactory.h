#pragma once
#include "globals.h"
#include <list>

// See "entity.h" for why the factory pattern was neccesary here

class Drawing;
class Entity;
class Projectile;
class VisualEffect;
class Powerup;
class Player;

// A Factory class that's used to assemble and produce all game objects in the game.
class GObjFactory {
private:
	static bool atLeastOne;
	static const std::list<Entity*>* playerLs;

	const static float b_plane_sp, b_plane_size;
	const static int b_plane_dmg, b_plane_hp, b_plane_score, b_proj_dmg, player_hp;
	const static float player_sp, player_w, player_h, pl_dangle;
	const static float b_proj_sp, b_proj_size;

	const static Keyset pl1_kset;
	const static Keyset pl2_kset;

	static Player* createPlayer(float x, float y, float angle, float dangle);
	static Entity* createSimpleEnemy(float x, float y, float angle);
	static Entity* createSimpleFiringEnemy(float x, float y, float angle);
	static Entity* createRotatingEnemyD(float x, float y, float angle);
	static Entity* createRotatingEnemyC(float x, float y, float angle);
	static Entity* createRotatingEnemyCA(float x, float y, float angle);
	static Entity* createAcceleratingEnemy(float x, float y, float angle);
	static Entity* createTankEnemy(float x, float y, float angle);
	static Entity* createHomingEnemy(float x, float y, float angle);

	static Projectile* createStandardBullet(float x, float y, float angle);
	static Projectile* createHeavyBullet(float x, float y, float angle);
	static Projectile* createLightBullet(float x, float y, float angle);

	static VisualEffect* createExplosion1(float x, float y, float angle, float duration, float fps);
	static VisualEffect* createSmoke(float x, float y, float angle, float duration);
	static VisualEffect* createNoEffect();

public:
	const static int STANDARD_BULLET = 10, HEAVY_BULLET = 11, LIGHT_BULLET = 12;
	const static int PLAYER = 0, SIMPLE_ENEMY = 1, SIMPLE_ENEMY2 = 7, ROTATING_ENEMYD = 2, ROTATING_ENEMYC = 8, ROTATING_ENEMYCA = 6, ACCELERATING_ENEMY = 3, TANK_ENEMY = 4, HOMING_ENEMY = 5;
	const static int NOEFFECT = 404, EXPLOSION_1 = 100, EXPLOSION_2 = 200, SMOKE = 300;

	static Entity* createEntity(int type, float x, float y, float angle);
	static void reset() {atLeastOne = false; playerLs = nullptr; }
	// might be used for creating stuff at run-time from game_data where the Drawing* is known (powerups?)
	// static Entity* createEntity(int type, float x, float y, float angle, Drawing* followee);
	static Projectile* createProjectile(int type, float x, float y, float angle);
	// fps not needed when creating VisualEffects with only 1 sprite
	static VisualEffect* createVisualEffect(int type, float x, float y, float angle, float duration, float fps = 0.0f);

	static const std::list<Entity*>* getPlayerData() {
		return playerLs;
	}

	//Supply the factory with a pointer to a list which contains the players at the start of using the factory, and after successive reset.
	static void setPlayerData(const std::list<Entity*>* ls) {
		playerLs = ls;
	}
};
