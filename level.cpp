#include "level.h"
#include "entity.h"
#include "Powerup.h"
#include "GObjFactory.h"
#include <iostream>

// ===== LEVEL =====

Level::Level(int id, const std::string& desc)
	: _id(id), _desc(desc),
	waves(new std::unordered_set<std::pair<float, Wave*>*>), enemy_queue(new std::queue<Entity*>),
	powerups(new std::unordered_set<std::pair<float, Powerup*>*>), powerup_queue(new std::queue<Powerup*>),
	_total_time(0.0f)
{
	if (id == -1)
	{
		std::cerr << "Error: invalid level id: " << id << std::endl;
	}
	else if (id < 0 || id > 9)	// concerning level select
	{
		std::cerr << "Warning: level id '" << id << "' out of bounds; players won't be able to select this level" << std::endl;
	}
}

Level* Level::clone() const
{
	Level* l = new Level(_id, _desc);

	for (auto iter = this->waves->begin(); iter != this->waves->end(); ++iter)
	{
		l->waves->insert(new std::pair<float, Wave*>((*iter)->first, (*iter)->second->clone()));
	}

	for (auto iter = this->powerups->begin(); iter != this->powerups->end(); ++iter)
	{
		l->powerups->insert(new std::pair<float, Powerup*>((*iter)->first, (*iter)->second->clone()));
	}
	return l;
}

void Level::update(float ms)
{
	// indicates end of level, no update should happen
	//if (waves->empty() && powerups->empty())
	//	return;

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
			// if there are no more enemies left in the wave, delete the wave
			if (!*((*iter)->second))
			{
				delete (*iter)->second;
				delete (*iter);
				iter = waves->erase(iter);
				// catch cases when the last item is deleted so `iter` is the end iterator
				// and it's incremented at the end of the loop
				if (iter == waves->end())
					break;
			}
		}
	}

	for (auto iter = powerups->begin(); iter != powerups->end(); ++iter)
	{
		if (_total_time >= (*iter)->first)
		{
			powerup_queue->push((*iter)->second);
			iter = powerups->erase(iter);
			if (iter == powerups->end())
				break;
		}
	}
}

bool Level::can_spawn() const
{
	return (!enemy_queue->empty());
}

Entity* Level::spawn() const
{
	Entity* return_val = enemy_queue->front();
	enemy_queue->pop();
	return return_val;
}

bool Level::can_spawn_p() const
{
	return (!powerup_queue->empty());
}

Powerup* Level::spawn_p()
{
	Powerup* return_val = powerup_queue->front();
	powerup_queue->pop();
	return return_val;
}

void Level::add_wave(float time, Wave* w)
{
	waves->insert(new std::pair<float, Wave*>(time, w));
}

void Level::add_powerup(float time, Powerup* p)
{
	powerups->insert(new std::pair<float, Powerup*>(time, p));
}

Level::operator bool() const
{
	return !(waves->empty() && enemy_queue->empty() && powerups->empty() && powerup_queue->empty());
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

	for (std::pair<float, Powerup*>* p : *powerups)
	{
		delete p->second;
		delete p;
	}
	powerups->clear();
	delete powerups;

	while (!enemy_queue->empty())
	{
		delete enemy_queue->front();
		enemy_queue->pop();
	}
	delete enemy_queue;

	while (!powerup_queue->empty())
	{
		delete powerup_queue->front();
		powerup_queue->pop();
	}
	delete powerup_queue;
}


// ===== WAVE =====

Wave::Wave(const std::string& desc)
	: _desc(desc), spawnpoints(new std::unordered_set<Spawnpoint*>), enemy_queue(new std::queue<Entity*>)
{}

Wave* const Wave::clone() const
{
	Wave* w = new Wave(_desc);
	for (Spawnpoint* sp : *(this->spawnpoints))
	{
		w->spawnpoints->insert(sp->clone());
	}
	return w;
}

void Wave::update(float ms)
{
	for (Spawnpoint* sp : *spawnpoints)
	{
		sp->update(ms);
		if (sp->can_spawn())
			enemy_queue->push(sp->spawn());
	}
}

bool Wave::can_spawn() const
{
	return !(enemy_queue->empty());
}

Entity* const Wave::spawn() const
{
	Entity* return_val = enemy_queue->front();
	enemy_queue->pop();
	return return_val;
}

void Wave::add_spawnpoint(Spawnpoint* const s)
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

Spawnpoint::Spawnpoint(GObjFactory::ENEMY type, float perc_x, float perc_y, float angle, int amount, float spawn_delta, float initial_delay)
	: type(type), perc_x(perc_x), perc_y(perc_y), angle(angle), _spawn_delta(spawn_delta), _amount(amount), _initial_delay(initial_delay), _elapsed_time(spawn_delta - 0.1f)
{}

Spawnpoint* Spawnpoint::clone()
{
	return new Spawnpoint(type, perc_x, perc_y, angle, _amount, _spawn_delta, _initial_delay);
}

void Spawnpoint::update(float ms)
{
	_initial_delay -= (ms / 1000.0f);

	if (_initial_delay <= 0)
	{
		_elapsed_time += (ms / 1000.0f);
		// same as `if (_elapsed_time >= _spawn_delta) _elapsed_time = 0.0f;`
		_elapsed_time = ((0.0f * (_elapsed_time >= _spawn_delta)) + (_elapsed_time * (_elapsed_time < _spawn_delta)));
	}
}

bool Spawnpoint::can_spawn() const
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
