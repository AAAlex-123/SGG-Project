#pragma once
#include "entity.h"
#include <queue>

class Level
{
private:
	class Wave;
	int _id;
	std::string _desc;

	std::queue<std::pair<float, Wave*>> waves;

	float _total_time;

	class Wave
	{
	private:
		int _id;
		std::string _desc;

		std::queue<std::pair<float, Entity*>> enemies;

		float _total_time;

	public:
		void update(float ms) { _total_time += (ms / 1000.0f); }
		bool can_spawn()
		{
			if (enemies.empty())
				return false;
			return _total_time > enemies.front().first;
		}

		Entity& spawn()
		{
			Entity* return_value = enemies.front().second;
			enemies.pop();
			return *(return_value);
		}

		void add_enemy(float time, Entity* enemy) { enemies.push(std::pair<float, Entity*>(time, enemy)); }
	};

public:
	// default constructor needed for some reason
	Level(int = -1, std::string = "null descr");

	void operator=(const Level& rhs);

	void update(float ms) { _total_time += (ms / 1000.0f); waves.front().second->update(ms); }
	bool can_spawn()
	{
		if (waves.empty())
			return false;
		return waves.front().second->can_spawn();
	}

	Entity& spawn()
	{
		Entity* return_value = &waves.front().second->spawn();
		waves.pop();
		return *(return_value);
	}

	void add_enemy(float time, Entity* enemy) { waves.back().second->add_enemy(time, enemy); }

	int id() { return _id; }
	std::string info() { return "level " + std::to_string(id()) + ": " + _desc; }
	
	// returns the string necessary to rebuild this level
	std::string to_file_string();
};
