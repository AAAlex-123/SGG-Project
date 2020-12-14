#include "game_data.h"
#include <iostream>
#include <regex>
#include <fstream>

GameData::GameData() : fps(0), game_state(0),
	el(0.0f), sps(4.0f), curr_img(0), images(),
	levels(), curr_active_level(-1), curr_selected_level(-1)
{
	if (!load_levels_from_file(level_path))
		std::cerr << "Unable to load levels from: " << level_path << std::endl;

	// initialize other stuff ...
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
	deleteList<Entity>(playerLs);
	deleteList<Entity>(enemyLs);
	deleteList<Projectile>(enemyProjLs);
	deleteList<Projectile>(playerProjLs);
	deleteList<VisualEffect>(effectsLs);
	deleteList<Entity>(enemyQueue);
}
