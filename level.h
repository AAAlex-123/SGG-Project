#pragma once
#include "entity.h"
#include <unordered_set>
#include <queue>

class Wave;
class Spawnpoint;

class Level
{
private:
	const int _id;
	const std::string _desc;

	std::queue<std::pair<float, Wave*>*>* waves;

	float _total_time;

public:
	Level(int, const std::string&);

	void update(float ms);
	bool can_spawn();
	Entity* spawn();
	void add_wave(float, Wave*);

	int id() { return _id; }
	std::string info() { return "level " + std::to_string(id()) + ": " + _desc; }
	
	std::string to_file_string();

	~Level();
};

class Wave
{
private:
	std::unordered_set<Spawnpoint*>* spawnpoints;
	std::queue<Entity*>* enemy_queue;

public:
	Wave();
	Wave(std::unordered_set<Spawnpoint*>* spawnpoints);

	void update(float);

	bool can_spawn();
	Entity* spawn();
	void add_spawnpoint(Spawnpoint*);

	std::string to_file_string();

	~Wave();
};

class Spawnpoint
{
private:
	const int type;
	const float x, y, angle;

	const float _duration, _spawn_delta, _initial_delay;
	float _total_time, _elapsed_time;

public:
	Spawnpoint(int type, float x, float y, float angle, float duration, float spawn_delta, float spawn_delay);

	void update(float);
	bool can_spawn();
	Entity* spawn();

	std::string to_file_string();
};
