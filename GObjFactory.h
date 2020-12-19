#pragma once
#include "entity.h"
#include "projectile.h"
#include "globals.h"

//See "entity.h" for why the factory pattern was neccesary here

//A Factory class that's used to assemble and produce all game objects in the game.
class GObjFactory {
private:
	const static int plane_size = 46;
	const static int zep_width = 20;
	const static int zep_height = 43;
	const static int proj_size = 20;
	const static int std_speed = 120;
  
	static Entity* createEnemy1(float x, float y, float angle);
	static Entity* createEnemy2(float x, float y, float angle);
	static Entity* createEnemy3(float x, float y, float angle);
	static Entity* createPlayer(float x, float y, float angle, float dangle, Keyset keyset);

	static Projectile* createStdB(float x, float y, float angle);
	static Projectile* createLB(float x, float y, float angle);
	static Projectile* createIncB(float x, float y, float angle);

public:
	const static int STANDARD_BULLET = 10, LIGHT_BULLET = 11, INCEND_BULLET = 12;
	const static int PLAYER = 0, ENEMY_1 = 1, ENEMY_2 = 2, ENEMY_3 = 3;

	static Entity* createEntity(int type, float x, float y, float angle, float dangle = 0.0f,
			Keyset keyset = Keyset(key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z));
	static Projectile* createProjectile(int type, float x, float y, float angle);
};
