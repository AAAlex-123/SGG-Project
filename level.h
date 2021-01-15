#pragma once
#include "globals.h"
#include "Factory.h"
#include <unordered_set>
#include <queue>

class Wave;
class Spawnpoint;
class Entity;
class Powerup;

/*
 * Composition of Levels and Waves:
 *
 *  -  Spawnpoints spawn a number of enemies all with the same properties.
 *     For example, a Spawnpoint might spawn 5 enemies of type t at [x, y] with a dt delay between each spawn
 *  -  Waves are a set of different Spawnpoints that all spawn at the same time.
 *	   For example, a Wave might have 3 spawnpoints at the top of the screen that spawn enemies moving downwards
 *  -  Levels are a set of Waves that start at a specific time to spawn enemies, and stop when they are empty
 *     Levels also spawn Powerups, again at specific times since level start.
 *     For example, a Level might have 3 Waves that start to spawn at 5, 10 and 15 seconds,
 *     and also a Powerup that spawns at 25 seconds since level start
 */

 /**
  * A class that defines an in-game level.
  * Specifically, defines the timing of the waves of enemies that spawn and the timing of the level's powerups.
  */
class Level
{
public:
	Level(int, const std::string&);
	// levels are cloned from the GameData map of levels so they can be replayed without reloading them
	Level* clone() const;

	// updates the level's timer as well as the timer of all the waves that are active
	// also adds enemies to the queue to be spawned using the waves' spawn method
	void update(float ms);
	bool can_spawn() const;
	// each spawn() call can only return a single enemy, therefore a queue is used internaly
	// to enable multiple enemies to be spawned at the same update() call
	Entity* spawn() const;
	// powerup spawning
	bool can_spawn_p() const;
	Powerup* spawn_p();

	void add_wave(float, Wave*);
	void add_powerup(float, Powerup*);

	int id() const { return _id; }
	std::string info() const { return "level " + std::to_string(id()) + ": " + _desc; }

	// false if no waves and no enemies left;
	operator bool() const;

	~Level();

private:
	const int _id;
	const std::string _desc;
	float _total_time;

	// sets of waves and powerups and the time from level start to spawn
	std::unordered_set<std::pair<float, Wave*>*>* const waves;
	std::unordered_set<std::pair<float, Powerup*>*>* const powerups;

	// queues of enemies and powerups to be spawned
	std::queue<Entity*>* enemy_queue;
	std::queue<Powerup*>* powerup_queue;
};

/**
* A class that defines a wave of enemies.
* Specifically, defines a set of spawnpoints that spawn enemies.
*/
class Wave
{
public:
	Wave(const std::string&);
	Wave* const clone() const;

	void update(float);
	bool can_spawn() const;
	Entity* const spawn() const;

	void add_spawnpoint(Spawnpoint* const);

	// false if there are no enemies left to spawn
	operator bool() const;

	~Wave();

private:
	const std::string _desc;

	std::unordered_set<Spawnpoint*>* const spawnpoints;
	std::queue<Entity*>* const enemy_queue;
};

/**
* A class that defines a spawnpoint that spawns enemies.
* Specifically, a set of parameters that define the properties of enemies spawning at a location
*/
class Spawnpoint
{
public:
	Spawnpoint(Factory::ENEMY type, float perc_x, float perc_y, float angle, int amount, float spawn_delta, float initial_delay);
	Spawnpoint* clone();

	void update(float);
	bool can_spawn() const;
	Entity* spawn();

	// false if there are no enemies left to spawn
	operator bool() const;

	~Spawnpoint() = default;

private:
	// enemy parameters
	const Factory::ENEMY type;
	const float perc_x, perc_y, angle;

	// spawnpoint parameters
	const float _spawn_delta;
	int _amount;
	float _initial_delay, _elapsed_time;
};
