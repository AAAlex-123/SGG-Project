#pragma once
#include "entity.h"
#include <queue>

class Level
{
private:
	int _id;
	std::string _desc;

	std::queue<std::pair<float, Entity*>> enemies;

	float _total_time;

public:
	// default constructor needed for some reason
	Level(int = -1, std::string = "null descr");

	void operator=(const Level& rhs);

	inline void update(float ms) { _total_time += (ms / 1000.0f); }
	inline bool can_spawn()
	{
		if (enemies.empty())
			return false;
		return _total_time > enemies.front().first;
	}

	Entity& spawn();

	inline void add_enemy(float time, Entity* enemy) { enemies.push(std::pair<float, Entity*>(time, enemy)); }

	inline int id() { return _id; }
	inline std::string info() { return "level " + std::to_string(id()) + ": " + _desc; }
	
	// returns the string necessary to rebuild this level
	std::string to_file_string();
};
