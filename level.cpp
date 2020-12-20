#include "level.h"
#include "entity.h"
#include "GObjFactory.h"
#include <iostream>

// ===== LEVEL =====

Level::Level(int id, const std::string& desc)
	: _id(id), _desc(desc), waves(new std::unordered_set<std::pair<float, Wave*>*>),
	enemy_queue(new std::queue<Entity*>), _total_time(0.0f)
{
	if (id == -1)
	{
		std::cerr << "Error: invalid level id: " << id << std::endl;
	}
	else if (id < 0 || id > 9)
	{
		std::cerr << "Warning: level id '" << id << "' out of bounds; players won't be able to select this level" << std::endl;
	}
}

void Level::update(float ms)
{
	// indicates end of level, no update should happen
	if (waves->empty())
		return;

	_total_time += (ms / 1000.0f);

	for (auto iter = waves->begin(); iter != waves->end(); ++iter)
	{
		if (_total_time >= (*iter)->first)
		{
			(*iter)->second->update(ms);
			if ((*iter)->second->can_spawn())
			{
				enemy_queue->push((*iter)->second->spawn());
			}
			// if there are no more enemies left in the wave delete wave
			if (!*((*iter)->second))
			{
				delete (*iter)->second;
				delete (*iter);
				iter = waves->erase(iter);
				if (iter == waves->end())
					break;
			}
		}
	}
}

bool Level::can_spawn()
{
	return (!enemy_queue->empty());
}

Entity* Level::spawn()
{
	// return the first enemy of the queue
	Entity* return_val = enemy_queue->front();
	enemy_queue->pop();
	return return_val;
}

void Level::add_wave(float time, Wave* w)
{
	waves->insert(new std::pair<float, Wave*>(time, w));
}

std::string Level::to_file_string()
{
	return "";
	// ...
}

Level::~Level()
{
	// carefully destroy each wave, pair of time-wave and the set
	for (std::pair<float, Wave*>* p : *waves)
	{
		delete p->second;
		delete p;
	}
	waves->clear();
	delete waves;
}


// ===== WAVE =====

Wave::Wave(const std::string& desc)
	: _desc(desc), spawnpoints(new std::unordered_set<Spawnpoint*>), enemy_queue(new std::queue<Entity*>)
{}

Wave::Wave(const Wave& w)
	: _desc(w._desc), spawnpoints(new std::unordered_set<Spawnpoint*>), enemy_queue(new std::queue<Entity*>)
{
	for (Spawnpoint* sp : *(w.spawnpoints))
	{
		spawnpoints->insert(new Spawnpoint(*sp));
	}
}

void Wave::update(float ms)
{
	// update spawnpoints' timer and add new enemies to the queue
	// a queue is used because if multiple spawnpoints spawn at the same time,
	// spawn() would have to return multiple pointers, which can't be done with this design
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
	// return the first enemy of the queue
	Entity* return_val = enemy_queue->front();
	enemy_queue->pop();
	return return_val;
}

void Wave::add_spawnpoint(Spawnpoint* s)
{
	spawnpoints->insert(s);
}

Wave::operator bool() const
{
	bool res = false;
	for (Spawnpoint* sp : *spawnpoints)
	{
		res |= *sp;
	}
	return res || !enemy_queue->empty();
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

Spawnpoint::Spawnpoint(int type, float perc_x, float perc_y, float angle, int amount, float spawn_delta, float initial_delay)
	: type(type), perc_x(perc_x), perc_y(perc_y), angle(angle), _spawn_delta(spawn_delta), _amount(amount), _initial_delay(initial_delay), _elapsed_time(spawn_delta - 0.1f)
{}

//Spawnpoint::Spawnpoint(const Spawnpoint& s)
//	: type(s.type), perc_x(s.perc_x), perc_y(s.perc_y), angle(s.angle), _spawn_delta(s._spawn_delta), _amount(s._amount), _initial_delay(s._initial_delay), _elapsed_time(s._spawn_delta - 0.1f)
//{}

void Spawnpoint::update(float ms)
{
	_initial_delay -= (ms / 1000.0f);

	if (_initial_delay <= 0)
	{
		_elapsed_time += (ms / 1000.0f);
		// if (_elapsed_time >= _spawn_delta) _elapsed_time = 0.0f;
		_elapsed_time = ((0.0f * (_elapsed_time >= _spawn_delta)) + (_elapsed_time * (_elapsed_time < _spawn_delta)));
	}
}

bool Spawnpoint::can_spawn()
{
	// _elapsed_time == 0.0f only when it is reset from update() -> only when it can spawn
	return (_elapsed_time == 0.0f) && (_amount > 0);
}

Entity* Spawnpoint::spawn()
{
	--_amount;
	return GObjFactory::createEntity(type, get_canvas_width() * perc_x, get_canvas_height() * perc_y, angle);
}

Spawnpoint::operator bool() const
{
	return _amount > 0;
}

std::string Spawnpoint::to_file_string()
{
	return "";
	// ...
}
