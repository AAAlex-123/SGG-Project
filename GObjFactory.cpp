#include "GObjFactory.h"
#include "Path.h"
#include "entity.h"
#include "projectile.h"
#include "visual_effect.h"
#include <iostream>

using namespace std;

// spaghetti that will not stay here forever
bool GObjFactory::atLeastOne = false;
const Keyset GObjFactory::pl1_kset = Keyset(key::SCANCODE_W, key::SCANCODE_S, key::SCANCODE_A, key::SCANCODE_D, key::SCANCODE_Q, key::SCANCODE_E, key::SCANCODE_X);
const Keyset GObjFactory::pl2_kset = Keyset(key::SCANCODE_UP, key::SCANCODE_DOWN, key::SCANCODE_LEFT, key::SCANCODE_RIGHT, key::SCANCODE_Z, key::SCANCODE_C, key::SCANCODE_SPACE);
//unused: Keyset(key::SCANCODE_T, key::SCANCODE_G, key::SCANCODE_F, key::SCANCODE_H, key::SCANCODE_R, key::SCANCODE_Y, key::SCANCODE_B);

// ===== ENTITY =====

Entity* GObjFactory::createEntity(int type, float x, float y, float angle, float dangle, float cooldown) {
	switch (type) {
	case GObjFactory::PLAYER: return createPlayer(x, y, angle, dangle, cooldown);
	case GObjFactory::SIMPLE_ENEMY: return createSimpleEnemy(x, y, angle);
	case GObjFactory::ROTATING_ENEMY: return createRotatingEnemy(x, y, angle);
	case GObjFactory::ACCELERATING_ENEMY: return createAcceleratingEnemy(x, y, angle);
	case GObjFactory::TANK_ENEMY: return createTankEnemy(x, y, angle);
	default:
		std::cerr << "Invalid constant, creating enemy1 as placeholder" << std::endl;
		return createSimpleEnemy(x, y, angle);
	}
}

Entity* GObjFactory::createPlayer(float x, float y, float angle, float dangle, float cooldown) {
	if (!atLeastOne) {
		atLeastOne = true;
		return new Entity(x, y, angle, player_sp, player_w, player_h, new string(image_path + "player1"), new KeyboardPath(dangle, cooldown, pl1_kset), INT_MAX, player_hp, 0, GObjFactory::STANDARD_BULLET);
	}
	else
		return new Entity(x, y, angle, player_sp, player_w, player_h, new string(image_path + "player2"), new KeyboardPath(dangle, cooldown, pl2_kset), INT_MAX, player_hp, 0, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createSimpleEnemy(float x, float y, float angle) {
	//	changes these numberes:					vvv					 vvv				  vvv													  vvv							   vvv				  vvv			        vvv			and this vvv bullet
	return new Entity(x, y, angle, b_plane_sp * 1.0f, b_plane_size * 1.0f, b_plane_size * 1.0f, new string(image_path + "plane1"), new FiringPath(1.0f, new Path()), b_plane_dmg * 1.0f, b_plane_hp * 1.0f, b_plane_score * 1.0f, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createRotatingEnemy(float x, float y, float angle) {																					     // this too vvv
	return new Entity(x, y, angle, b_plane_sp * 1.0f, b_plane_size * 1.0f, b_plane_size * 1.0f, new string(image_path + "plane2"), new FiringPath(0.3f, new RotatingPath(1.0f / 4.0f, new Path())), b_plane_dmg * 1.5f, b_plane_hp * 1.5f, b_plane_score * 0.5f, GObjFactory::LIGHT_BULLET);
}

Entity* GObjFactory::createAcceleratingEnemy(float x, float y, float angle) {																					 // and  too vvv
	return new Entity(x, y, angle, b_plane_sp * 0.3f, b_plane_size * 0.8f, b_plane_size * 0.8f, new string(image_path + "plane3"), new FiringPath(2.0f, new AcceleratingPath(200.0f, new Path())), b_plane_dmg * 0.8f, b_plane_hp * 0.4f, b_plane_score * 3.0f, GObjFactory::STANDARD_BULLET);
}

Entity* GObjFactory::createTankEnemy(float x, float y, float angle) {
	return new Entity(x, y, angle, b_plane_sp * 0.5f, b_plane_size * 1.5f, b_plane_size * 1.5f, new string(image_path + "plane4"), new FiringPath(3.5f, new Path()), b_plane_dmg * 3.0f, b_plane_hp * 3.0f, b_plane_score * 2.0f, GObjFactory::HEAVY_BULLET);
}

// ===== PROJECTILE =====

Projectile* GObjFactory::createProjectile(int type, float x, float y, float angle) {
	switch (type) {
	case GObjFactory::STANDARD_BULLET: return createStandardBullet(x, y, angle);
	case GObjFactory::HEAVY_BULLET: return createHeavyBullet(x, y, angle);
	case GObjFactory::LIGHT_BULLET: return createLightBullet(x, y, angle);
	default:
		std::cerr << "Invalid constant, creating standard bullet as placeholder" << std::endl;
		return createStandardBullet(x, y, angle);
	}
}

Projectile* GObjFactory::createStandardBullet(float x, float y, float angle) {
	// change these numbers:					   vvv				   vvv																	  vvv
	return new Projectile(x, y, angle, b_proj_sp * 1.0f, b_proj_size * 1.0f, new string(image_path + "bullet1.png"), new Path(), b_proj_dmg * 1.0f);
}

Projectile* GObjFactory::createHeavyBullet(float x, float y, float angle) {
	return new Projectile(x, y, angle, b_proj_sp * 0.8f, b_proj_size * 3.0f, new string(image_path + "bullet1.png"), new Path(), b_proj_dmg * 3.0f);
}

Projectile* GObjFactory::createLightBullet(float x, float y, float angle) {
	return new Projectile(x, y, angle, b_proj_sp * 1.5f, b_proj_size * 0.8f, new string(image_path + "bullet1.png"), new Path(), b_proj_dmg * 0.7f);
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
