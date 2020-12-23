#include "GObjFactory.h"
#include "Path.h"
#include "entity.h"
#include "projectile.h"
#include "visual_effect.h"
#include <iostream>

using namespace std;

bool GObjFactory::atLeastOne = false;
const Keyset GObjFactory::pl1_kset = Keyset(key::SCANCODE_W, key::SCANCODE_S, key::SCANCODE_A, key::SCANCODE_D, key::SCANCODE_Q, key::SCANCODE_E, key::SCANCODE_X);
const Keyset GObjFactory::pl2_kset = Keyset(key::SCANCODE_UP, key::SCANCODE_DOWN, key::SCANCODE_LEFT, key::SCANCODE_RIGHT, key::SCANCODE_Z, key::SCANCODE_C, key::SCANCODE_SPACE);
//unused: Keyset(key::SCANCODE_T, key::SCANCODE_G, key::SCANCODE_F, key::SCANCODE_H, key::SCANCODE_R, key::SCANCODE_Y, key::SCANCODE_B);

// ===== ENTITY =====

Entity* GObjFactory::createEntity(int type, float x, float y, float angle, float dangle, float cooldown) {
	switch (type) {
	case GObjFactory::PLAYER: return createPlayer(x, y, angle, dangle, cooldown);
	case GObjFactory::ENEMY_1: return createEnemy1(x, y, angle);
	case GObjFactory::ENEMY_2: return createEnemy2(x, y, angle);
	case GObjFactory::ENEMY_3: return createEnemy3(x, y, angle);
	default:
		std::cerr << "Invalid constant, creating enemy1 as placeholder" << std::endl;
		return createEnemy1(x, y, angle);
	}
}


Entity* GObjFactory::createPlayer(float x, float y, float angle, float dangle, float cooldown) {
	if (!atLeastOne) {
		atLeastOne = true;
		return new Entity(x, y, angle, std_speed / 2, (float)zep_width, (float)zep_height, new string(image_path + "player1"), new KeyboardPath(dangle, cooldown ,pl1_kset), 5, 10, 0, GObjFactory::STANDARD_BULLET);
	}else
		return new Entity(x, y, angle, std_speed / 2, (float)zep_width, (float)zep_height, new string(image_path + "player2"), new KeyboardPath(dangle, cooldown, pl2_kset), 5, 10, 0, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createEnemy1(float x,float y,float angle) {
	return new Entity(x, y, angle, std_speed, plane_size, plane_size, new string(image_path + "plane1"), new FiringPath(0.5f, new AcceleratingPath(150.0f, new Path())), 5, 1, 100, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createEnemy2(float x, float y, float angle) {
	return new Entity(x, y, angle, std_speed, plane_size, plane_size, new string(image_path + "plane2"), new FiringPath(1.0f, new RotatingPath(1.0f / 4, new Path())), 5, 1, 150, GObjFactory::INCEND_BULLET);
}

Entity* GObjFactory::createEnemy3(float x, float y, float angle) {
	return new Entity(x, y, angle, std_speed, zep_width, zep_height, new string(image_path + "plane3"), new FiringPath(1.0f, new Path()), 5, 1, 400, GObjFactory::LIGHT_BULLET);
}

// ===== PROJECTILE =====

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

Projectile* GObjFactory::createStdB(float x, float y, float angle) {
	return new Projectile(x, y, angle, std_speed * 2, (float)proj_size, new string(image_path + "bullet1.png"), new AcceleratingPath(50.0f, new Path()), 2);
}

Projectile* GObjFactory::createLB(float x, float y, float angle) {
	return new Projectile(x, y, angle, std_speed * 3, (float)proj_size, new string(image_path + "bullet2.png"), new AcceleratingPath(50.0f, new Path()), 1);
}

Projectile* GObjFactory::createIncB(float x, float y, float angle) {
	return new Projectile(x, y, angle, std_speed * 2, (float)proj_size, new string(image_path + "bullet3.png"), new AcceleratingPath(50.0f, new Path()), 4);
}

// ===== VISUAL EFFECT =====

VisualEffect* GObjFactory::createVisualEffect(int type, float x, float y, float angle, float duration, float fps) {
	switch (type) {
	case GObjFactory::EXPLOSION_1: return createExplosion1(x, y, angle, duration, fps);
	case GObjFactory::SMOKE: return createSmoke(x, y, angle, duration);
	case GObjFactory::NOEFFECT: return createNoEffect();
	default:
		std::cerr << "Invalid constant, creating smoke as placeholder" << std::endl;
		return createSmoke(x, y, angle, duration);
	}
}

VisualEffect* GObjFactory::createExplosion1(float x, float y, float angle, float duration, float fps) {
	return new VisualEffect(x, y, angle, 0, 50,
		new string[7]{ image_path + "expl1.png", image_path + "expl2.png", image_path + "expl3.png",
		image_path + "expl4.png", image_path + "expl5.png", image_path + "expl6.png", image_path + "expl7.png"
		}, 7, duration, fps);
}

VisualEffect* GObjFactory::createSmoke(float x, float y, float angle, float duration) {
	return new VisualEffect(x, y, angle, 0, 10,
		new string[1]{ image_path + "smoke.png" 
		}, 1, duration, 0.01f);
}

VisualEffect* GObjFactory::createNoEffect() {
	return new VisualEffect(0, 0, 0, 0, 0, new string[1]{ "" }, 0, 0.01f, 1.0f);
}
