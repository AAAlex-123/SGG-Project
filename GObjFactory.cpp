#include "GObjFactory.h"
#include "Path.h"
#include "globals.h"
#include <iostream>

using namespace std;

Entity* GObjFactory::createPlayer(float x, float y, float angle, float dangle, Keyset keyset) {
	return new Entity(x, y, angle, std_speed / 2, (float)zep_width, (float)zep_height, new string(asset_path + "player1.png"), new KeyboardPath(dangle, keyset), 5, 1, 0, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createEnemy1(float x,float y,float angle) {
	return new Entity(x, y, angle, std_speed, plane_size, plane_size, new string(asset_path + "plane1.png"), new FiringPath(0.5f, new AcceleratingPath(150.0f, new Path())), 5, 1, 100,GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createEnemy2(float x, float y, float angle) {
	return new Entity(x, y, angle, std_speed, plane_size, plane_size, new string(asset_path + "plane2.png"), new FiringPath(1.0f, new RotatingPath(1.0f / 4, new Path())), 5, 1, 150, GObjFactory::INCEND_BULLET);
}

Entity* GObjFactory::createEnemy3(float x, float y, float angle) {
	return new Entity(x, y, angle, std_speed / 2, zep_width, zep_height, new string(asset_path + "plane3.png"), new FiringPath(1.0f, new Path()), 5, 1, 400, GObjFactory::LIGHT_BULLET);
}

Projectile* GObjFactory::createStdB(float x, float y, float angle) {
	return new Projectile(x, y, angle, std_speed * 2, (float)proj_size, new string(asset_path + "bullet1.png"), new AcceleratingPath(50.0f, new Path()), 2);
}

Projectile* GObjFactory::createLB(float x, float y, float angle) {
	return new Projectile(x, y, angle, std_speed * 3, (float)proj_size, new string(asset_path + "bullet2.png"), new AcceleratingPath(50.0f, new Path()), 1);
}

Projectile* GObjFactory::createIncB(float x, float y, float angle) {
	return new Projectile(x, y, angle, std_speed * 2, (float)proj_size, new string(asset_path + "bullet3.png"), new AcceleratingPath(50.0f, new Path()), 4);
}

Entity* GObjFactory::createEntity(int type, float x, float y, float angle, float dangle, Keyset keyset) {
	switch (type) {
	case GObjFactory::PLAYER: return createPlayer(x, y, angle, dangle, keyset);
	case GObjFactory::ENEMY_1: return createEnemy1(x, y, angle);
	case GObjFactory::ENEMY_2: return createEnemy2(x, y, angle);
	case GObjFactory::ENEMY_3: return createEnemy3(x, y, angle);
	default: 
		std::cerr << "Invalid constant, creating enemy1 as placeholder" << std::endl;
		return createEnemy1(x, y, angle);
	}
}

Projectile* GObjFactory::createProjectile(int type, float x, float y, float angle) {
	switch (type) {
	case GObjFactory::STANDARD_BULLET: return createStdB(x, y, angle);
	case GObjFactory::LIGHT_BULLET: return createLB(x, y, angle);
	case GObjFactory::INCEND_BULLET: return createIncB(x, y, angle);
	default: 
		std::cerr << "Invalid constant, creating standard bullet as placeholder" << std::endl;
		return createStdB(x, y, angle);
	}
}
