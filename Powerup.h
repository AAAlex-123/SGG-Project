#pragma once
#include "globals.h"
#include "gameobject.h"

class Player;

//A superclass defining objects that when interact with a player give him any kind of buff
//Subclasses specify the details of said buff
class Powerup :public GameObject {
protected:
	//A simple template method called whenever a powerup is touched
	virtual void hit(GameObject* o2) final override;

public:
	Powerup(float xpos, float ypos, float angle, const std::string* sprite, int score);
	//The method subclasses override to specify hit()'s behavior
	virtual void consume(Player* target) const = 0;
	//Powerup is implemented with the clone pattern because of how levels are loaded
	virtual Powerup* clone() const = 0;

	virtual VisualEffect* getDestructionVisualEffect() const override;

	virtual ~Powerup();
};

class HealthPowerup : public Powerup {
private:
	static const int score = 10000;
	const std::string sprite = std::string(image_path + "h_powerup.png");
public:

	HealthPowerup(float xpos, float ypos, float angle);

	virtual void consume(Player* target) const override;

	virtual Powerup* clone() const;

	virtual ~HealthPowerup() = default;
};

class ProjectilePowerup : public Powerup {
private:
	static const int score = 10000;
	const std::string sprite = std::string(image_path + "proj_powerup.png");
	int proj_type = 0;

public:
	ProjectilePowerup(float xpos, float ypos, float angle);

	virtual void consume(Player* target) const override;

	virtual Powerup* clone() const;

	virtual ~ProjectilePowerup() = default;
};

class PointsPowerup : public Powerup {
private:
	static const int score = 100000;
	const std::string sprite = std::string(image_path + "points_powerup.png");

public:
	PointsPowerup(float xpos, float ypos, float angle);

	virtual void consume(Player* target) const override;

	virtual Powerup* clone() const;

	virtual ~PointsPowerup() = default;
};
