#include "game_data.h"
#include <iostream>
#include <regex>
#include <fstream>

GameData::GameData() : fps(0), game_state(0),
	el(0.0f), sps(4.0f), curr_img(0), images(),
	levels(), curr_active_level(-1), curr_selected_level(-1),
	enemyLs(new list<Entity*>), playerLs(new list<Entity*>), enemyProjLs(new list<Projectile*>), playerProjLs(new list<Projectile*>), effectsLs(new list<VisualEffect*>)
{
	// sets all the user-selectable levels to nullptr
	// to check later if a user-selectable level has been
	// defined (setting the nullptr to Level*) by calling `levels[id]`
	for (int i = 0; i < 10; ++i)
		levels[i] = nullptr;

	if (!load_levels_from_file(level_path, wave_path))
	{
		std::cerr << "Warning: Level loading from files failed, loading hardcoded levels..." << level_path << std::endl;
		load_hardcoded_levels();
	}

	// initialize other stuff ...
}


// doesn't work properly at the moment, it's just a template.
// code should be added where there are '...'
bool GameData::load_levels_from_file(const std::string& level_path, const std::string& wave_path) {

	if (!_load_waves_from_file(wave_path))
	{
		std::cerr << "Warning: Unable to load waves from file: " + level_path << std::endl;
		return false;
	}
	if (!_load_levels_from_file(level_path))
	{
		std::cerr << "Warning: Unable to load levels from file: " + level_path << std::endl;
		return false;
	}
	return true;
}

bool GameData::_load_waves_from_file(const std::string& wave_path)
{	// create stream to levels_path
	std::ifstream in(level_path);

	if (!in) {
		std::cerr << "Error opening file '" << level_path << "'" << std::endl;
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

bool GameData::_load_levels_from_file(const std::string& wave_path)
{
	// create stream to levels_path
	std::ifstream in(level_path);

	if (!in) {
		std::cerr << "Error opening file '" << level_path << "'" << std::endl;
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

void GameData::load_hardcoded_levels()
{
	Spawnpoint* sp11 = new Spawnpoint(3, 100.0f, 150.0f, -PI / 2, 10, 1.0f, 0.0f);

	Wave* w1 = new Wave("line");
	w1->add_spawnpoint(sp11);

	Spawnpoint* sp21 = new Spawnpoint(2, 300.0f, 300.0f, 0.0f, 2, 1.5f, 1.0f);
	Spawnpoint* sp22 = new Spawnpoint(2, 600.0f, 300.0f, 0.0f, 2, 3.0f, 1.0f);
	Spawnpoint* sp23 = new Spawnpoint(2, 900.0f, 300.0f, 0.0f, 2, 5.0f, 1.0f);

	Wave* w2 = new Wave("donuts");
	w2->add_spawnpoint(sp21);
	w2->add_spawnpoint(sp22);
	w2->add_spawnpoint(sp23);

	Level* l1 = new Level(-2, "owo");
	l1->add_wave(5.0f, w1);
	l1->add_wave(10.0f, w2);
	
/*	ABOUT LEVEL ID
	=>	no level can have an id of -1 (because -1 is the default)

	=>	levels selectable by users must have id 0-9 inclusive
		(see update::OP_LEVEL case for selectable levels)

	=>	levels not be selectable by users must have any other id
		(see above and also l1 and update::TEST case)

	=>	level id should always match its key in the levels map (see below)
*/
	levels[l1->id()] = l1;

	// dummy levels to see the level select screen
	Level *l4 = new Level(4, "xd");
	Level* l6 = new Level(6, "uwu");

	levels[l4->id()] = l4;
	levels[l6->id()] = l6;
}

GameData::~GameData() {
	deleteList(playerLs);
	deleteList(enemyLs);
	deleteList(enemyProjLs);
	deleteList(playerProjLs);
	deleteList(effectsLs);
}
