#pragma once
#include "globals.h"

// See "entity.h" for why the factory pattern was neccesary here

class Entity;
class Projectile;
class VisualEffect;
class Powerup;

// A Factory class that's used to assemble and produce all game objects in the game.
class GObjFactory {
private:
	// :knife:
	static bool atLeastOne;

	const static int b_plane_sp = 100;
	const static int b_plane_size = 20;
	const static int b_plane_dmg = 35;
	const static int b_plane_hp = 20;
	const static int b_plane_score = 100;

	const static int player_sp = 150;
	const static int player_w = (int) b_plane_size * 1.5;
	const static int player_h = (int) b_plane_size * 3;
	const static int player_hp = 100;
	const static float pl_dangle;

	const static int b_proj_sp = 350;
	const static int b_proj_size = 10;
	const static int b_proj_dmg = 10;

	const static Keyset pl1_kset;
	const static Keyset pl2_kset;

	static Entity* createPlayer(float x, float y, float angle, float dangle);

	static Entity* createSimpleEnemy(float x, float y, float angle);
	static Entity* createRotatingEnemy(float x, float y, float angle);
	static Entity* createAcceleratingEnemy(float x, float y, float angle);
	static Entity* createTankEnemy(float x, float y, float angle);

	static Projectile* createStandardBullet(float x, float y, float angle);
	static Projectile* createHeavyBullet(float x, float y, float angle);
	static Projectile* createLightBullet(float x, float y, float angle);

	static VisualEffect* createExplosion1(float x, float y, float angle, float duration, float fps);
	static VisualEffect* createSmoke(float x, float y, float angle, float duration);
	static VisualEffect* createNoEffect();

public:
	const static int STANDARD_BULLET = 10, HEAVY_BULLET = 11, LIGHT_BULLET = 12;
	const static int PLAYER = 0, SIMPLE_ENEMY = 1, ROTATING_ENEMY = 2, ACCELERATING_ENEMY = 3, TANK_ENEMY = 4;
	const static int NOEFFECT = 404, EXPLOSION_1 = 100, EXPLOSION_2 = 200, SMOKE = 300;

	static Entity* createEntity(int type, float x, float y, float angle);
	static Projectile* createProjectile(int type, float x, float y, float angle);
	// fps not needed when creating VisualEffects with only 1 sprite
	static VisualEffect* createVisualEffect(int type, float x, float y, float angle, float duration, float fps = 0.0f);
};

