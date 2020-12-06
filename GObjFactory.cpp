#include "GObjFactory.h"
#include "Path.h"
using namespace std;


Entity& GObjFactory::createPlayer(float x, float y, float angle, float dangle, key u, key d, key l, key r, key rl, key rr) {
	return *(new Entity(x, y, angle, (float)std_speed*5, (float)std_size, new string("assets\\pl_sprt.png"), new KeyboardPath(dangle, u, d, l, r, rl, rr), 5, 1, GObjFactory::STANDARD_BULLET));
}

Entity& GObjFactory::createEnemy1(float x,float y,float angle) {
	return *(new Entity(x, y, angle, (float)std_speed * 3, (float)std_size, new string("sprt1"), new AcceleratingPath(new RotatingPath(new Path(), 1.0f/4), 50.0f), 5, 1, GObjFactory::STANDARD_BULLET));
}

Entity& GObjFactory::createEnemy2(float x, float y, float angle) {
	return *(new Entity(x, y, angle, (float)std_speed, (float)std_size, new string("sprt2"), new AcceleratingPath(new Path(), 50.0f), 5, 1, GObjFactory::LIGHT_BULLET));
}

Entity& GObjFactory::createEnemy3(float x, float y, float angle) {
	return *(new Entity(x, y, angle, (float)std_speed / 2, (float)std_size, new string("sprt1"), new RotatingPath(new Path(), 1.0f / 4), 5, 1, GObjFactory::INCEND_BULLET));
}

Projectile& GObjFactory::createStdB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle, (float)std_speed, (float)std_size / 2, new string("bullet1"), new Path, 2));
}

Projectile& GObjFactory::createLB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle, (float)std_speed * 3, (float)std_size / 2, new string("bullet2"), new Path, 1));
}

Projectile& GObjFactory::createIncB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle, (float)std_speed * 2, (float)std_size / 2, new string("bullet3"), new Path, 4));
}

Entity& GObjFactory::createEntity(int type, float x, float y, float angle, float dangle, key u, key d, key l, key r, key rl, key rr) {
	switch (type) {
	case GObjFactory::PLAYER: return createPlayer(x, y, angle, dangle, u, d, l, r, rl, rr);
	case GObjFactory::ENEMY_1: return createEnemy1(x, y, angle);
	case GObjFactory::ENEMY_2: return createEnemy2(x, y, angle);
	case GObjFactory::ENEMY_3: return createEnemy3(x, y, angle);
	//default throw exception
	}
}

Projectile& GObjFactory::createProjectile(int type, float x, float y, float angle) {
	switch (type) {
	case GObjFactory::STANDARD_BULLET: return createStdB(x, y, angle);
	case GObjFactory::LIGHT_BULLET: return createLB(x, y, angle);
	case GObjFactory::INCEND_BULLET:return createIncB(x, y, angle);
	//default throw exception
	}
}
