#include "GObjFactory.h"
#include "Path.h"
using namespace std;

string asset_path = "assets\\";

Entity& GObjFactory::createPlayer(float x, float y, float angle,float dangle, Keyset keyset) {
  return *(new Entity(x, y, angle,(float)std_speed*5, (float)std_size, new string(asset_path+"player1.png"), new KeyboardPath(dangle, keyset), 5, 1, GObjFactory::STANDARD_BULLET));
}

Entity& GObjFactory::createEnemy1(float x,float y,float angle) {
	return *(new Entity(x,y,angle,std_size,new string(asset_path + "plane1.png"),std_speed,5,1,new AcceleratingPath(50.0f, new RotatingPath(1.0f/4, new Path())), 5, 1, GObjFactory::STANDARD_BULLET));,GObjFactory::STANDARD_BULLET));
}

Entity& GObjFactory::createEnemy2(float x, float y, float angle) {
	return *(new Entity(x, y, angle, std_size, new string(asset_path + "plane2.png"), std_speed, 5, 1,  new AcceleratingPath(50.0f, new Path()), 5, 1, GObjFactory::LIGHT_BULLET));, GObjFactory::LIGHT_BULLET));
}

Entity& GObjFactory::createEnemy3(float x, float y, float angle) {
	return *(new Entity(x, y, angle, std_size*3, new string(asset_path + "plane3.png"), std_speed/2, 5, 1, new RotatingPath(1.0f / 4, new Path()), 5, 1, GObjFactory::INCEND_BULLET));, GObjFactory::INCEND_BULLET));
}

Projectile& GObjFactory::createStdB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle,(float) std_size / 2, string(asset_path + "bullet1.png"), std_speed * 2, 2, new StraightPath()));
}

Projectile& GObjFactory::createLB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle, std_size/2, string(asset_path + "bullet2.png"), std_speed*3, 1, new StraightPath()));
}

Projectile& GObjFactory::createIncB(float x, float y, float angle) {
	return *(new Projectile(x, y, angle, std_size / 2, string(asset_path + "bullet3.png"), std_speed * 2, 4, new StraightPath()));
}

Entity& GObjFactory::createEntity(int type, float x, float y, float angle, float dangle, Keyset keyset)
{
	switch (type)
	{
	case GObjFactory::PLAYER: return createPlayer(x, y, angle, dangle, keyset);
	case GObjFactory::ENEMY_1: return createEnemy1(x, y, angle);
	case GObjFactory::ENEMY_2: return createEnemy2(x, y, angle);
	case GObjFactory::ENEMY_3: return createEnemy3(x, y, angle);
	//default throw exception
	}
}

Projectile& GObjFactory::createProjectile(int type, float x, float y, float angle)
{
	switch (type)
	{
	case GObjFactory::STANDARD_BULLET: return createStdB(x, y, angle);
	case GObjFactory::LIGHT_BULLET: return createLB(x, y, angle);
	case GObjFactory::INCEND_BULLET: return createIncB(x, y, angle);
	//default throw exception
	}
}
