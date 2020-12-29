#pragma once
#include "globals.h"
#include "Player.h"
#include "GObjFactory.h"

//All the methods here are supposed to be in-line so they are declared only in the header file

class Powerup :public GameObject{
protected:
	virtual void hit(GameObject* o2) final override {
		GameObject::hit(o2);
		consume(dynamic_cast<Player*> (o2)); //it's guaranteed to be a Player* and the method will be used only a couple times in the game
	}

public:
	Powerup(float xpos, float ypos, float angle, const std::string* sprite, int score):
		GameObject(xpos,ypos,angle, 50.f,20,20, sprite, new Path(),0,1,score){}

	virtual void consume(Player* target) const = 0;

	virtual ~Powerup() {
		graphics::playSound(sound_path + "powerup.mp3", 0.2f, false);
	}

	virtual VisualEffect* getDestructionVisualEffect() const override {
		return GObjFactory::createVisualEffect(GObjFactory::NOEFFECT, x, y, 0.0f, 1.0f, 18.0f); //obviously change this later
	}
};

class HealthPowerup : public Powerup {
private:
	static const int score = 1000;
	const std::string sprite = std::string(image_path + "h_powerup.png");
public:

	HealthPowerup(float xpos, float ypos, float angle):
		Powerup(xpos, ypos, angle, &sprite, score){}

	virtual void consume(Player* target) const override {
		target->addHealth(3);
	}

	virtual ~HealthPowerup() = default;
};

class ProjectilePowerup : public Powerup {
private:
	static const int score = 1000;
	const std::string sprite = std::string(image_path + "proj_powerup.png");
	int proj_type = 0;

public:
	ProjectilePowerup(float xpos, float ypos, float angle) :
		Powerup(xpos, ypos, angle, &sprite, score) {

		if ((double)rand() / (RAND_MAX) + 1 > 0.5f)
			proj_type = GObjFactory::HEAVY_BULLET;
		else
			proj_type = GObjFactory::LIGHT_BULLET;
	}

	virtual void consume(Player* target) const override {
		target->setProjectile(proj_type);
	}

	virtual ~ProjectilePowerup() = default;
};

class PointsPowerup : public Powerup {
private:
	static const int score = 4000;
	const std::string sprite = std::string(image_path + "points_powerup.png");

public:
	PointsPowerup(float xpos, float ypos, float angle) :
		Powerup(xpos, ypos, angle, &sprite, score) {}

	virtual void consume(Player* target) const override {
		;
	}

	virtual ~PointsPowerup() = default;
};
