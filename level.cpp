#include "level.h"
#include "GObjFactory.h"

// ===== LEVEL =====

Level::Level(int id, const std::string& desc)
	: _id(id), _desc(desc), waves(new std::queue<std::pair<float, Wave*>*>), _total_time(0.0f)
{}

void Level::update(float ms)
{
	if (waves->empty())
		return;

	_total_time += (ms / 1000.0f);
	waves->front()->second->update(ms);

	if (_total_time > waves->front()->first)
	{
		delete waves->front();
		waves->pop();
	}
}

bool Level::can_spawn()
{
	return (!waves->empty() && waves->front()->second->can_spawn());
}

Entity* Level::spawn()
{
	return waves->front()->second->spawn();
}

void Level::add_wave(float time, Wave* w)
{
	waves->push(new std::pair<float, Wave*>(time, w));
}

std::string Level::to_file_string()
{
	return "";
	// ...
}

Level::~Level()
{
	while (!waves->empty())
	{
		delete waves->front()->second;
		delete waves->front();
		waves->pop();
	}
	delete waves;
}


// ===== WAVE =====

Wave::Wave()
	: spawnpoints(new std::unordered_set<Spawnpoint*>), enemy_queue(new std::queue<Entity*>)
{}

Wave::Wave(std::unordered_set<Spawnpoint*>* spawnpoints)
	: spawnpoints(spawnpoints), enemy_queue(new std::queue<Entity*>)
{}

void Wave::update(float ms)
{
	for (Spawnpoint* sp : *spawnpoints)
	{
		sp->update(ms);
		if (sp->can_spawn())
			enemy_queue->push(sp->spawn());
	}
}

bool Wave::can_spawn()
{
	return !(enemy_queue->empty());
}

Entity* Wave::spawn()
{
	Entity* return_val = enemy_queue->front();
	enemy_queue->pop();
	return return_val;
}

void Wave::add_spawnpoint(Spawnpoint* s)
{
	spawnpoints->insert(s);
}

std::string Wave::to_file_string()
{
	return "";
	// ...
}

Wave::~Wave()
{
	for (Spawnpoint* sp : *spawnpoints)
		delete sp;
	delete spawnpoints;

	while (!enemy_queue->empty())
	{
		delete enemy_queue->front();
		enemy_queue->pop();
	}
	delete enemy_queue;
}


// ===== SPAWNPOINT =====

Spawnpoint::Spawnpoint(int type, float x, float y, float angle, float duration, float spawn_delta, float initial_delay)
	: type(type), x(x), y(y), angle(angle), _duration(duration), _spawn_delta(spawn_delta), _initial_delay(initial_delay), _total_time(0.0f), _elapsed_time(spawn_delta - 0.1f)
{}

void Spawnpoint::update(float ms)
{
	_total_time += (ms / 1000.0f);

	if ((_total_time > _initial_delay) && (_total_time < _duration + _initial_delay))
		_elapsed_time += (ms / 1000.0f);
}

bool Spawnpoint::can_spawn()
{
	_elapsed_time = ((0.0f * (_elapsed_time >= _spawn_delta)) + (_elapsed_time * (_elapsed_time < _spawn_delta)));
	return _elapsed_time == 0.0f;
}

Entity* Spawnpoint::spawn()
{
	return GObjFactory::createEntity(type, x, y, angle);
}

std::string Spawnpoint::to_file_string()
{
	return "";
	// ...
}
