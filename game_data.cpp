#include "game_data.h"
#include <iostream>
#include <regex>
#include <fstream>

GameData::GameData() : fps(0), game_state(0),
	el(0.0f), sps(4.0f), curr_img(0), images(),
	levels(), curr_active_level(-1), curr_selected_level(-1),
	enemyLs(new list<Entity*>), playerLs(new list<Entity*>), enemyProjLs(new list<Projectile*>), playerProjLs(new list<Projectile*>), effectsLs(new list<VisualEffect*>)
{
	if (!load_levels_from_file(level_path))
		std::cerr << "Unable to load levels from: " << level_path << std::endl;

	// initialize other stuff ...

	Spawnpoint* sp11 = new Spawnpoint(3, 100.0f, 150.0f, -PI / 2, 6.0f, 1.0f, 0.0f);
	Spawnpoint* sp12 = new Spawnpoint(3, 100.0f, 300.0f, -PI / 2, 8.0f, 1.5f, 1.0f);
	Spawnpoint* sp13 = new Spawnpoint(3, 100.0f, 450.0f, -PI / 2, 10.0f, 2.0f, 2.0f);

	std::unordered_set<Spawnpoint*>* sps1 = new std::unordered_set<Spawnpoint*>;
	sps1->insert(sp11);
	sps1->insert(sp12);
	sps1->insert(sp13);

	Spawnpoint* sp21 = new Spawnpoint(2, 300.0f, 300.0f, 0.0f, 10.0f, 1.5f, 1.0f);
	Spawnpoint* sp22 = new Spawnpoint(2, 600.0f, 300.0f, 0.0f, 10.0f, 3.0f, 1.0f);
	Spawnpoint* sp23 = new Spawnpoint(2, 900.0f, 300.0f, 0.0f, 10.0f, 5.0f, 1.0f);

	std::unordered_set<Spawnpoint*>* sps2 = new std::unordered_set<Spawnpoint*>;
	sps2->insert(sp21);
	sps2->insert(sp22);
	sps2->insert(sp23);

	Wave* w1 = new Wave(sps1);
	Wave* w2 = new Wave(sps2);

	levels[0] = new Level(1, "owo");
	levels[0]->add_wave(5.0f, w2);
	levels[0]->add_wave(10.0f, w1);
}

// doesn't work properly at the moment, it's just a template.
// code should be added where there are '...'
bool GameData::load_levels_from_file(const std::string& levels_path) {
	// create stream to levels_path
	std::ifstream in(levels_path);

	if (!in) {
		std::cerr << "Error opening file '" << levels_path << "'" << std::endl;
		return false;
	}

	// get all lines from file
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	std::smatch match;

	// regex pattern 1 ...
	std::string sr1(".*");

	// regex objects ...
	std::regex r1;

	int curr_level = 0, curr_wave = 0, line = 0;

	while (true)
	{
		// construct regexes ...
		r1 = std::regex(std::to_string(line) + "   " + sr1);

		// match with 1st ...
		if (std::regex_search(contents, match, r1))
		{
			// ...
		}
		
		// no match
		else { break; }
		++line;
		contents = match.suffix();
	}

	// close stream
	in.close();
	return true;
}

GameData::~GameData() {
	deleteList(playerLs);
	deleteList(enemyLs);
	deleteList(enemyProjLs);
	deleteList(playerProjLs);
	deleteList(effectsLs);
}
