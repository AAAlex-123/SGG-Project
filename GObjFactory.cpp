#include "GObjFactory.h"
#include "Path.h"
using namespace std;

Entity& GObjFactory::createPlayer(float x, float y, float angle) {
	return *(new Entity(x, y, angle, std_size * 2, new string("pl_sprt"), std_speed / 2, 5, 1, new PlayerPath(), GObjFactory::STANDARD_BULLET));
}

Entity& GObjFactory::createEnemy1(float x,float y,float angle) {
	return *(new Entity(x,y,angle,std_size,new string("sprt1"),std_speed,5,1,new StraightPath(),GObjFactory::STANDARD_BULLET));
}

Entity& GObjFactory::createEnemy2(float x, float y, float angle) {
	return *(new Entity(x, y, angle, std_size, new string("sprt2"), std_speed, 5, 1, new SwigglyPath(), GObjFactory::LIGHT_BULLET));
}

Entity& GObjFactory::createEnemy3(float x, float y, float angle) {
	return *(new Entity(x, y, angle, std_size*3, new string("sprt1"), std_speed/2, 5, 1, new StraightPath(), GObjFactory::INCEND_BULLET));
}

Projectile& GObjFactory::createStdB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle,(float) std_size / 2, string("bullet1"), std_speed * 2, 2, new StraightPath()));
}

Projectile& GObjFactory::createLB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle, std_size/2, string("bullet2"), std_speed*3, 1, new StraightPath()));
}

Projectile& GObjFactory::createIncB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle, std_size / 2, string("bullet3"), std_speed * 2, 4, new StraightPath()));
}

Entity& GObjFactory::createEntity(int type, float x, float y, float angle) {
	switch (type) {
	case GObjFactory::PLAYER: return createPlayer(x, y, angle);
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