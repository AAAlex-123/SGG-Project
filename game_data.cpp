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

	Spawnpoint* sp11 = new Spawnpoint(3, 100.0f, 150.0f, -PI / 2, 10, 1.0f, 0.0f);

	Wave* w1 = new Wave();
	w1->add_spawnpoint(sp11);

	Spawnpoint* sp21 = new Spawnpoint(2, 300.0f, 300.0f, 0.0f, 2, 1.5f, 1.0f);
	Spawnpoint* sp22 = new Spawnpoint(2, 600.0f, 300.0f, 0.0f, 2, 3.0f, 1.0f);
	Spawnpoint* sp23 = new Spawnpoint(2, 900.0f, 300.0f, 0.0f, 2, 5.0f, 1.0f);

	Wave* w2 = new Wave();
	w2->add_spawnpoint(sp21);
	w2->add_spawnpoint(sp22);
	w2->add_spawnpoint(sp23);

	levels[0] = new Level(1, "owo");
	levels[0]->add_wave(5.0f, w1);
	levels[0]->add_wave(10.0f, w2);

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
