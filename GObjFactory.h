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

/**
* A Factory class that is used to assemble and produce all game objects in the game.
*/
class GObjFactory
{
public:
	enum class ENEMY
	{
		PLAYER,
		SIMPLE_ENEMY,
		SIMPLE_ENEMY_F,
		ROTATING_ENEMY_D,
		ROTATING_ENEMY_C,
		ROTATING_ENEMY_CA,
		ACCELERATING_ENEMY,
		TANK_ENEMY,
		HOMING_ENEMY,
	};
	enum class BULLET
	{
		STANDARD_BULLET,
		HEAVY_BULLET,
		LIGHT_BULLET,
	};
	enum class EFFECT
	{
		NOEFFECT,
		EXPLOSION,
		SMOKE,
	};

	static Entity* createEntity(GObjFactory::ENEMY type, float x, float y, float angle);
	static Projectile* createProjectile(GObjFactory::BULLET type, float x, float y, float angle);
	static VisualEffect* createVisualEffect(GObjFactory::EFFECT type, float x, float y, float angle, float duration, float fps = 0.0f);
	
	static void reset();

	static const std::list<Player*>* const getPlayerData();
	// This could have been done with graphics::getUserData but this returns the whole GameData* therefore not protecting encapsulation.
	static void setPlayerData(const std::list<Player*>* const ls);

private:
	// keeps track of the players created
	static bool atLeastOne;

	// player data in order to supply created objects with necessary information. look at Path.h/cpp, specifically Homing- and TargetedFiring-Paths
	static const std::list<Player*>* playerLs;

	const static float player_speed, player_w, player_h, player_hp, player_dangle;
	const static float b_plane_speed, b_plane_size, b_plane_dmg, b_plane_hp, b_plane_score;
	const static float b_proj_speed, b_proj_size, b_proj_dmg;

	const static Keyset pl1_kset, pl2_kset;

	static Player* createPlayer(float x, float y, float angle, float dangle);
	static Entity* createSimpleEnemy(float x, float y, float angle);
	static Entity* createSimpleFiringEnemy(float x, float y, float angle);
	static Entity* createDonutEnemy(float x, float y, float angle);
	static Entity* createRotatingEnemyClockwise(float x, float y, float angle);
	static Entity* createRotatingEnemyAnticlockwise(float x, float y, float angle);
	static Entity* createAcceleratingEnemy(float x, float y, float angle);
	static Entity* createTankEnemy(float x, float y, float angle);
	static Entity* createHomingEnemy(float x, float y, float angle);

	static Projectile* createLightBullet(float x, float y, float angle);
	static Projectile* createStandardBullet(float x, float y, float angle);
	static Projectile* createHeavyBullet(float x, float y, float angle);

	static VisualEffect* createNoEffect();
	static VisualEffect* createExplosion(float x, float y, float angle, float duration, float fps);
	static VisualEffect* createSmoke(float x, float y, float angle, float duration);
};
