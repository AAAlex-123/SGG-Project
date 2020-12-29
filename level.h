#pragma once
#include "globals.h"
#include <unordered_set>
#include <queue>

class Wave;
class Spawnpoint;
class Entity;
class Powerup;

// defines a set of waves and their timings
class Level
{
private:
	const int _id;
	const std::string _desc;

	// queue of waves and their start time
	// the time of each wave corresponds to the amount of time that will pass
	// from level start before the wave starts
	std::unordered_set<std::pair<float, Wave*>*>* waves;
	std::unordered_set<std::pair<float, Powerup*>*>* powerups;
	// queue of enemies to be spawned (look at Wave::update() for why it is a queue)
	std::queue<Entity*>* enemy_queue;
	std::queue<Powerup*>* powerup_queue;

	float _total_time;

public:
	Level(int, const std::string&);

	// update level and wave time
	void update(float ms);
	// delegate entity spawning to the waves
	bool can_spawn();
	Entity* spawn();
	// handle powerup spawning
	bool can_spawn_p();
	Powerup* spawn_p();

	void add_wave(float, Wave*);
	void add_powerup(float, Powerup*);

	// used by level selection to display info and select level
	int id() { return _id; }
	std::string info() { return "level " + std::to_string(id()) + ": " + _desc; }

	// false if no enemies or waves left;
	// false if it shouldn't be here
	operator bool() const;

	// (wip) returns the string necessary to rebuild the level
	std::string to_file_string();

	~Level();
};

class Wave
{
private:
	const std::string _desc;

	std::unordered_set<Spawnpoint*>* spawnpoints;
	// queue of enemies to be spawned (look at Wave::update() for why it is a queue)
	std::queue<Entity*>* enemy_queue;

public:
	Wave(const std::string&);
	Wave(const Wave&);

	// update the spawnpoints' timer, add new enemies to enemy_queue
	void update(float);
	// work with the enemy_queue
	bool can_spawn();
	Entity* spawn();

	void add_spawnpoint(Spawnpoint*);

	// false if no spawnpoint has any enemies left
	// false if it shouldn't be here
	operator bool() const;

	// (wip) returns the string necessary to rebuild the wave
	std::string to_file_string();

	~Wave();
};

class Spawnpoint
{
private:
	// enemy parameters
	const int type;
	const float perc_x, perc_y, angle;

	// spawnpoint parameters
	const float _spawn_delta;
	int _amount;
	float _initial_delay, _elapsed_time;

public:
	Spawnpoint(int type, float perc_x, float perc_y, float angle, int amount, float spawn_delta, float initial_delay);
	// the automatically generated one should be just fine
	//Spawnpoint(const Spawnpoint&);

	void update(float);
	bool can_spawn();
	Entity* spawn();

	// false if there are no enemies left
	operator bool() const;

	// (wip) returns the string necessary to rebuild the spawnpoint
	std::string to_file_string();
};
