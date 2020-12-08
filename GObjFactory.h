#pragma once
#include "entity.h"
#include "projectile.h"
#include "globals.h"

class GObjFactory {
private:
	const static std::string asset_path;
	const static int std_size = 30;
	const static int std_speed = 120;
  
	static Entity& createEnemy1(float x, float y, float angle);
	static Entity& createEnemy2(float x, float y, float angle);
	static Entity& createEnemy3(float x, float y, float angle);
	static Entity& createPlayer(float x, float y, float angle, float dangle, Keyset keyset);

	static Projectile& createStdB(float x, float y, float angle);
	static Projectile& createLB(float x, float y, float angle);
	static Projectile& createIncB(float x, float y, float angle);

public:
	const static int STANDARD_BULLET = 10, LIGHT_BULLET = 11, INCEND_BULLET = 12;
	const static int PLAYER = 0, ENEMY_1 = 1, ENEMY_2 = 2, ENEMY_3 = 3;

	static Entity& createEntity(int type, float x, float y, float angle, float dangle = 0.0f,
			Keyset keyset = Keyset(key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z, key::SCANCODE_Z));
	static Projectile& createProjectile(int type, float x, float y, float angle);
};
