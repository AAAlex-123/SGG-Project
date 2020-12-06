#pragma once
#include "entity.h"
#include "projectile.h"

class GObjFactory{
private:
	const static int std_size = 50;
	const static int std_speed = 35;
	static Entity& createEnemy1(float x, float y, float angle);
	static Entity& createEnemy2(float x, float y, float angle);
	static Entity& createEnemy3(float x, float y, float angle);
	static Entity& createPlayer(float x, float y, float angle);

	static Projectile& createStdB(float x, float y, float angle);
	static Projectile& createLB(float x, float y, float angle);
	static Projectile& createIncB(float x, float y, float angle);
	
public:
	const static int STANDARD_BULLET = 10, LIGHT_BULLET = 11, INCEND_BULLET = 12; 
	const static int PLAYER = 0, ENEMY_1 = 1, ENEMY_2 = 2, ENEMY_3 =3;
	
	static Entity& createEntity(int type, float x,float y, float angle);
	static Projectile& createProjectile(int type, float x, float y,float angle);

};

