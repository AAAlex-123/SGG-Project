#pragma once
#include "gameobject.h"

class Player;

/**
 * A superclass defining objects that when interact with a player give him any kind of buff
 * Subclasses specify the details of said buff
 */ 
class Powerup :public GameObject
{
public:
	Powerup(float xpos, float ypos, float angle, const std::string* sprite, int score);

	//The method subclasses override to specify hit()'s behavior
	virtual void consume(Player* target) const = 0;
	//Powerup is implemented with the prototype pattern because of how they get spawned from Level
	virtual Powerup* clone() const = 0;

	virtual VisualEffect* getDestructionVisualEffect() const override;

	virtual ~Powerup();

protected:
	// A simple template method called whenever a powerup is touched
	virtual void hit(GameObject* o2) final override;
};

class HealthPowerup : public Powerup
{
public:

	HealthPowerup(float xpos, float ypos, float angle);

	virtual void consume(Player* target) const override;

	virtual Powerup* clone() const;

	virtual ~HealthPowerup() = default;

private:
	static const int score = 10000;
};

class ProjectilePowerup : public Powerup
{
public:
	ProjectilePowerup(float xpos, float ypos, float angle);

	virtual void consume(Player* target) const override;

	virtual Powerup* clone() const;

	virtual ~ProjectilePowerup() = default;

private:
	static const int score = 10000;
};

class PointsPowerup : public Powerup
{
public:
	PointsPowerup(float xpos, float ypos, float angle);

	virtual void consume(Player* target) const override;

	virtual Powerup* clone() const;

	virtual ~PointsPowerup() = default;

private:
	static const int score = 100000;
};
